#include "dr16.hpp"

/**
 * @brief DR16接收机数据更新
 * 
 * @param rx_buf_addr_ 数据的地址
 */
void DR16::DataUpdate() {
    remote_.ch0_ = (rx_buf_addr_[0] | rx_buf_addr_[1] << 8) & 0x07FF;
    remote_.ch1_ = (rx_buf_addr_[1] >> 3 | rx_buf_addr_[2] << 5) & 0x07FF;
    remote_.ch2_ =
        (rx_buf_addr_[2] >> 6 | rx_buf_addr_[3] << 2 | rx_buf_addr_[4] << 10) & 0x07FF;
    remote_.ch3_ = (rx_buf_addr_[4] >> 1 | rx_buf_addr_[5] << 7) & 0x07FF;
    remote_.s1_ = ((rx_buf_addr_[5] >> 4) & 0x000C) >> 2;
    remote_.s2_ = ((rx_buf_addr_[5] >> 4) & 0x0003);
    mouse_.x_axis_ = (rx_buf_addr_[6] | rx_buf_addr_[7] << 8);
    mouse_.y_axis_ = (rx_buf_addr_[8] | rx_buf_addr_[9] << 8);
    mouse_.z_axis_ = (rx_buf_addr_[10] | rx_buf_addr_[11] << 8);
    mouse_.press_left_ = rx_buf_addr_[12];
    mouse_.press_right_ = rx_buf_addr_[13];
    key_board_.key_code_ = (rx_buf_addr_[14] | rx_buf_addr_[15] << 8);
}