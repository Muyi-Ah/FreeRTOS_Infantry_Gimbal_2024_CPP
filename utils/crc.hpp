#ifndef CRC_H
#define CRC_H

#include "stdint.h"
#include "stdlib.h"

extern unsigned int Verify_CRC8_Check_Sum(unsigned char* pchMessage,
                                          unsigned int dwLength);
extern void Append_CRC8_Check_Sum(unsigned char* pchMessage,
                                  unsigned int dwLength);
extern uint32_t Verify_CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength);
extern void Append_CRC16_Check_Sum(uint8_t* pchMessage, uint32_t dwLength);

#endif  // CRC_H