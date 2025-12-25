/**
 ******************************************************************************
 * @file    ar_eeprom.c
 * @author  TP-Thread
 * @date    2025-08-31
 * @brief   MPU6050 操作.
 ******************************************************************************
 */

/* Includes -----------------------------------------------------------------*/
#include "ar_mpu6050.h"

/* Private variables --------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
int16_t g_gyro[3];  // 陀螺仪原始数据
int16_t g_accel[3]; // 加速度原始数据
float g_temp;       // 温度数据

/* Private functions --------------------------------------------------------*/
/**
 * @brief   在指定地址写一个字节到MUP6050中
 * @param   addr:要写入数据的地址 0-255
 * @param   data:要写入字节数据
 * @retval  状态：0-成功，非0-错误状态
 */
uint8_t mpu6050_write_byte(uint16_t addr, uint8_t data)
{
    uint8_t status;
    status = HAL_I2C_Mem_Write(&hi2c1, MPU6050_DEVICE_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);

    if (status != 0)
    {
        return status; // 返回错误状态
    }

    // AT24C02写入后需要等待内部写周期完成（典型值5ms）
    // 使用设备就绪检测等待写入完成
    uint32_t timeout = HAL_GetTick() + 10; // 10ms超时
    while (HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_DEVICE_ADDR, 1, 1) != HAL_OK)
    {
        if (HAL_GetTick() > timeout)
        {
            return 0xFF; // 返回超时错误
        }
    }

    return 0;
}

/**
 * @brief  从指定地址读取多个字节
 * @param  addr: 要读取数据的起始地址 0-255
 * @param  pbuf: 指向读取数据缓冲区的指针
 * @param  size: 要读取的字节数
 * @retval 状态：0-成功，非0-错误状态
 */
uint8_t mpu6050_read_bytes(uint8_t addr, uint8_t *pbuf, uint8_t size)
{
    uint8_t status;

    // AT24C02读取没有页限制，可以连续读取
    // 但要注意不要超出EEPROM地址范围（0-255）
    if ((uint16_t)(addr + size) > 256)
    {
        return 0xFD; // 地址超出范围
    }

    // 执行I2C内存读取
    status = HAL_I2C_Mem_Read(&hi2c1, MPU6050_DEVICE_ADDR, addr,
                              I2C_MEMADD_SIZE_8BIT, pbuf, size, 1000);

    if (status != 0)
    {
        return status; // 返回HAL错误状态
    }

    return 0; // 读取成功
}

/**
 * @brief   读取MPU6050的ID
 * @param
 * @retval  正常返回1，异常返回0
 */
uint8_t mpu6050_check_id(void)
{
    uint8_t device_id = 0;
    mpu6050_read_bytes(MPU6050_DEVICE_ID_REG, &device_id, 1);

    if (device_id != 0x68)
    {
        printf("检测不到MPU6050模块，请检查模块与开发板的接线\r\n");
        return 0;
    }
    else
    {
        printf("MPU6050 ID = %d\r\n", device_id);
        return 1;
    }
}

/**
 * @brief   初始化MPU6050芯片
 */
void mpu6050_init(void)
{
    // 在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
    HAL_Delay(100);
    mpu6050_write_byte(MPU6050_PWR_MGMT1_REG, 0x80); // 解除休眠状态并复位所有寄存器

    mpu6050_write_byte(MPU6050_GYRO_CFG_REG, 0x18);    // 陀螺仪量程 默认2000deg/s
    mpu6050_write_byte(MPU6050_ACCEL_CFG_REG, 0x00);   // 加速计量程 默认2g
    mpu6050_write_byte(MPU6050_INTBP_CFG_REG, 0x80);   // INT引脚低电平有效
    mpu6050_write_byte(MPU6050_CFG_REG, 0x02);         // 设置低通滤波器带宽,加速度带宽94Hz 陀螺仪带宽98Hz
    mpu6050_write_byte(MPU6050_SAMPLE_RATE_REG, 0x04); // 设置陀螺仪采样率200Hz，5分频

    mpu6050_write_byte(MPU6050_PWR_MGMT1_REG, 0x01); // 设置CLKSEL,PLL X轴为参考
    mpu6050_write_byte(MPU6050_PWR_MGMT2_REG, 0x00); // 加速度与陀螺仪都工作

    HAL_Delay(100); // 等待传感器稳定
}

/**
 * @brief   读取MPU6050三轴角加速度
 * @param   pbuf：读取的数据缓冲区指针
 */
void mpu6050_get_gyro(int16_t *pbuf)
{
    uint8_t buf[6];

    mpu6050_read_bytes(MPU6050_GYRO_OUT, buf, 6);

    pbuf[0] = (buf[0] << 8) | buf[1];
    pbuf[1] = (buf[2] << 8) | buf[3];
    pbuf[2] = (buf[4] << 8) | buf[5];
}

/**
 * @brief   读取MPU6050三轴加速度
 * @param   pbuf：读取的数据缓冲区指针
 */
void mpu6050_get_accel(int16_t *pbuf)
{
    uint8_t buf[6];

    mpu6050_read_bytes(MPU6050_ACC_OUT, buf, 6);

    pbuf[0] = (buf[0] << 8) | buf[1];
    pbuf[1] = (buf[2] << 8) | buf[3];
    pbuf[2] = (buf[4] << 8) | buf[5];
}

/**
 * @brief   读取MPU6050的温度数据，转化成摄氏度
 * @retval  传感器温度值
 */
void mpu6050_get_temp(float *temp)
{
    uint8_t buf[2];
    int16_t tmp;

    mpu6050_read_bytes(MPU6050_RA_TEMP_OUT, buf, 2); // 读取温度值

    tmp = (buf[0] << 8) | buf[1];

    *temp = 36.53f + tmp / 340.0f;
}
