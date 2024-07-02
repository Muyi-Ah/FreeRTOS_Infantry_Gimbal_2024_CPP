#pragma once
#include "can.h"
#include "stdint.h"

class Vofa {
   private:
    const uint8_t tail_[4] = {0x00, 0x00, 0x80, 0x7F};  //数据尾
    size_t count_;                                      //数据个数
    UART_HandleTypeDef* huart_ = nullptr;               //UART实例指针
    uint8_t* data_array_ = nullptr;                     //发送数组
    float* data_ = nullptr;                             //数据数组

   public:
    Vofa(UART_HandleTypeDef* huart, size_t count);
    ~Vofa();
    void set_data(size_t count, float value);
    void Send();
};