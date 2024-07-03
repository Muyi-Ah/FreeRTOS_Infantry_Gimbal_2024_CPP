#pragma once
#include "stdint.h"
#include "usart.h"

UART_HandleTypeDef* const kUartRemote = &huart3;         //@config 遥控器UART实例
UART_HandleTypeDef* const kUartCommunication = &huart1;  //@config 板间通信UART实例
UART_HandleTypeDef* const kUartVision = &huart6;         //@config 视觉通信UART实例

const uint16_t kRemoteDataLength = 18;               //@config 遥控器数据长度
const uint16_t kCommunicationReciveDataLength = 15;  //@config 板间通信数据长度 后四字节用于校验
const uint16_t kVisionReceiveDataLength = 23;  //@config 视觉通信数据长度

class UartManager {
   protected:
    UART_HandleTypeDef* huart_ = nullptr;
    uint8_t* rx_buf_addr_ = nullptr;
    size_t rx_size_ = 0;
    size_t tx_size_ = 0;

   public:
    UartManager(UART_HandleTypeDef* huart, size_t rx_size = 0);
    ~UartManager();

    virtual void Send() = 0;  //UART发送虚函数

    void Init();
    auto get_huart() { return huart_; };
    auto get_rx_buf_address() { return rx_buf_addr_; };
    auto get_rx_size() { return rx_size_; };
};