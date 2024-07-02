#ifndef VERIFY_H
#define VERIFY_H

#include "stdint.h"

/**
 * @brief 算出列表末尾四个字节前的数据累加值
 * 
 * @param pBuffer 列表首地址
 * @param lenght 长度(字节)
 * @return int32_t 累加值
 */
extern int32_t summation(uint8_t* pBuffer, uint32_t lenght);

/**
 * @brief 附加校验值到列表最后四个字节
 * 
 * @param pBuffer 列表首地址
 * @param lenght 长度(字节)
 */
extern void append_accumulation_check_sum(uint8_t* pBuffer, uint32_t lenght);

/**
 * @brief 校验数据是否无误
 * 
 * @param pBuffer 列表首地址
 * @param lenght 长度(字节)
 * @return uint8_t true or false
 */
extern uint8_t verify_accumulation_check_sum(uint8_t* pBuffer, uint32_t lenght);

#endif  // VERIFY_H