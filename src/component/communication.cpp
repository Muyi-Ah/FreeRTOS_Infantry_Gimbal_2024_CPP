#include "communication.hpp"

Communicator::Communicator(UART_HandleTypeDef* huart, size_t rx_size)
    : UartManager(huart, rx_size) {}

void Communicator::Send(){

}

void Communicator::RecvDataUpdate(){
    
}