#include "pid.hpp"

/**
 * @brief Construct a new Pid Controller:: Pid Controller object
 * 
 * @param kp 误差系数
 * @param ki 积分系数
 * @param kd 微分系数
 * @param min_integral 最小积分值
 * @param max_integral 最大积分值
 * @param min_output 最小输出
 * @param max_output 最大输出
 */
PidController::PidController(float kp, float ki, float kd, float min_integral, float max_integral,
                             float min_output, float max_output) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    min_integral_ = min_integral;
    max_integral_ = max_integral;
    min_output_ = min_output;
    max_output_ = max_output;
}

/**
 * @brief Destroy the Pid Controller:: Pid Controller object
 * 
 */
PidController::~PidController() {}

/**
 * @brief 限幅
 * 
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return float 输出值
 */
float PidController::clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    }
    return value;
}

/**
 * @brief 参数设定
 * 
 * @param kp 误差系数
 * @param ki 积分系数
 * @param kd 微分系数
 * @param min_integral 最小积分值
 * @param max_integral 最大积分值
 * @param min_output 最小输出
 * @param max_output 最大输出
 */
void PidController::ParameterSet(float kp, float ki, float kd, float min_integral,
                                 float max_integral, float min_output, float max_output) {
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
    min_integral_ = min_integral;
    max_integral_ = max_integral;
    min_output_ = min_output;
    max_output_ = max_output;
}

/**
 * @brief PID计算
 * 
 * @param target 目标值
 * @param measure 实际值
 * @return float PID输出
 */
float PidController::Compute(float target, float measure) {
    float error = target - measure;
    integral_ += error;
    integral_ = clamp(integral_, min_integral_, max_integral_);
    float derivative = error - prev_error_;
    float output = kp_ * error + ki_ * integral_ + kd_ * derivative;
    output = clamp(output, min_output_, max_output_);
    return output;
}

/**
   * @brief PID计算（输入误差）
   * 
   * @param error 误差值
   * @return float PID输出
   */
float PidController::Compute(float error) {
    integral_ += error;
    integral_ = clamp(integral_, min_integral_, max_integral_);
    float derivative = error - prev_error_;
    float output = kp_ * error + ki_ * integral_ + kd_ * derivative;
    output = clamp(output, min_output_, max_output_);
    return output;
}

/**
   * @brief PID重置
   * 
   */
void PidController::Reset() {
    kp_ = 0;
    ki_ = 0;
    kd_ = 0;
    prev_error_ = 0;
    integral_ = 0;
    min_integral_ = 0;
    max_integral_ = 0;
    min_output_ = 0;
    max_output_ = 0;
}