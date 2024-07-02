#pragma once
#include "stdint.h"

enum DirectionType {
    kBoth,
    kCW,
    kCCW,
};

class DjiMotor {
   private:
    uint32_t receive_id_ = 0;               //接收ID
    volatile uint16_t encoder_value_ = 0;   //编码器值
    uint16_t encoder_value_prev_ = 0;       //上一次的编码器值
    int32_t encoder_integral_ = 0;          //编码器值积分
    volatile int16_t actual_rpm_ = 0;       //实际转速值
    volatile uint16_t actual_current_ = 0;  //实际电流值
    volatile uint8_t temperatrue_ = 0;      //温度值
    int16_t input_ = 0;                     //电机输入值
    bool is_enable_ = false;                //电机使能标志位
    bool is_reply_ = false;                 //电机应答标志位
    int16_t IntegralErrorCompute();

   public:
    DjiMotor(uint32_t receive_id);
    ~DjiMotor();

    void DataUpdate(volatile const uint8_t* buf);
    int32_t AbsoluteErrorCompute(uint16_t target, enum DirectionType direction);

    auto get_recv_id() { return receive_id_; };
    auto get_encoder_value() { return encoder_value_; };
    auto get_encoder_integral() { return encoder_integral_; };
    auto get_actual_rpm() { return actual_rpm_; };
    auto get_actual_current() { return actual_current_; };
    auto get_temperature() { return temperatrue_; };
    auto get_input() { return input_; };
    auto get_is_enable() { return is_enable_; };
    auto get_is_reply() { return is_reply_; };

    void set_input(int16_t input) { input_ = input; };
    void set_is_enable(bool flag) { is_enable_ = flag; };
    void set_is_reply(bool flag) { is_reply_ = flag; };

    void clear_encoder_integral() { encoder_integral_ = 0; };
};

void DjiMotorSend();