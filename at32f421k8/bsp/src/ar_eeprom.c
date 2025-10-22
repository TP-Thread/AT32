/**
 ******************************************************************************
 * @file    ar_eeprom.c
 * @author  TP-Thread
 * @brief   i2c EEPROM(AT24C02)应用.
 ******************************************************************************
 */

#include "ar_eeprom.h"

extern I2C_HandleTypeDef hi2c1;

/**
 * @brief   在指定地址写一个字节到I2C EEPROM中
 * @param   addr:要写入数据的地址 0-255
 * @param   data:要写入字节数据
 * @retval  状态：0-成功，非0-错误状态
 */
uint8_t eeprom_write_byte(uint16_t addr, uint8_t data)
{
    uint8_t status;
    status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_DEVICE_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);

    if (status != 0)
    {
        return status; // 返回错误状态
    }

    // AT24C02写入后需要等待内部写周期完成（典型值5ms）
    // 使用设备就绪检测等待写入完成
    uint32_t timeout = HAL_GetTick() + 10; // 10ms超时
    while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_DEVICE_ADDR, 1, 1) != HAL_OK)
    {
        if (HAL_GetTick() > timeout)
        {
            return 0xFF; // 返回超时错误
        }
    }

    return 0;
}

/**
 * @brief  在指定地址写多个字节（支持跨页写入）
 * @param  addr: 要写入数据的起始地址 0-255
 * @param  pbuf: 指向要写入数据的指针
 * @param  size: 要写入的字节数
 * @retval 状态：0-成功，非0-错误状态
 */
uint8_t eeprom_write_bytes(uint8_t addr, uint8_t *pbuf, uint8_t size)
{
    uint8_t status = 0;
    uint8_t current_addr = addr;
    uint8_t *current_buf = pbuf;
    uint8_t remaining_size = size;
    uint8_t page_remaining;
    uint8_t write_size;

    while (remaining_size > 0)
    {
        // 计算当前页内剩余可写入的字节数
        page_remaining = EEPROM_PAGE_SIZE - (current_addr % EEPROM_PAGE_SIZE);

        // 确定本次写入的字节数（不超过页边界）
        write_size = (remaining_size < page_remaining) ? remaining_size : page_remaining;

        // 执行页内写入
        status = HAL_I2C_Mem_Write(&hi2c1, EEPROM_DEVICE_ADDR, current_addr,
                                   I2C_MEMADD_SIZE_8BIT, current_buf, write_size, 100);

        if (status != 0)
        {
            return status; // 返回错误状态
        }

        // AT24C02写入后需要等待内部写周期完成（典型值5ms）
        // 使用设备就绪检测等待写入完成
        uint32_t timeout = HAL_GetTick() + 10; // 10ms超时
        while (HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_DEVICE_ADDR, 1, 1) != HAL_OK)
        {
            if (HAL_GetTick() > timeout)
            {
                return 0xFF; // 返回超时错误
            }
        }

        // 更新参数准备下一页写入
        current_addr += write_size;
        current_buf += write_size;
        remaining_size -= write_size;
    }

    return 0; // 写入成功
}

/**
 * @brief  从指定地址读取多个字节
 * @param  addr: 要读取数据的起始地址 0-255
 * @param  pbuf: 指向读取数据缓冲区的指针
 * @param  size: 要读取的字节数
 * @retval 状态：0-成功，非0-错误状态
 */
uint8_t eeprom_read_bytes(uint8_t addr, uint8_t *pbuf, uint8_t size)
{
    uint8_t status;

    // AT24C02读取没有页限制，可以连续读取
    // 但要注意不要超出EEPROM地址范围（0-255）
    if ((uint16_t)(addr + size) > 256)
    {
        return 0xFD; // 地址超出范围
    }

    // 执行I2C内存读取
    status = HAL_I2C_Mem_Read(&hi2c1, EEPROM_DEVICE_ADDR, addr,
                              I2C_MEMADD_SIZE_8BIT, pbuf, size, 1000);

    if (status != 0)
    {
        return status; // 返回HAL错误状态
    }

    return 0; // 读取成功
}

/**
 * @brief   检查AT24CXX是否正常，这里用了24XX的最后一个地址(255)来存储标志字.
 *          如果用其他24C系列,这个地址要修改
 * @retval  检测成功返回0 失败返回1
 */
uint8_t eeprom_check_id(void)
{
    uint8_t device_id = 0;
    eeprom_read_bytes(AT24CXX, &device_id, 1);

    if (device_id != 0x02)
    {
        // 标志字错误,可能是第一次使用,也可能是EEPROM损坏
        eeprom_write_byte(AT24CXX, 0x02);
        printf("检测不到AT24C02\r\n");
        return 0;
    }
    else
    {
        printf("AT24C02 ID = %d\r\n", device_id);
        return 1;
    }
}