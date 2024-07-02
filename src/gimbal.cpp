#include "gimbal.hpp"
#include "clamp.hpp"
#include "cmsis_os2.h"
#include "init.hpp"
#include "stdint.h"

/*  =========================== 常量定义 ===========================  */

const auto kRemoteDeadBand = 10;
const auto kRemoteYawCoefficient = 0.03f;
const auto kRemotePitchCoefficient = 0.006f;
const auto kMouseYawCoefficient = 0.12f;
const auto kMousePitchCoefficient = 0.08f;
const auto kShotsPerFire = 2;
const auto kFrictionRpm = 4000;

/*  =========================== 函数声明 ===========================  */

void HaltFunction();
void SubMode11Function();
void SubMode12Function();
void SubMode13Function();
void SubMode21Function();
void SubMode22Function();
void SubMode23Function();
void SubMode31Function();
void SubMode32Function();
void SubMode33Function();
void TimeStampClear();
void SubStateUpdate();

/*  =========================== 变量定义 ===========================  */

//进入子模式时间戳
uint32_t enter_mode_11_timestamp;
uint32_t enter_mode_12_timestamp;
uint32_t enter_mode_13_timestamp;
uint32_t enter_mode_21_timestamp;
uint32_t enter_mode_22_timestamp;
uint32_t enter_mode_23_timestamp;
uint32_t enter_mode_31_timestamp;
uint32_t enter_mode_32_timestamp;
uint32_t enter_mode_33_timestamp;

//子模式时间戳
uint32_t mode_11_timestamp;
uint32_t mode_12_timestamp;
uint32_t mode_13_timestamp;
uint32_t mode_21_timestamp;
uint32_t mode_22_timestamp;
uint32_t mode_23_timestamp;
uint32_t mode_31_timestamp;
uint32_t mode_32_timestamp;
uint32_t mode_33_timestamp;

float yaw_target_euler;       //YAW轴目标欧拉角
float pitch_target_euler;     //PITCH轴目标欧拉角
float yaw_target_vision;      //YAW轴视觉目标
float pitch_target_vision;    //PITCH轴视觉目标
int16_t friction_target_rpm;  //摩擦轮目标转速
int32_t trigger_target_pos;   //拨盘目标位置

/*  =========================== 函数定义 ===========================  */

/**
 * @brief 
 * 
 * @param argument 
 */
