#pragma once
#include "bsp_can.hpp"
#include "stdint.h"

class CH110 : public CanManager {
   private:
    volatile float roll_ = 0;
    volatile float pitch_ = 0;
    volatile float yaw_ = 0;
    float roll_prev_ = 0;
    float pitch_prev_ = 0;
    float yaw_prev_ = 0;
    float roll_integral_ = 0;   //@notice 此处作为积分值若精度不够，考虑换成double
    float pitch_integral_ = 0;  //@notice 此处作为积分值若精度不够，考虑换成double
    float yaw_integral_ = 0;    //@notice 此处作为积分值若精度不够，考虑换成double
    volatile int16_t x_velocity_ = 0;
    volatile int16_t y_velocity_ = 0;
    volatile int16_t z_velocity_ = 0;
    bool is_reply_ = false;  //IMU应答标志位
    float IntegralErrorCompute(float value, float* value_prev);

   public:
    using CanManager::CanManager;

    void EulerDataUpdate(const uint8_t* buf);
    void VelocityDataUpdate(const uint8_t* buf);

    auto get_roll() { return roll_; };
    auto get_pitch() { return pitch_; };
    auto get_yaw() { return yaw_; };
    auto get_roll_integral() { return roll_integral_; };
    auto get_pitch_integral() { return pitch_integral_; };
    auto get_yaw_integral() { return yaw_integral_; };
    auto get_x_velocity() { return x_velocity_; };
    auto get_y_velocity() { return y_velocity_; };
    auto get_z_velocity() { return z_velocity_; };
    auto get_is_reply() { return is_reply_; };

    void set_is_reply(bool flag) { is_reply_ = flag; };

    void clear_roll_integral() { roll_integral_ = 0; };
    void clear_pitch_integral() { pitch_integral_ = 0; };
    void clear_yaw_integral() { yaw_integral_ = 0; };
};