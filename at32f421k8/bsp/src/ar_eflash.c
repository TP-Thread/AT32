/**
 ******************************************************************************
 * @作  者  没PF的老亚瑟
 * @版  本  V1.0
 * @日  期  2023-1-20
 * @内  容  SPI 读写 W25QXXX
 *
 ******************************************************************************
 */

#include "ar_eflash.h"

/**
 * @brief       读写一个字节数据，不能进行实际收发，供其他函数调用
 * @param
 * @arg 		data_w：要写入数据
 * @retval  	data_r：读取的数据
 */
uint8_t SPI_FLASH_Read_Write_Byte(uint8_t data_w)
{
    uint8_t data_r = 0;
    HAL_SPI_TransmitReceive(&hspi1, &data_w, &data_r, 1, 1000);

    return data_r;
}

/**
 * @brief  读取FLASH ID
 * @param 	无
 * @retval FLASH ID
 */
uint32_t SPI_FLASH_ReadID(void)
{
    uint32_t ID = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    // 通讯开始：CS 拉低
    SPI1_NSS_L();

    // 发送JEDEC指令，读取ID
    SPI_FLASH_Read_Write_Byte(W25X_JedecDeviceID);
    // 读取三个字节数据
    Temp0 = SPI_FLASH_Read_Write_Byte(Dummy_Byte);
    Temp1 = SPI_FLASH_Read_Write_Byte(Dummy_Byte);
    Temp2 = SPI_FLASH_Read_Write_Byte(Dummy_Byte);

    // 通讯结束：CS 拉高
    SPI1_NSS_H();

    // 把数据组合起来，作为函数的返回值
    ID = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return ID;
}

/**
 * @brief       读取FLASH中的一个字节
 * @param
 * @arg 		ReadAddr：要读取的数据所在地址,共24位 0~0xFFFFFF
 * @retval  	data_r：读取的数据
 */
uint8_t SPI_FLASH_ByteRead(uint32_t ReadAddr)
{
    uint8_t data_r = 0;

    // 通讯开始：CS 拉低
    SPI1_NSS_L();

    // 发送读命令
    SPI_FLASH_Read_Write_Byte(W25X_ReadData);
    // 发送要读取的数据的24位地址，分三次发送，高位先发
    SPI_FLASH_Read_Write_Byte(ReadAddr >> 16);
    SPI_FLASH_Read_Write_Byte(ReadAddr >> 8);
    SPI_FLASH_Read_Write_Byte(ReadAddr);
    // 读取数据
    data_r = SPI_FLASH_Read_Write_Byte(Dummy_Byte);

    // 通讯结束：CS 拉高
    SPI1_NSS_H();

    return data_r;
}

/**
 * @brief   读取FLASH数据
 * @param 	pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   BufferSize，读取数据长度
 * @retval  无
 */
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t BufferSize)
{
    // 通讯开始：CS 拉低
    SPI1_NSS_L();

    // 发送读指令
    SPI_FLASH_Read_Write_Byte(W25X_ReadData);
    // 发送要读取的数据的24位地址，分三次发送，高位先发
    SPI_FLASH_Read_Write_Byte(ReadAddr >> 16);
    SPI_FLASH_Read_Write_Byte(ReadAddr >> 8);
    SPI_FLASH_Read_Write_Byte(ReadAddr);
    // 读取数据
    while (BufferSize--)
    {
        *pBuffer = SPI_FLASH_Read_Write_Byte(Dummy_Byte);
        pBuffer++;
    }

    // 通讯结束：CS 拉高
    SPI1_NSS_H();
}

/**
 * @brief      SPI等待FLASH空闲，即所有写操作完成
 */
void SPI_FLASH_WaitForWriteEnd(void)
{
    uint8_t data_r = 0;

    // 通讯开始：CS 拉低
    SPI1_NSS_L();

    // 发送读状态寄存器1命令
    SPI_FLASH_Read_Write_Byte(W25X_ReadStatusReg);
    // 若 FLASH 忙碌，则等待
    do
    {
        // 读取 FLASH 芯片的状态寄存器
        data_r = SPI_FLASH_Read_Write_Byte(Dummy_Byte);
    } while ((data_r & 0x01) == 1);

    // 通讯结束：CS 拉高
    SPI1_NSS_H();
}

