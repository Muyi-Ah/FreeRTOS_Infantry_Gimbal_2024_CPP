#pragma once
#include "stdint.h"

class PidController {
   private:
    float kp_ = 0;
    float ki_ = 0;
    float kd_ = 0;
    volatile float prev_error_ = 0;
    volatile float integral_ = 0;
    float min_integral_ = 0;
    float max_integral_ = 0;
    float min_output_ = 0;
    float max_output_ = 0;
    float clamp(float value, float min, float max);

   public:
    PidController(float kp, float ki, float kd, float min_integral, float max_integral,
                  float min_output, float max_output);
    ~PidController();
    void ParameterSet(float kp, float ki, float kd, float min_integral, float max_integral,
                      float min_output, float max_output);
    float Compute(float target, float measure);
    float Compute(float error);
    void Reset();
};