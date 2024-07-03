#include "bsp_can.hpp"
#include "DJI_motor.hpp"
#include "ch110.hpp"
#include "error_handle.hpp"

/**
 * @brief Construct a new Can Manager:: Can Manager object
 * 
 * @param hcan CAN_HandleTypeDef的指针
 * @param filter CAN_FilterTypeDef的指针
 */
CanManager::CanManager(CAN_HandleTypeDef* hcan, CAN_FilterTypeDef* filter) {
    hcan_ = hcan;
    filter_ = filter;
}

extern const size_t kMotorCount;    //定义在init.cpp
extern DjiMotor* dji_motor_list[];  //定义在init.cpp
extern CH110 ch110;                 //定义在init.cpp
/**
 * @brief 
 * 
 * @param hcan 
 */
void CanRxFifo0Callback(CAN_HandleTypeDef* hcan) {

    if (hcan->Instance == kCanMotor->Instance) {

        CAN_RxHeaderTypeDef rx_header;
        uint8_t rx_buf[8];

        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_buf);  //接收数据

        //遍历寻找与报文ID对应的电机对象
        for (size_t i = 0; i < kMotorCount; i++) {
            if (rx_header.StdId == dji_motor_list[i]->get_recv_id()) {
                dji_motor_list[i]->DataUpdate(rx_buf);  //更新电机数据
                dji_motor_list[i]->set_is_reply(true);  //置位电机应答标志位
                break;
            }
        }
    } else if (hcan->Instance == kCanCh110->Instance) {

        CAN_RxHeaderTypeDef rx_header;
        uint8_t rx_buf[8];

        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_buf);  //接收数据

        if (rx_header.StdId == 0x388)  //@config 欧拉角接收ID可能会变
        {
            ch110.EulerDataUpdate(rx_buf);    //欧拉角更新
        } else if (rx_header.StdId == 0x288)  //@config 角速度接收ID可能会变
        {
            ch110.VelocityDataUpdate(rx_buf);  //角速度更新
        }
        ch110.set_is_reply(true);  //置位IMU应答标志位
    }
}

/**
 * @brief CAN报文发送
 * 
 * @param tx_buf 数据的地址
 */
void CanManager::Send(const CAN_TxHeaderTypeDef* tx_header, const uint8_t* tx_buf) {

    if (HAL_CAN_GetTxMailboxesFreeLevel(hcan_)) {
        HAL_CAN_AddTxMessage(hcan_, tx_header, tx_buf, (uint32_t*)CAN_TX_MAILBOX0);
    }
}

/**
 * @brief 
 * 
 */
void CanManager::CanInit() {

    CheckPointer(hcan_);    //检查地址
    CheckPointer(filter_);  //检查地址

    //注册中断函数
    if (HAL_CAN_RegisterCallback(hcan_, HAL_CAN_RX_FIFO0_MSG_PENDING_CB_ID, CanRxFifo0Callback) !=
        HAL_OK) {
        ErrorHandle(kHalLibError);
    }

    HAL_CAN_ConfigFilter(hcan_, filter_);  //配置CAN报文滤波器

    //启动CAN_IT_RX_FIFO0_MSG_PENDING中断通知
    HAL_CAN_ActivateNotification(hcan_, CAN_IT_RX_FIFO0_MSG_PENDING);

    HAL_CAN_Start(hcan_);  //启动CAN外设
}