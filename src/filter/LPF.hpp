#pragma once
#include "stdint.h"

class LPF {
   private:
    float alpha_ = 0;
    float output_prev_ = 0;

   public:
    LPF(float alpha) { alpha_ = alpha; };
    ~LPF();

    float Compute(float input);
    void set_alpha(float value);
    void clear_output_prev();
};