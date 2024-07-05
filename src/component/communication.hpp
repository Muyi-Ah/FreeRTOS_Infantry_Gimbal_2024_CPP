#pragma once
#include "bsp_uart.hpp"
#include "stdint.h"

class Communicator : public UartManager {
   private:
    bool is_reply_ = false;

   public:
    Communicator(UART_HandleTypeDef* huart, size_t rx_size);
    void Send() override;

    auto get_is_reply() { return is_reply_; };

    void set_is_reply(bool flag) { is_reply_ = flag; };

    void RecvDataUpdate();
};