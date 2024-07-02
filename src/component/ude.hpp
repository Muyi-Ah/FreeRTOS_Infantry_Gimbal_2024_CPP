#pragma once
#include "stdint.h"

class UDE {
   private:
    float u0 = 0;            //标称控制输入
    float integral_u0_ = 0;  //标称控制输入积分
    float x_n_ = 0;          //系统当前状态
    float initial_x_n_ = 0;  //系统初始状态
    float T_ = 0;            //设计参数

   public:
    UDE(float initial_x_n, float T);
    ~UDE();
    
    void set_initial_x_n(float value){initial_x_n_ = value;};
    void set_T(float value){T_ = value;};
    void clear_initial_x_n(){initial_x_n_ = 0;};
    float compute(float x_n, float u0);
};