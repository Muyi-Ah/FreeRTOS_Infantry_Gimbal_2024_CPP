#include "vision.hpp"
#include "stdio.h"

Vision::Vision(/* args */) {}

Vision::~Vision() {}

void Vision::DataUpdate(volatile const uint8_t* rx_buf) {

    auto recvParamCnt =
        sscanf((const char*)rx_buf, "[%4hu/%4hu/%4hu/%4hu/%1hu]", &robot_hub_yaw_error_,
               &robot_hub_pitch_error_, &armor_yaw_error_, &armor_pitch_error_, &fire_flag_);

    is_aimed_ = false;  // 假设接收失败

    // 数据有误
    if (recvParamCnt != 5) {
        is_aimed_ = false;
        is_fire_ = false;
        return;
    }

    if (robot_hub_yaw_error_ == 0 && robot_hub_pitch_error_ == 0 && armor_yaw_error_ == 0 &&
        armor_pitch_error_ == 0) {
        is_aimed_ = false;
        is_fire_ = false;
        return;
    }

    is_aimed_ = true;  //接收成功
    is_fire_ = fire_flag_;

    //目标中心模式
    if (aim_type_ == kRobotHub) {
        yaw_error_ = (float)(robot_hub_yaw_error_ - 1000);
        pitch_error_ = (float)(robot_hub_pitch_error_ - 1000);
    }
    //装甲板模式
    else if (aim_type_ == kArmor) {
        yaw_error_ = (float)(armor_yaw_error_ - 1000);
        pitch_error_ = (float)(armor_pitch_error_ - 1000);
    }
}