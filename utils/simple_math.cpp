#include "simple_math.hpp"

const auto kPI = 3.141592653589793f;

float deg2rad(float degrees) {
    return degrees * (kPI / 180.0f);
}

float rad2deg(float radians) {
    return radians * (180.0f / kPI);
}