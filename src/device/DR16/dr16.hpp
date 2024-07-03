#pragma once
#include "stdint.h"
#include "bsp_uart.hpp"

class DR16 : public UartManager {
   private:
    bool is_reply_ = false;  //DR16接收机应答标志位

   public:
    struct {
        uint16_t ch0_ = 1024;
        uint16_t ch1_ = 1024;
        uint16_t ch2_ = 1024;
        uint16_t ch3_ = 1024;
        uint8_t s1_ = 0;
        uint8_t s2_ = 0;
    } remote_;
    struct {
        int16_t x_axis_ = 0;
        int16_t y_axis_ = 0;
        int16_t z_axis_ = 0;
        uint8_t press_left_ = 0;
        uint8_t press_right_ = 0;
    } mouse_;
    union {
        uint16_t key_code_ = 0;
        struct {
            uint16_t W_key_ : 1;
            uint16_t S_key_ : 1;
            uint16_t A_key_ : 1;
            uint16_t D_key_ : 1;
            uint16_t SHIFT_key_ : 1;
            uint16_t CTRL_key_ : 1;
            uint16_t Q_key_ : 1;
            uint16_t E_key_ : 1;
            uint16_t R_key_ : 1;
            uint16_t F_key_ : 1;
            uint16_t G_key_ : 1;
            uint16_t Z_key_ : 1;
            uint16_t X_key_ : 1;
            uint16_t C_key_ : 1;
            uint16_t V_key_ : 1;
            uint16_t B_key_ : 1;
        } key_;
    } key_board_;

    using UartManager::UartManager;

    void Send() override{};

    void DataUpdate();

    auto get_is_reply() { return is_reply_; };

    void set_is_reply(bool flag) { is_reply_ = flag; };
};