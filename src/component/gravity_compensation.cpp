#include "gravity_compensation.hpp"
#include "arm_math.h"
#include "ch110.hpp"
#include "simple_math.hpp"

extern CH110 ch110;

/**
 * @brief Construct a new Gravity Compensator:: Gravity Compensator object
 * 
 * @param m 云台质量 (kg)
 * @param l 重心到旋转轴距离 (m)
 * @param k_t 电机力矩常数 (Nm/A)
 */
GravityCompensator::GravityCompensator(float m, float l, float k_t) {
    m_ = m;
    l_ = l;
    k_t_ = k_t;
}

GravityCompensator::~GravityCompensator() {}

/**
 * @brief 带模型的重力补偿计算
 * 
 * @return float 补偿力矩
 */
float GravityCompensator::Compute() {
    auto theta_degree = ch110.get_pitch();
    auto theta_radian = deg2rad(theta_degree);
    float tau_g = m_ * g_ * l_ * arm_cos_f32(theta_radian);  //计算重力矩 力矩 = 力 * 力臂
    float tau_c = tau_g / k_t_;  //计算重力矩抵消扭矩(电流值)的大小
    return tau_c;  //@notice 这里的单位我也不知道 不知道dji电机的电流值单位是什么 我就没打算用
}

/**
 * @brief Construct a new Empirical Gravity Compensator:: Empirical Gravity Compensator object
 * 
 * @param C 经验参数
 */
EmpiricalGravityCompensator::EmpiricalGravityCompensator(float C) {
    C_ = C;
}

EmpiricalGravityCompensator::~EmpiricalGravityCompensator() {}

/**
 * @brief 无模型的重力补偿计算
 * 
 * @return float 补偿力矩
 */
float EmpiricalGravityCompensator::Compute() {
    auto theta_degree = ch110.get_pitch();
    auto theta_radian = deg2rad(theta_degree);
    //计算力矩但用经验系数C 调试时自己试出合适的C的大小
    float tau_c = C_ * arm_cos_f32(theta_radian);
    return tau_c;
}