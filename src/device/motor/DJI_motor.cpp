#include "DJI_motor.hpp"
#include "bsp_can.hpp"
#include "can.h"
#include "error_handle.hpp"
#include "stdlib.h"

/**
 * @brief Construct a new Dji Motor:: Dji Motor object
 * 
 * @param hcan CAN_HandleTypeDef的指针
 * @param filter CAN_FilterTypeDef的指针
 * @param receive_id 电机接收ID
 */
DjiMotor::DjiMotor(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* filter, uint32_t receive_id)
    : CanManager(hcan, filter), receive_id_(receive_id) {}

/**
 * @brief 编码器积分误差计算
 * @note 输入当前编码器值，返回当前编码器值相较于上一次编码器值的真实改变的差值
 */
int16_t DjiMotor::IntegralErrorCompute() {
    int16_t res1 = 0, res2 = 0;
    if (encoder_value_ - encoder_value_prev_ > 0) {
        res1 = encoder_value_ - encoder_value_prev_ - 8192;
        res2 = encoder_value_ - encoder_value_prev_;
        if (abs(res1) < abs(res2)) {
            return res1;
        } else {
            return res2;
        }

    } else {
        res1 = encoder_value_ - encoder_value_prev_ + 8192;
        res2 = encoder_value_ - encoder_value_prev_;
        if (abs(res1) < abs(res2)) {
            return res1;
        } else {
            return res2;
        }
    }
    encoder_value_prev_ = encoder_value_;  //更新上一次值
}

/**
 * @brief 电机数据更新
 * 
 * @param buf 数据的地址
 */
void DjiMotor::DataUpdate(volatile const uint8_t* buf) {
    encoder_value_ = buf[0] << 8 | buf[1];
    uint16_t rpm = buf[2] << 8 | buf[3];
    //计算成有符号的实际转速
    if (rpm < 32767) {
        actual_rpm_ = rpm;
    } else {
        actual_rpm_ = rpm - 65535;
    }
    actual_current_ = buf[4] << 8 | buf[5];
    temperatrue_ = buf[6];
    encoder_integral_ += IntegralErrorCompute();
    //@warning 务必在编码器值更新之后再使用integral_error_compute更新编码器积分值
}

/**
 * @brief 编码器绝对位置误差计算
 * @note 专门用于计算编码器绝对位置误差，返回误差给PID进行计算
 * @param target 目标值
 * @param direction 旋转方向
 * @return int32_t 
 */
int32_t DjiMotor::AbsoluteErrorCompute(uint16_t target, enum DirectionType direction) {
    int32_t small_value = 0;
    int32_t big_value = 0;

    switch (direction) {
        case kBoth:  //两边都能转
            if (target < encoder_value_) {
                small_value = target;
                big_value = encoder_value_;
            } else {
                small_value = encoder_value_;
                big_value = target;
            }

            if (big_value - small_value < 8192 - abs(small_value - big_value)) {
                if (target < encoder_value_) {
                    return -(big_value - small_value);
                } else {
                    return big_value - small_value;
                }
            } else {
                if (target < encoder_value_) {
                    return 8192 - abs(small_value - big_value);
                } else {
                    return -(8192 - abs(small_value - big_value));
                }
            }
            break;

        case kCW:  //顺时针转
            if (target < encoder_value_) {
                small_value = target;
                big_value = encoder_value_;
            } else {
                small_value = encoder_value_;
                big_value = target;
            }

            if (target < encoder_value_) {
                return 8192 - abs(small_value - big_value);
            } else {
                return big_value - small_value;
            }
            break;

        case kCCW:  //逆时针转
            if (target < encoder_value_) {
                small_value = target;
                big_value = encoder_value_;
            } else {
                small_value = encoder_value_;
                big_value = target;
            }

            if (target < encoder_value_) {
                return -(big_value - small_value);
            } else {
                return -(8192 - abs(small_value - big_value));
            }
            break;

        default:
            ErrorHandle(kSwitchError);  //错误处理
            break;
    }
}

extern DjiMotor dji_motor_list[];
extern const size_t kMotorCount;
extern DjiMotor dji_motor_201;
/**
 * @brief 电机控制数据发送
 * 
 */
void DjiMotorSend() {

    uint8_t motor_tx_buf[2][8];  //@notice 第一个是0x200用的，第二个是0x1FF用的

    //各电机输入值写入电机控制报文
    for (size_t i = 0; i < kMotorCount; i++) {
        switch (dji_motor_list[i].get_recv_id()) {
            case 0x201:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[0][0] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[0][1] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[0][0] = 0;
                    motor_tx_buf[0][1] = 0;
                }
                break;
            case 0x202:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[0][2] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[0][3] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[0][2] = 0;
                    motor_tx_buf[0][3] = 0;
                }
                break;
            case 0x203:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[0][4] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[0][5] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[0][4] = 0;
                    motor_tx_buf[0][5] = 0;
                }
                break;
            case 0x204:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[0][6] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[0][7] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[0][6] = 0;
                    motor_tx_buf[0][7] = 0;
                }
                break;
            case 0x205:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[1][0] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[1][1] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[1][0] = 0;
                    motor_tx_buf[1][1] = 0;
                }
                break;
            case 0x206:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[1][2] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[1][3] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[1][2] = 0;
                    motor_tx_buf[1][3] = 0;
                }
                break;
            case 0x207:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[1][4] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[1][5] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[1][4] = 0;
                    motor_tx_buf[1][5] = 0;
                }
                break;
            case 0x208:
                if (dji_motor_list[i].get_is_enable()) {
                    motor_tx_buf[1][6] = dji_motor_list[i].get_input() >> 8;
                    motor_tx_buf[1][7] = dji_motor_list[i].get_input();
                } else {
                    motor_tx_buf[1][6] = 0;
                    motor_tx_buf[1][7] = 0;
                }
                break;

            default:
                ErrorHandle(kSwitchError);  //错误处理
                break;
        }
    }

    CAN_TxHeaderTypeDef tx_header[2];

    tx_header[0].StdId = 0x200;
    tx_header[0].ExtId = 0;
    tx_header[0].IDE = CAN_ID_STD;
    tx_header[0].RTR = CAN_RTR_DATA;
    tx_header[0].DLC = 8;
    tx_header[0].TransmitGlobalTime = DISABLE;

    tx_header[1].StdId = 0x1FF;
    tx_header[1].ExtId = 0;
    tx_header[1].IDE = CAN_ID_STD;
    tx_header[1].RTR = CAN_RTR_DATA;
    tx_header[1].DLC = 8;
    tx_header[1].TransmitGlobalTime = DISABLE;

    //@warning 一个电机进行发送即可 不要多个电机对象发送 因为几个电机都使用同一个CAN
    dji_motor_201.Send(&tx_header[0], motor_tx_buf[0]);
    dji_motor_201.Send(&tx_header[1], motor_tx_buf[1]);
}
