/**
 * @file algo_crc.h
 * @author your name (you@domain.com)
 * @brief CRC 相关的实现
 * @version 0.1
 * @date 2025-12-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef ALGO_CRC_H
#define ALGO_CRC_H

#include <stdint.h>

#define CRC_DEFAULT_POLYNOMIAL 0x04C11DB7

int crc32_table_init(unsigned int polynomial, unsigned char reflectIn, unsigned char reflectOut);
unsigned int crc32_calculate(unsigned int crc, unsigned char const *buf, unsigned int len);
unsigned int crc32_calculate_word(unsigned int crc, const uint32_t *data, unsigned int count);

#endif /* ALGO_CRC_H */
