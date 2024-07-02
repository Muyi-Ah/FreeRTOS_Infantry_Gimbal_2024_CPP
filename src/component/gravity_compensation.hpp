#pragma once

class GravityCompensator {
   private:
    const float g_ = 9.81;  // 重力加速度 (m/s^2)
    float m_ = 0;           // 云台质量 (kg)
    float l_ = 0;           // 重心到旋转轴距离 (m)
    float k_t_ = 0;         // 电机力矩常数 (Nm/A)
    
   public:
    GravityCompensator(float m, float l, float k_t);
    ~GravityCompensator();

    float Compute();
};

class EmpiricalGravityCompensator {
   private:
    float C_ = 0;  //经验参数 试出来

   public:
    EmpiricalGravityCompensator(float C);
    ~EmpiricalGravityCompensator();

    float Compute();
};