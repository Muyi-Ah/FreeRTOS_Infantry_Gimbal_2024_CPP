#include "ude.hpp"

/**
 * @brief Construct a new UDE::UDE object
 * 
 * @param initial_x_n 系统初始状态
 * @param T 设计参数
 */
UDE::UDE(float initial_x_n, float T) {
    initial_x_n_ = initial_x_n;
    T_ = T;
}

/**
 * @brief Destroy the UDE::UDE object
 * 
 */
UDE::~UDE() {}

/**
 * @brief UDE计算
 * 
 * @param x_n 系统当前状态
 * @param u0 标称控制输入
 */
float UDE::compute(float x_n, float u0) {
    integral_u0_ += u0;
    float f_hat = (x_n - initial_x_n_ - integral_u0_) / T_;
    return f_hat;
}