void GimbalTask(void* argument) {
    for (;;) {

        //解决串口ORE问题
        if (__HAL_UART_GET_FLAG(uart_remote.get_huart(), UART_FLAG_ORE) != RESET) {

            __HAL_UART_CLEAR_OREFLAG(uart_remote.get_huart());  //清除ORE位

            //重新启动接收
            HAL_UARTEx_ReceiveToIdle_DMA(uart_remote.get_huart(), uart_remote.get_rx_buf_address(),
                                         uart_remote.get_rx_size());
        }

        //解决串口ORE问题
        if (__HAL_UART_GET_FLAG(uart_communication.get_huart(), UART_FLAG_ORE) != RESET) {

            __HAL_UART_CLEAR_OREFLAG(uart_communication.get_huart());  //清除ORE位

            //重新启动接收
            HAL_UARTEx_ReceiveToIdle_DMA(uart_communication.get_huart(),
                                         uart_communication.get_rx_buf_address(),
                                         uart_communication.get_rx_size());
        }

        //解决串口ORE问题
        if (__HAL_UART_GET_FLAG(uart_vision.get_huart(), UART_FLAG_ORE) != RESET) {

            __HAL_UART_CLEAR_OREFLAG(uart_vision.get_huart());  //清除ORE位

            //重新启动接收
            HAL_UARTEx_ReceiveToIdle_DMA(uart_vision.get_huart(), uart_vision.get_rx_buf_address(),
                                         uart_vision.get_rx_size());
        }

        SubStateUpdate();  //子状态更新

        //状态机
        switch (state_machine.get_main_state()) {
            case kMainStateNone:
                dji_motor_201.set_is_enable(false);
                dji_motor_202.set_is_enable(false);
                dji_motor_203.set_is_enable(false);
                dji_motor_205.set_is_enable(false);
                dji_motor_206.set_is_enable(false);
                break;

            case kOperate:

                switch (state_machine.get_sub_state()) {
                    case kSubStateNone:
                        dji_motor_201.set_is_enable(false);
                        dji_motor_202.set_is_enable(false);
                        dji_motor_203.set_is_enable(false);
                        dji_motor_205.set_is_enable(false);
                        dji_motor_206.set_is_enable(false);
                        break;

                    case kSubMode11:  //切到维护模式
                        state_machine.HandleEvent(kEventEnterHalt);
                        SubMode11Function();
                        break;

                    case kSubMode12:  //遥控器小陀螺顺时针模式
                        SubMode12Function();
                        dji_motor_201.set_is_enable(true);
                        dji_motor_202.set_is_enable(true);
                        dji_motor_203.set_is_enable(true);
                        dji_motor_205.set_is_enable(true);
                        dji_motor_206.set_is_enable(true);
                        break;

                    case kSubMode13:  //遥控器小陀螺逆时针模式
                        SubMode13Function();
                        dji_motor_201.set_is_enable(true);
                        dji_motor_202.set_is_enable(true);
                        dji_motor_203.set_is_enable(true);
                        dji_motor_205.set_is_enable(true);
                        dji_motor_206.set_is_enable(true);
                        break;

                    case kSubMode21:
                    case kSubMode22:
                    case kSubMode23:  //转场模式
                        SubMode23Function();
                        dji_motor_201.set_is_enable(true);
                        dji_motor_202.set_is_enable(true);
                        dji_motor_203.set_is_enable(true);
                        dji_motor_205.set_is_enable(true);
                        dji_motor_206.set_is_enable(true);
                        break;

                    case kSubMode31:
                    case kSubMode32:  //遥控射击模式
                        SubMode32Function();
                        dji_motor_201.set_is_enable(true);
                        dji_motor_202.set_is_enable(true);
                        dji_motor_203.set_is_enable(true);
                        dji_motor_205.set_is_enable(true);
                        dji_motor_206.set_is_enable(true);
                        break;

                    case kSubMode33:  //键鼠模式
                        SubMode33Function();
                        dji_motor_201.set_is_enable(true);
                        dji_motor_202.set_is_enable(true);
                        dji_motor_203.set_is_enable(true);
                        dji_motor_205.set_is_enable(true);
                        dji_motor_206.set_is_enable(true);
                        break;

                    default:
                        /* code */
                        break;
                }

                break;  // kOperate

            case kHalt:
                dji_motor_201.set_is_enable(false);
                dji_motor_202.set_is_enable(false);
                dji_motor_203.set_is_enable(false);
                dji_motor_205.set_is_enable(false);
                dji_motor_206.set_is_enable(false);
                HaltFunction();
                break;

            default:
                /* code */
                break;

        }  // state machine

        TimeStampClear();  //时间戳清除

        osDelay(1);  //延时
    }  // for
}

/**
 * @brief 目标值处理
 * 
 */
static void RemoteTargetHandle() {
    if (dr16.remote_.ch0_ < 1024 - kRemoteDeadBand || dr16.remote_.ch0_ > 1024 + kRemoteDeadBand) {
        yaw_target_euler += (float)((dr16.remote_.ch0_ - 1024) * kRemoteYawCoefficient);
    }
    if (dr16.remote_.ch1_ < 1024 - kRemoteDeadBand || dr16.remote_.ch1_ > 1024 + kRemoteDeadBand) {
        pitch_target_euler += (float)((dr16.remote_.ch1_ - 1024) * kRemotePitchCoefficient);
    }
}

/**
 * @brief 
 * 
 */
void HaltFunction() {

    //该阶段下YAW和PITCH的目标值需与实际值一致
    yaw_target_euler = ch110.get_yaw_integral();
    pitch_target_euler = clamp(ch110.get_pitch(), -5.0f, 30.0f);

    //该阶段下拨盘的目标值需与实际值一致
    trigger_target_pos = dji_motor_203.get_encoder_integral();
}

