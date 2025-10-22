#ifndef BSP_MPU6050_DMP_H
#define BSP_MPU6050_DMP_H

#include "main.h"
#include <stdio.h>
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#define DEFAULT_MPU_HZ  (100)  	//输出频率   
#define q30   1073741824.0f		//2的30次方


void MPU6050_DMP_Init(void);    				//MPU6050 DMP初始化
//void MPU6050_DMP_GetRawData(int16_t *data);  	//MPU6050 MDP获取原始解算数据,姿态四元数
void MPU6050_DMP_GetData(int16_t *data);  		//MPU6050 MDP获取解算数据，姿态欧拉角

#endif
