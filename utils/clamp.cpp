/**
 * @file clamp.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-06-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "clamp.hpp"

/**
 * @brief float类型限幅函数
 * 
 * @param value 目标值
 * @param min 最小值
 * @param max 最大值
 * @return float 输出值
 */
float clamp(float value, float min, float max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  }
  return value;
}

/**
 * @brief int32类型限幅函数
 * 
 * @param value 目标值
 * @param min 最小值
 * @param max 最大值
 * @return int32_t 输出值
 */
int32_t clamp(int32_t value, int32_t min, int32_t max) {
  if (value < min) {
    return min;
  } else if (value > max) {
    return max;
  }
  return value;
}