/**
 * @brief      擦除发送地址所在的扇区
 * @param
 * @arg 	   SectorAddr：要擦除的扇区中的任一数据的地址,最好写扇区首地址
 * @retval
 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
    // 写使能
    SPI1_NSS_L();
    SPI_FLASH_Read_Write_Byte(W25X_WriteEnable);
    SPI1_NSS_H();
    // 等待空闲
    SPI_FLASH_WaitForWriteEnd();

    // 发送扇区擦书指令
    SPI1_NSS_L();
    SPI_FLASH_Read_Write_Byte(W25X_SectorErase);
    // 发送24位地址，分三次发送，高位先发
    // 芯片会自动对齐到该地址所在的扇区首地址进行整扇区擦除
    SPI_FLASH_Read_Write_Byte(SectorAddr >> 16);
    SPI_FLASH_Read_Write_Byte(SectorAddr >> 8);
    SPI_FLASH_Read_Write_Byte(SectorAddr);

    // 通讯结束
    SPI1_NSS_H();
}

/**
 * @brief      对FLASH按页写入数据，本函数内部已擦除扇区，这种方式不太灵活
 * @param	   pBuffer，要写入数据的指针
 * @param      WriteAddr，要写入的扇区的地址，它会把这个地址所在的扇区给擦除
 * @param      BufferSize，写入数据长度，必须小于等于 FLASH_PageSize
 * @retval     无
 */
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t BufferSize)
{
    // 擦除扇区
    SPI_FLASH_SectorErase(WriteAddr);

    // 写使能
    SPI1_NSS_L();
    SPI_FLASH_Read_Write_Byte(W25X_WriteEnable);
    SPI1_NSS_H();

    // 发送页写指令
    SPI1_NSS_L();
    SPI_FLASH_Read_Write_Byte(W25X_PageProgram);
    SPI1_NSS_H();

    // 发送24位地址，分三次发送，高位先发
    SPI_FLASH_Read_Write_Byte(WriteAddr >> 16);
    SPI_FLASH_Read_Write_Byte(WriteAddr >> 8);
    SPI_FLASH_Read_Write_Byte(WriteAddr);

    if (BufferSize > FLASH_PageSize)
    {
        BufferSize = FLASH_PageSize;
        printf("SPI_FLASH_PageWrite too large!");
    }

    /* 写入数据*/
    while (BufferSize--)
    {
        /* 发送当前要写入的字节数据 */
        SPI_FLASH_Read_Write_Byte(*pBuffer);
        /* 指向下一字节数据 */
        pBuffer++;
    }

    /* 停止信号 FLASH: CS 高电平 */
    SPI1_NSS_H();

    /* 等待写入完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  对FLASH写入数据
 * @param  pBuffer，要写入数据的指针
 * @param  WriteAddr，写入地址（无要求首地址）
 * @param  BufferSize，写入数据长度
 * @retval 无
 */
void SPI_FLASH_BufferWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t BufferSize)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    // mod运算求余，若writeAddr是FLASH_PageSize整数倍，运算结果Addr值为0
    Addr = WriteAddr % FLASH_PageSize;

    // 差count个数据值，刚好可以对齐到页地址
    count = FLASH_PageSize - Addr;
    // 计算出要写多少整数页
    NumOfPage = BufferSize / FLASH_PageSize;
    // mod运算求余，计算出剩余不满一页的字节数
    NumOfSingle = BufferSize % FLASH_PageSize;

    // Addr=0,则WriteAddr 刚好按页对齐
    if (Addr == 0)
    {
        // BufferSize < FLASH_PageSize
        if (NumOfPage == 0)
        {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, BufferSize);
        }
        else // BufferSize > FLASH_PageSize
        {
            /*先把整数页都写了*/
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
                WriteAddr += FLASH_PageSize;
                pBuffer += FLASH_PageSize;
            }

            // 若有多余的不满一页的数据，把它写完
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    // 若地址与 FLASH_PageSize 不对齐
    else
    {
        // BufferSize < FLASH_PageSize
        if (NumOfPage == 0)
        {
            // 当前页剩余的count个位置比NumOfSingle小，写不完
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;

                // 先写满当前页
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr += count;
                pBuffer += count;

                // 再写剩余的数据
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            // 当前页剩余的count个位置能写完NumOfSingle个数据
            else
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, BufferSize);
            }
        }
        // BufferSize > FLASH_PageSize
        else
        {
            // 地址不对齐多出的count分开处理，不加入这个运算
            BufferSize -= count;
            NumOfPage = BufferSize / FLASH_PageSize;
            NumOfSingle = BufferSize % FLASH_PageSize;

            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr += count;
            pBuffer += count;

            // 把整数页都写了
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
                WriteAddr += FLASH_PageSize;
                pBuffer += FLASH_PageSize;
            }
            // 若有多余的不满一页的数据，把它写完
            if (NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/******************* (C) 版权 TP-Thread **************************************/
