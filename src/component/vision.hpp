#pragma once
#include "stdint.h"

enum AimType {
    kArmor,
    kRobotHub,
};

class Vision {
   private:
    bool use_;                        //使用视觉标志位
    bool is_aimed_;                   //接收成功标志位
    bool is_fire_;                    //开火标志位
    enum AimType aim_type_;           //瞄准类型
    float yaw_error_;                 //YAW偏差值
    float pitch_error_;               //PITCH偏差值
    uint16_t robot_hub_yaw_error_;    //YAW中心点目标偏差值
    uint16_t robot_hub_pitch_error_;  //PITCH中心点目标偏差值
    uint16_t armor_yaw_error_;        //YAW装甲板目标偏差值
    uint16_t armor_pitch_error_;      //PITCH装甲板目标偏差值
    uint16_t fire_flag_;              //开火接收标志位
    bool is_reply_;                   //视觉应答标志位

   public:
    Vision(/* args */);
    ~Vision();

    void DataUpdate(volatile const uint8_t* rx_buf);

    void set_use(bool value) { use_ = value; };
    void set_is_aimed(bool flag){is_aimed_ = flag;};
    void set_aim_type(enum AimType type) { aim_type_ = type; };
    void set_is_reply(bool flag) { is_reply_ = flag; };

    auto get_use() { return use_; };
    auto get_is_aimed() { return is_aimed_; };
    auto get_is_fire() { return is_fire_; };
    auto get_yaw_error() { return yaw_error_; };
    auto get_pitch_error() { return pitch_error_; };
    auto get_is_reply() { return is_reply_; };
};