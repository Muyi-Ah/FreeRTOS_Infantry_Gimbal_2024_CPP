#ifndef CLAMP_H
#define CLAMP_H

#include "stdint.h"

float clamp(float value, float min, float max);
int32_t clamp(int32_t value, int32_t min, int32_t max);

#endif // CLAMP_H