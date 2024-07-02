#include "bsp_uart.hpp"
#include "communication.hpp"
#include "dr16.hpp"
#include "error_handle.hpp"
#include "vision.hpp"

/**
 * @brief Construct a new Uart Manager:: Uart Manager object
 * 
 * @param huart UART_HandleTypeDef的指针
 * @param rx_buf 接收数组的指针
 * @param rx_size 接收数据大小
 */
UartManager::UartManager(UART_HandleTypeDef* huart, size_t rx_size = 0) {
    huart_ = huart;
    rx_size_ = rx_size;
    rx_buf_address = new uint8_t[rx_size];
}

UartManager::~UartManager() {
    delete[] rx_buf_address;
}

extern UartManager uart_remote;
extern UartManager uart_communication;
extern UartManager uart_vision;
extern DR16 dr16;
extern Communication communication;
extern Vision vision;
/**
 * @brief 
 * 
 * @param huart 
 * @param Size 
 */
void UartRxIdleCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    if (huart->Instance == kUartRemote->Instance) {
        if (Size == kRemoteDataLength) {
            dr16.DataUpdate(uart_remote.get_rx_buf_address());  //DR16数据更新
            dr16.set_is_reply(true);                            //置位DR16应答标志位
        }
    } else if (huart->Instance == kUartCommunication->Instance) {
        if (Size == kCommunicationReciveDataLength) {
            communication.DataUpdate(uart_communication.get_rx_buf_address());  //板间通信数据更新
            communication.set_is_reply(true);  //置位板间通信应答标志位
        }
    } else if (huart->Instance == kUartVision->Instance) {
        if (Size == kVisionReceiveDataLength) {
            vision.DataUpdate(uart_vision.get_rx_buf_address());  //视觉数据更新
            vision.set_is_reply(true);                            //置位视觉应答标志位
        }
    }
}

/**
 * @brief 
 * 
 * @param tx_buf 
 * @param size 
 */
void UartManager::Send(const uint8_t* tx_buf, size_t size) {
    HAL_UART_Transmit_DMA(huart_, (uint8_t*)tx_buf, size);
}

/**
 * @brief 
 * 
 */
void UartManager::Init() {
    CheckPointer(huart_);          //检查地址
    CheckPointer(rx_buf_address);  //检查地址

    //注册中断函数
    if (HAL_UART_RegisterRxEventCallback(huart_, UartRxIdleCallback) != HAL_OK) {
        ErrorHandle(kHalLibError);
    }
    HAL_UARTEx_ReceiveToIdle_DMA(huart_, rx_buf_address, rx_size_);  //启动接收
    //@notice 此处使用的是接收完成中断+空闲中断，接收仅使用空闲中断，不使用接收完成中断，逻辑是空闲后判断所接受的数据大小是否符合要求
}