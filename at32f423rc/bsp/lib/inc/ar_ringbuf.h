#ifndef AR_RINGBUF_H
#define AR_RINGBUF_H

#include "main.h"

/* 环形缓冲区结构体定义 */
typedef struct
{
    uint8_t *buffer; /* 缓冲区指针 */
    uint32_t size;   /* 缓冲区大小 */
    uint32_t head;   /* 头指针(读取位置) */
    uint32_t tail;   /* 尾指针(写入位置) */
    uint32_t count;  /* 当前数据个数 */
    uint8_t full;    /* 缓冲区满标志位 */
} arRingBuf_t;

/* 函数声明 */
void arRingBufInit(arRingBuf_t *ptRingBuf, uint8_t *buffer, uint32_t size);
uint8_t arRingBufWrite(arRingBuf_t *ptRingBuf, uint8_t data);
uint8_t arRingBufRead(arRingBuf_t *ptRingBuf, uint8_t *data);
uint8_t arRingBufWriteArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len);
uint32_t arRingBufReadArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len);
uint32_t arRingBufAvailable(arRingBuf_t *ptRingBuf);
uint32_t arRingBufFreeSpace(arRingBuf_t *ptRingBuf);
uint8_t arRingBufIsEmpty(arRingBuf_t *ptRingBuf);
uint8_t arRingBufIsFull(arRingBuf_t *ptRingBuf);
void arRingBufClear(arRingBuf_t *ptRingBuf);
uint8_t arRingBufPeek(arRingBuf_t *ptRingBuf, uint32_t offset);
uint32_t arRingBufPeekArray(arRingBuf_t *ptRingBuf, uint8_t *data, uint32_t len, uint32_t offset);

#endif /* AR_RINGBUF_H */
