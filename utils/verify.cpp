#include "verify.hpp"

int32_t summation(uint8_t* pBuffer, uint32_t lenght) {
  int32_t result = 0;

  for (uint8_t index = 0; index < lenght - 4; index++) {
    result += pBuffer[index];
  }

  return result;
}

void append_accumulation_check_sum(uint8_t* pBuffer, uint32_t lenght) {
  int32_t result = 0;

  result = summation(pBuffer, lenght);

  pBuffer[lenght - 4] = result;
  pBuffer[lenght - 3] = result >> 8;
  pBuffer[lenght - 2] = result >> 16;
  pBuffer[lenght - 1] = result >> 24;
}

uint8_t verify_accumulation_check_sum(uint8_t* pBuffer, uint32_t lenght) {
  int32_t buffer_result;
  int32_t accumulation_result;

  buffer_result = pBuffer[lenght - 4] | pBuffer[lenght - 3] << 8 |
                  pBuffer[lenght - 2] << 16 | pBuffer[lenght - 1] << 24;

  accumulation_result = summation(pBuffer, lenght);

  if (buffer_result == accumulation_result) {
    return 1U;
  } else {
    return 0U;
  }
}