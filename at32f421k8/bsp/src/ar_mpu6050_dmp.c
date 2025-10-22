/**			                                                                                 
  ******************************************************************************
  * @作  者  两点一线
  * @版  本  V1.0
  * @日  期  2023-1-23
  * @内  容  MPU6050 操作
  *
  ******************************************************************************
  * @说  明
  * 1.MPU6050 DMP姿态解算
  * 2.IIC通信采用硬件方式
  ******************************************************************************
  */
#include "bsp_mpu6050_dmp.h" 


//陀螺仪方向设置参数
signed char gyro_orientation[9] = { 1, 0, 0,
                                    0, 1, 0,
                                    0, 0, 1};

//函数inv_orientation_matrix_to_scalar()的辅助函数			   
static uint16_t inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;      // error
    return b;
}

/**
 * @brief       将方向矩阵转换为标量表示，以供DMP使用
 * @param       mtx: 方向矩阵
 * @retval      标量表示的方向参数
 */
uint16_t inv_orientation_matrix_to_scalar(const signed char *mtx)
{
    unsigned short scalar;
    /*
        XYZ  010_001_000 Identity Matrix
        XZY  001_010_000
        YXZ  010_000_001
        YZX  000_010_001
        ZXY  001_000_010
        ZYX  000_001_010
    */
    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;

    return scalar;
}

//mpu6050传感器自测试函数
void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
	
    if (result == 0x3) 
	{
        float sens;
        unsigned short accel_sens;
		
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
		
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
    }
}


/**
  * @简  述  MPU6050 DMP初始化
  * @参  数  无	  
  * @返回值  无
  */
void MPU6050_DMP_Init(void)
{
   uint8_t res=0;

	if(mpu_init() == 0)
	{	
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//设置所需要的传感器
		if(res) printf("mpu_set_sensor error\r\n");
		
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//设置FIFO
		if(res) printf("mpu_configure_fifo error\r\n"); 
		
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//设置采样率
		if(res) printf("mpu_set_sample_rate error\r\n");
		
		res=dmp_load_motion_driver_firmware();		//加载dmp固件
		if(res) printf("dmp_load_motion_driver_firmware error\r\n"); 
		
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//设置陀螺仪方向
		if(res) printf("dmp_set_orientation error\r\n");; 
		
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//设置dmp功能
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res) printf("dmp_enable_feature error\r\n");
		
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//设置DMP输出速率(最大不超过200Hz)
		if(res) printf("dmp_set_fifo_rate error.\r\n");
		
		run_self_test();  //校准
		
		res=mpu_set_dmp_state(1);  //使能DMP
		if(res) printf("mpu_set_dmp_state error.\r\n"); 
	}
}


/**
  * @简  述  MPU6050 DMP获取解算数据，姿态欧拉角	  
		data[0-2] 陀螺仪
		data[3-5] 加速度
		data[6-8] 欧拉角 横滚，俯仰，航向，实际角度扩大100倍
		pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
		roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
		yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
  */
void MPU6050_DMP_GetData(int16_t *data)
{
	static long quat[4];
	static unsigned long sensor_timestamp;
	static short sensors;
	static unsigned char more;
	
	static float pitch,roll,yaw; 
	static float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;

    //读取MPU6050 FIFO中数据的频率需与宏DEFAULT_MPU_HZ定义的频率一致，读取得太快或太慢都可能导致读取失败
    //读取太快：MPU6050还未采样，FIFO中无数据，读取失败  读取太慢：MPU6050的FIFO溢出，读取失败
	dmp_read_fifo(&data[0], &data[3], quat, &sensor_timestamp, &sensors, &more);
	
	if ( sensors & INV_WXYZ_QUAT )
	{
		//MPU6050的DMP输出的是姿态解算后的四元数，采用q30格式，即结果被放大了2的30次方倍，
		//因为四元数并不是角度信号，为了得到欧拉角，就需要对MPU6050的DMP输出结果进行转换
		q0=quat[0] / q30;
		q1=quat[1] / q30;
		q2=quat[2] / q30;
		q3=quat[3] / q30;		

		//计算俯仰角、横滚角、航向角
		//57.3为弧度转角度的转换系数，即180/PI
		pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 	
		roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
		yaw  = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;

		data[6] = roll*100;		
		data[7] = pitch*100;	
		data[8] = yaw*100;	
	}
}

