/**
 ******************************************************************************
 * @file    ar_ringbuf.c
 * @author  TP-Thread
 * @brief   环形队列.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "ar_ringbuf.h"

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  初始化环形缓冲区
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  buffer: 缓冲区指针
 * @param  size: 缓冲区大小
 * @retval None
 */
void arRingBufInit(arRingBuf_t *ptRingBuf, uint8_t *buffer, uint32_t size)
{
    if (ptRingBuf == NULL || buffer == NULL || size == 0)
    {
        return;
    }

    ptRingBuf->buffer = buffer; // 指向用户提供的缓冲区
    ptRingBuf->size = size;     // 缓冲区大小
    ptRingBuf->head = 0;        // 指向读取位置
    ptRingBuf->tail = 0;        // 指向写入位置
    ptRingBuf->count = 0;       // 环形缓冲区数据个数
    ptRingBuf->full = 0;        // 满标志位
}

/**
 * @brief  向环形缓冲区写入一个字节
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  data: 要写入的数据
 * @retval 1: 成功, 0: 失败(缓冲区满)
 */
uint8_t arRingBufWrite(arRingBuf_t *ptRingBuf, uint8_t data)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL)
    {
        return 0;
    }

    if (ptRingBuf->count >= ptRingBuf->size)
    {
        return 0; // 缓冲区满
    }

    ptRingBuf->buffer[ptRingBuf->tail] = data;
    ptRingBuf->tail = (ptRingBuf->tail + 1) % ptRingBuf->size;
    ptRingBuf->count++;

    // 检查是否已满
    if (ptRingBuf->count >= ptRingBuf->size)
    {
        ptRingBuf->full = 1;
    }

    return 1;
}

/**
 * @brief  从环形缓冲区读取一个字节
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  data: 读取数据的存储位置
 * @retval 1: 成功, 0: 失败(缓冲区空)
 */
uint8_t arRingBufRead(arRingBuf_t *ptRingBuf, uint8_t *data)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL || data == NULL)
    {
        return 0;
    }

    if (ptRingBuf->count == 0)
    {
        return 0; // 缓冲区空
    }

    *data = ptRingBuf->buffer[ptRingBuf->head];
    ptRingBuf->head = (ptRingBuf->head + 1) % ptRingBuf->size;
    ptRingBuf->count--;

    // 读取数据后清除满标志位
    if (ptRingBuf->full)
    {
        ptRingBuf->full = 0;
    }

    return 1;
}

/**
 * @brief  向环形缓冲区写入数组
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  data: 要写入的数据数组
 * @param  len: 数据长度
 * @retval 1: 成功, 0: 失败(空间不足)
 */
uint8_t arRingBufWriteArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL || data == NULL || len == 0)
    {
        return 0;
    }

    if (arRingBufFreeSpace(ptRingBuf) < len)
    {
        return 0; // 空间不足
    }

    for (uint32_t i = 0; i < len; i++)
    {
        ptRingBuf->buffer[ptRingBuf->tail] = data[i];
        ptRingBuf->tail = (ptRingBuf->tail + 1) % ptRingBuf->size;
        ptRingBuf->count++;
    }

    // 检查是否已满
    if (ptRingBuf->count >= ptRingBuf->size)
    {
        ptRingBuf->full = 1;
    }

    return 1;
}

/**
 * @brief  从环形缓冲区读取数组
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  data: 读取数据的存储位置
 * @param  len: 要读取的数据长度
 * @retval 实际读取的数据长度
 */
uint32_t arRingBufReadArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL || data == NULL || len == 0)
    {
        return 0;
    }

    uint32_t read_len = (len > ptRingBuf->count) ? ptRingBuf->count : len;

    for (uint32_t i = 0; i < read_len; i++)
    {
        data[i] = ptRingBuf->buffer[ptRingBuf->head];
        ptRingBuf->head = (ptRingBuf->head + 1) % ptRingBuf->size;
        ptRingBuf->count--;
    }

    // 读取数据后清除满标志位
    if (ptRingBuf->full && read_len > 0)
    {
        ptRingBuf->full = 0;
    }

    return read_len;
}

/**
 * @brief  获取环形缓冲区中可用数据的数量
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @retval 可用数据数量
 */
uint32_t arRingBufAvailable(arRingBuf_t *ptRingBuf)
{
    if (ptRingBuf == NULL)
    {
        return 0;
    }

    return ptRingBuf->count;
}

/**
 * @brief  获取环形缓冲区中剩余空间大小
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @retval 剩余空间大小
 */
uint32_t arRingBufFreeSpace(arRingBuf_t *ptRingBuf)
{
    if (ptRingBuf == NULL)
    {
        return 0;
    }

    return ptRingBuf->size - ptRingBuf->count;
}

/**
 * @brief  检查环形缓冲区是否为空
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @retval 1: 为空, 0: 不为空
 */
uint8_t arRingBufIsEmpty(arRingBuf_t *ptRingBuf)
{
    if (ptRingBuf == NULL)
    {
        return 1;
    }

    return (ptRingBuf->count == 0) ? 1 : 0;
}

/**
 * @brief  检查环形缓冲区是否已满
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @retval 1: 已满, 0: 未满
 */
uint8_t arRingBufIsFull(arRingBuf_t *ptRingBuf)
{
    if (ptRingBuf == NULL)
    {
        return 0;
    }

    return ptRingBuf->full;
}

/**
 * @brief  清空环形缓冲区
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @retval None
 */
void arRingBufClear(arRingBuf_t *ptRingBuf)
{
    if (ptRingBuf == NULL)
    {
        return;
    }

    ptRingBuf->head = 0;
    ptRingBuf->tail = 0;
    ptRingBuf->count = 0;
    ptRingBuf->full = 0;
}

/**
 * @brief  查看指定偏移位置的数据(不移除数据)
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  offset: 偏移量(0表示下一个要读取的数据)
 * @retval 查看到的数据
 */
uint8_t arRingBufPeek(arRingBuf_t *ptRingBuf, uint32_t offset)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL)
    {
        return 0;
    }

    if (offset >= ptRingBuf->count)
    {
        return 0; // 偏移量超出范围
    }

    uint32_t pos = (ptRingBuf->head + offset) % ptRingBuf->size;
    return ptRingBuf->buffer[pos];
}

/**
 * @brief  查看指定偏移位置的数据数组(不移除数据)
 * @param  ptRingBuf: 环形缓冲区结构体指针
 * @param  data: 存储查看数据的缓冲区
 * @param  len: 要查看的数据长度
 * @param  offset: 偏移量
 * @retval 实际查看的数据长度
 */
uint32_t arRingBufPeekArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len, uint32_t offset)
{
    if (ptRingBuf == NULL || ptRingBuf->buffer == NULL || data == NULL || len == 0)
    {
        return 0;
    }

    if (offset >= ptRingBuf->count)
    {
        return 0; // 偏移量超出范围
    }

    uint32_t available = ptRingBuf->count - offset;
    uint32_t read_len = (len > available) ? available : len;

    for (uint32_t i = 0; i < read_len; i++)
    {
        uint32_t pos = (ptRingBuf->head + offset + i) % ptRingBuf->size;
        data[i] = ptRingBuf->buffer[pos];
    }

    return read_len;
}
