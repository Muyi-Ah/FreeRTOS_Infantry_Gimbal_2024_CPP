#include "init.hpp"
#include "tim.h"

//  ==================== PID对象创建 ====================  //
//parameter: kp, ki, kd, ,最小积分值, 最大积分值, 最小输出值, 最大输出值

PidController pid_vel_201(0, 0, 0, 0, 0, 0, 0);  //摩擦轮速度环PID
PidController pid_vel_202(0, 0, 0, 0, 0, 0, 0);  //摩擦轮速度环PID
PidController pid_pos_203(0, 0, 0, 0, 0, 0, 0);  //拨盘外环（位置）PID
PidController pid_vel_203(0, 0, 0, 0, 0, 0, 0);  //拨盘内环（速度）PID
PidController pid_pos_205(0, 0, 0, 0, 0, 0, 0);  //YAW外环（位置）PID
PidController pid_vel_205(0, 0, 0, 0, 0, 0, 0);  //YAW内环（速度）PID
PidController pid_pos_206(0, 0, 0, 0, 0, 0, 0);  //PITCH外环（位置）PID
PidController pid_vel_206(0, 0, 0, 0, 0, 0, 0);  //PITCH内环（速度）PID

PidController vision_pid_pos_205(0, 0, 0, 0, 0, 0, 0);  //YAW视觉外环（误差）PID
PidController vision_pid_vel_205(0, 0, 0, 0, 0, 0, 0);  //YAW视觉内环（速度）PID
PidController vision_pid_pos_206(0, 0, 0, 0, 0, 0, 0);  //PITCH视觉外环（误差）PID
PidController vision_pid_vel_206(0, 0, 0, 0, 0, 0, 0);  //PITCH视觉内环（速度）PID

//  ==================== DJI电机对象创建 ====================  //

CAN_FilterTypeDef can_filter1 = {
    .FilterIdHigh = 0x0000,
    .FilterIdLow = 0x0000,
    .FilterMaskIdHigh = 0x0000,
    .FilterMaskIdLow = 0x0000,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterBank = 0,
    .FilterMode = CAN_FILTERMODE_IDMASK,
    .FilterScale = CAN_FILTERSCALE_32BIT,
    .FilterActivation = CAN_FILTER_ENABLE,
    .SlaveStartFilterBank = 14,
};

//parameter: CAN_HandleTypeDef*, CAN_FilterTypeDef*, 电机ID

DjiMotor dji_motor_201(kCanMotor, &can_filter1, 0x201);
DjiMotor dji_motor_202(kCanMotor, &can_filter1, 0x202);
DjiMotor dji_motor_203(kCanMotor, &can_filter1, 0x203);
DjiMotor dji_motor_205(kCanMotor, &can_filter1, 0x205);
DjiMotor dji_motor_206(kCanMotor, &can_filter1, 0x206);

//@config 放入电机对象结构体地址
DjiMotor* dji_motor_list[] = {&dji_motor_201, &dji_motor_202, &dji_motor_203, &dji_motor_205,
                              &dji_motor_206};

//计算出电机个数供其他文件使用
extern const size_t kMotorCount = sizeof(dji_motor_list) / sizeof(dji_motor_list[0]);
/*
 * @notice C++和C的const在文件可见性上不一样，C++的const可见性为本文件，
 * 因此若要在其他地方使用extern来使用该变量了，则需要在定义时加上extern
 */

//  ==================== CH110对象创建 ====================  //

CAN_FilterTypeDef can_filter2 = {
    .FilterIdHigh = 0x0000,
    .FilterIdLow = 0x0000,
    .FilterMaskIdHigh = 0x0000,
    .FilterMaskIdLow = 0x0000,
    .FilterFIFOAssignment = CAN_FILTER_FIFO0,
    .FilterBank = 15,
    .FilterMode = CAN_FILTERMODE_IDMASK,
    .FilterScale = CAN_FILTERSCALE_32BIT,
    .FilterActivation = CAN_FILTER_ENABLE,
    .SlaveStartFilterBank = 14,
};

//parameter: CAN_HandleTypeDef*, CAN_FilterTypeDef*

CH110 ch110(kCanCh110, &can_filter2);

//  ==================== 其他对象创建 ====================  //

StateMachine state_machine;  //状态机对象

//parameter: UART_HandleTypeDef*, 数据接收字节数

DR16 dr16(kUartRemote, kRemoteDataLength);  //DR16对象

//parameter: UART_HandleTypeDef*, 数据接收字节数

Communicator communicator(kUartCommunication, kCommunicationReciveDataLength);  //板间通信对象

//parameter: UART_HandleTypeDef*, 数据接收字节数

Vision vision(kUartVision, kVisionReceiveDataLength);  //视觉对象

//parameter: UART_HandleTypeDef*, 发送数据个数(float), 数据接收字节数

Vofa vofa(kUartVision, 1);  //VOFA对象 @config 需要修改值

//parameter: Alpha系数

LPF lpf_rpm_201(0.5);  //LPF对象 @config 需要修改值
LPF lpf_rpm_202(0.5);  //LPF对象 @config 需要修改值
LPF lpf_rpm_203(0.5);  //LPF对象 @config 需要修改值

//parameter: 经验参数（靠调试得出）

EmpiricalGravityCompensator EGC(1000);  //经验重力补偿对象

/**
 * @brief 初始化
 * 
 */
void init() {
    can_motor.Init();
    can_ch110.Init();
    dr16.Init();
    communicator.Init();
    vision.Init();

    HAL_TIM_Base_Start_IT(&htim6);  //@warning 完成各模块初始化后再启动定时器中断
}