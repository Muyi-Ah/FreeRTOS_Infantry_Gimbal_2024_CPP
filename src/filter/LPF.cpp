#include "LPF.hpp"

LPF::~LPF() {}

/**
 * @brief 低通滤波器计算
 * 
 * @param input 输入值
 * @return float 滤波值
 */
float LPF::Compute(float input) {
    float output = alpha_ * input + (1 - alpha_) * output_prev_;
    output_prev_ = output;
    return output;
}

void LPF::set_alpha(float value) {
    alpha_ = value;
}

void LPF::clear_output_prev() {
    output_prev_ = 0;
}