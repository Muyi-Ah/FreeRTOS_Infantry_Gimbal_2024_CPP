#pragma once
#include "bsp_uart.hpp"
#include "stdint.h"

class Vofa : public UartManager {
   private:
    const uint8_t tail_[4] = {0x00, 0x00, 0x80, 0x7F};  //数据尾
    size_t count_;                                      //数据个数
    uint8_t* data_array_ = nullptr;                     //发送数组
    float* data_ = nullptr;                             //数据数组

   public:
    using UartManager::UartManager;
    Vofa(UART_HandleTypeDef* huart, size_t count, size_t rx_size = 0);
    ~Vofa();

    void set_data(size_t count, float value);
    void Send() override;
};