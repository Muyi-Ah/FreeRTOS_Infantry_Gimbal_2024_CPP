#pragma once
#include "can.h"
#include "stdint.h"

CAN_HandleTypeDef* const kCanMotor = &hcan1;  //@config 电机can实例
CAN_HandleTypeDef* const kCanCh110 = &hcan2;  //@config CH110can实例

class CanManager {
   private:
    CAN_HandleTypeDef* hcan_ = nullptr;
    CAN_FilterTypeDef* filter_ = nullptr;
    // CAN_TxHeaderTypeDef* tx_header_ = nullptr;

   public:
    CanManager(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* filter);
    ~CanManager();

    void Send(const CAN_TxHeaderTypeDef* tx_header, const uint8_t* tx_buf);
    void Init();
};