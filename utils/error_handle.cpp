#include "error_handle.hpp"
#include "DJI_motor.hpp"
#include "ch110.hpp"
#include "communication.hpp"
#include "dr16.hpp"
#include "stddef.h"
#include "vision.hpp"

extern DjiMotor* dji_motor_list[];   //定义在init.cpp
extern const size_t kMotorCount;     //定义在init.cpp
extern DR16 dr16;                    //定义在init.cpp
extern CH110 ch110;                  //定义在init.cpp
extern Communication communication;  //定义在init.cpp
extern Vision vision;                //定义在init.cpp

/**
 * @brief 错误处理函数
 * 
 * @param error_type 错误类型
 */
void ErrorHandle(enum ErrorType error_type) {

    switch (error_type) {
        case kPointerError:  //指针错误
            while (1) {
                /* code */
            }
            break;

        case kHalLibError:  //HAL库错误
            while (1) {
                /* code */
            }
            break;

        case kSwitchError:  //Switch分支错误
            while (1) {
                /* code */
            }
            break;

        case kMotorError:  //电机错误
            //@develop 晚点再写
            break;

        case kDr16Error:  //DR16接收错误
            dr16.key_board_.key_code_ = 0;
            dr16.remote_.ch0_ = 1024;
            dr16.remote_.ch1_ = 1024;
            dr16.remote_.ch2_ = 1024;
            dr16.remote_.ch3_ = 1024;
            dr16.remote_.s1_ = 3;
            dr16.remote_.s2_ = 1;
            dr16.mouse_.x_axis_ = 0;
            dr16.mouse_.y_axis_ = 0;
            dr16.mouse_.press_left_ = 0;
            dr16.mouse_.press_right_ = 0;
            break;

        case kImuError:  //IMU错误
            //@develop 好似开香槟
            break;

        case kComuError:  //板间通信错误
                          //@develop 晚点写
            break;

        case kVisionError:               //视觉通信错误
            vision.set_use(false);       //清空使用标志位
            vision.set_is_aimed(false);  //清空接收成功标志位
            break;

        default:
            while (1) {
                /* code */
            }
            break;
    }
}

/**
 * @brief 错误检测
 * 
 */
void ErrorDetection() {
    static uint16_t motor_error_mask;  //电机错误掩码

    //电机错误判断
    for (size_t i = 0; i < kMotorCount; i++) {  //遍历电机对象
        if (dji_motor_list[i]->get_is_reply()) {
            dji_motor_list[i]->set_is_reply(false);  //清除应答标志位
            //@notice 收到对应电机报文时在中断处理函数里应答标志位会置为true

            motor_error_mask &= ~(0x0001 << i);  //电机错误掩码对应位置0
        } else {
            motor_error_mask |= (0x0001 << i);  //电机错误掩码对应位置1
        }
    }
    if (motor_error_mask != 0x0000) {  //掩码不为0
        ErrorHandle(kMotorError);
    }

    //接收机错误判断
    if (dr16.get_is_reply()) {
        dr16.set_is_reply(false);  //清除应答标志位
        //@notice 收到对应接收机报文时在中断处理函数里应答标志位会置为true
    } else {
        ErrorHandle(kDr16Error);
    }

    //IMU错误判断
    if (ch110.get_is_reply()) {
        ch110.set_is_reply(false);  //清除应答标志位
    } else {
        ErrorHandle(kImuError);
    }

    //板间通信错误判断
    if (communication.get_is_reply()) {
        communication.set_is_reply(false);  //清除应答标志位
    } else {
        ErrorHandle(kComuError);
    }

    //视觉通信错误判断
    if (vision.get_is_reply()) {
        vision.set_is_reply(false);  //清除应答标志位
    } else {
        ErrorHandle(kVisionError);
    }
}

/**
 * @brief 检查传入的指针是否为空
 * 
 * @param ptr 指针
 */
void CheckPointer(void* ptr) {
    if (ptr == nullptr) {
        ErrorHandle(kPointerError);
    }
}