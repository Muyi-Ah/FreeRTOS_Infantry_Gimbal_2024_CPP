#include "vofa.hpp"
#include "string.h"
#include "bsp_uart.hpp"

/**
 * @brief Construct a new Vofa:: Vofa object
 * 
 * @param huart UART实例地址
 * @param count 数据个数
 */
Vofa::Vofa(UART_HandleTypeDef* huart, size_t count) {
    huart_ = huart;
    count_ = count;
    data_ = new float[count]{};
    data_array_ = new uint8_t[sizeof(float) * count + sizeof(tail_)]{};
}

/**
 * @brief Destroy the Vofa:: Vofa object
 * 
 */
Vofa::~Vofa() {
    delete[] data_array_;
    delete[] data_;
}

void Vofa::set_data(size_t count, float value) {
    data_[count] = value;
}

extern UartManager uart_vision;
/**
 * @brief 发送数据到VOFA上位机
 * 
 */
void Vofa::Send() {

    // //将data_内的每一个float变量到data_array_中
    // for (size_t i = 0; i < count_; i++) {
    //     memcpy(data_array_ + sizeof(float) * i, (uint8_t*)data_ + sizeof(float) * i, sizeof(float));
    // }
    memcpy(data_array_, (uint8_t*)data_, sizeof(data_));
    
    //将tail_放入到data_array_中
    memcpy(data_array_ + sizeof(float) * count_, tail_,sizeof(tail_));
    
    uart_vision.Send(data_array_, sizeof(data_array_));
}