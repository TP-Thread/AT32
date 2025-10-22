#ifndef BSP_MPU6050_DMP_H
#define BSP_MPU6050_DMP_H

#include "main.h"
#include <stdio.h>
#include <math.h>
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#define DEFAULT_MPU_HZ  (100)  	//���Ƶ��   
#define q30   1073741824.0f		//2��30�η�


void MPU6050_DMP_Init(void);    				//MPU6050 DMP��ʼ��
//void MPU6050_DMP_GetRawData(int16_t *data);  	//MPU6050 MDP��ȡԭʼ��������,��̬��Ԫ��
void MPU6050_DMP_GetData(int16_t *data);  		//MPU6050 MDP��ȡ�������ݣ���̬ŷ����

#endif
