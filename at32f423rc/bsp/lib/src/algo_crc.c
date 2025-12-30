/**
 * @file algo_crc.c
 * @author your name (you@domain.com)
 * @brief CRC 相关的实现
 * @version 0.1
 * @date 2025-12-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "algo_crc.h"

static uint32_t crc32_run_table[256] = {0};

unsigned int Reverse(unsigned int value)
{
    value = ((value & 0xAAAAAAAA) >> 1) | ((value & 0x55555555) << 1);
    value = ((value & 0xCCCCCCCC) >> 2) | ((value & 0x33333333) << 2);
    value = ((value & 0xF0F0F0F0) >> 4) | ((value & 0x0F0F0F0F) << 4);
    value = ((value & 0xFF00FF00) >> 8) | ((value & 0x00FF00FF) << 8);
    value = (value >> 16) | (value << 16);

    return value;
}

int crc32_table_init(unsigned int polynomial, unsigned char reflectIn, unsigned char reflectOut)
{
    for (int byte = 0; byte < 256; ++byte)
    {
        unsigned int crc = (reflectIn ? (Reverse((unsigned int)(byte)) >> 24) : byte);

        for (int bit = 32; bit > 0; --bit)
        {
            if (crc & 0x80000000)
            {
                crc = (crc << 1) ^ polynomial;
            }
            else
            {
                crc <<= 1;
            }
        }

        crc32_run_table[byte] = (reflectOut ? Reverse(crc) : crc);
    }

    return 1;
}

unsigned int crc32_calculate(unsigned int crc, unsigned char const *buf, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
    {
        crc = crc32_run_table[(crc >> 24) ^ buf[i]] ^ (crc << 8);
    }
    return crc;
}

/**
 * @brief 按32位字计算CRC（匹配AT32硬件CRC处理方式）
 * @param crc 初始CRC值
 * @param data 32位数据数组指针
 * @param count 数据个数（32位字的数量）
 * @return 计算后的CRC值
 * @note 按大端序处理每个32位字，匹配AT32硬件CRC的行为
 */
unsigned int crc32_calculate_word(unsigned int crc, const uint32_t *data, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        /* 将32位字按大端序拆分为4个字节 */
        uint8_t bytes[4];
        bytes[0] = (data[i] >> 24) & 0xFF; /* 高字节先处理 */
        bytes[1] = (data[i] >> 16) & 0xFF;
        bytes[2] = (data[i] >> 8) & 0xFF;
        bytes[3] = (data[i] >> 0) & 0xFF; /* 低字节后处理 */

        /* 逐字节计算CRC */
        for (int j = 0; j < 4; j++)
        {
            crc = crc32_run_table[(crc >> 24) ^ bytes[j]] ^ (crc << 8);
        }
    }
    return crc;
}