void SubMode11Function() {}
void SubMode12Function() {
    friction_target_rpm = 0;
    RemoteTargetHandle();  //目标值处理
}
void SubMode13Function() {
    friction_target_rpm = 0;
    RemoteTargetHandle();  //目标值处理
}
void SubMode21Function() {}
void SubMode22Function() {}
void SubMode23Function() {
    friction_target_rpm = 0;
    RemoteTargetHandle();  //目标值处理
}
void SubMode31Function() {}
void SubMode32Function() {

    RemoteTargetHandle();  //目标值处理

    //获取系统时间戳，单位为ms
    auto current_timestamp = osKernelGetSysTimerCount();  // 需configTICK_RATE_HZ配置为1000

    if (enter_mode_32_timestamp == 0) {  //刚进入该模式
        enter_mode_32_timestamp = current_timestamp;
        friction_target_rpm = 0;  //摩擦轮停转
    }
    //1秒后
    else if ((current_timestamp - enter_mode_32_timestamp) >= 1000) {
        friction_target_rpm = kFrictionRpm;  //摩擦轮启动

        //拨杆推到底
        if (dr16.remote_.ch3_ > 1684 - kRemoteDeadBand) {
            if (mode_32_timestamp == 0) {
                mode_32_timestamp = current_timestamp;
            }
            //每500ms
            else if (current_timestamp - mode_32_timestamp >= 500) {
                trigger_target_pos +=
                    49152 * kShotsPerFire;  //假设使用M2006 P36且拨盘每圈6颗弹丸 :8192*36/6=49152
                mode_32_timestamp = current_timestamp;
            }
        }
    }
}
void SubMode33Function() {
    //获取系统时间戳，单位为ms
    uint32_t current_timestamp = osKernelGetSysTimerCount();  // 需configTICK_RATE_HZ配置为1000

    //G键按下
    if (dr16.key_board_.key_.G_key_) {
        friction_target_rpm = kFrictionRpm;  //摩擦轮启动
    }
    //B键按下
    if (dr16.key_board_.key_.B_key_) {
        friction_target_rpm = 0;  //摩擦轮关闭
    }
    //鼠标左键按下
    if (dr16.mouse_.press_left_) {
        if (mode_33_timestamp == 0) {
            mode_33_timestamp = current_timestamp;
        }
        //每500ms
        else if (current_timestamp - mode_33_timestamp >= 100) {
            //假设使用M2006 P36且拨盘每圈6颗弹丸 :8192*36/6=49152
            trigger_target_pos += 49152 * kShotsPerFire;
            mode_32_timestamp = current_timestamp;
        }
    }
    //鼠标右键按下且已瞄准目标
    if (dr16.mouse_.press_right_ && vision.get_is_aimed()) {

        vision.set_use(true);  //视觉使用标志位置1

        yaw_target_vision = vision.get_yaw_error();
        pitch_target_vision = vision.get_pitch_error();
    }
    //未启动视觉
    else {

        vision.set_use(false);  //视觉使用标志位置0

        //鼠标横移
        if (dr16.mouse_.x_axis_) {
            yaw_target_euler += (float)(dr16.mouse_.x_axis_ * kMouseYawCoefficient);
        }
        //鼠标纵移
        if (dr16.mouse_.y_axis_) {
            pitch_target_euler += (float)(dr16.mouse_.y_axis_ * kMousePitchCoefficient);
        }
    }
}

void TimeStampClear() {
    if (state_machine.get_sub_state() != kSubMode11) {
        enter_mode_11_timestamp = 0;
        mode_11_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode12) {
        enter_mode_12_timestamp = 0;
        mode_12_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode13) {
        enter_mode_13_timestamp = 0;
        mode_13_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode21) {
        enter_mode_21_timestamp = 0;
        mode_21_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode22) {
        enter_mode_22_timestamp = 0;
        mode_22_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode23) {
        enter_mode_23_timestamp = 0;
        mode_23_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode31) {
        enter_mode_31_timestamp = 0;
        mode_31_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode32) {
        enter_mode_32_timestamp = 0;
        mode_32_timestamp = 0;
    }
    if (state_machine.get_sub_state() != kSubMode33) {
        enter_mode_33_timestamp = 0;
        mode_33_timestamp = 0;
    }
}

/**
 * @brief 
 * 
 */
void SubStateUpdate() {
    switch (dr16.remote_.s1_) {
        case 1:
            switch (dr16.remote_.s2_) {
                case 1:
                    state_machine.HandleEvent(kEventSwitchSubMode11);
                    break;
                case 3:
                    state_machine.HandleEvent(kEventSwitchSubMode12);
                    break;
                case 2:
                    state_machine.HandleEvent(kEventSwitchSubMode13);
                    break;

                default:
                    break;
            }
            break;
        case 3:
            switch (dr16.remote_.s2_) {
                case 1:
                    state_machine.HandleEvent(kEventSwitchSubMode21);
                    break;
                case 3:
                    state_machine.HandleEvent(kEventSwitchSubMode22);
                    break;
                case 2:
                    state_machine.HandleEvent(kEventSwitchSubMode23);
                    break;

                default:
                    break;
            }
            break;
        case 2:
            switch (dr16.remote_.s2_) {
                case 1:
                    state_machine.HandleEvent(kEventSwitchSubMode31);
                    break;
                case 3:
                    state_machine.HandleEvent(kEventSwitchSubMode32);
                    break;
                case 2:
                    state_machine.HandleEvent(kEventSwitchSubMode33);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}