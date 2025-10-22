/**			                                                                                 
  ******************************************************************************
  * @��  ��  ����һ��
  * @��  ��  V1.0
  * @��  ��  2023-1-23
  * @��  ��  MPU6050 ����
  *
  ******************************************************************************
  * @˵  ��
  * 1.MPU6050 DMP��̬����
  * 2.IICͨ�Ų���Ӳ����ʽ
  ******************************************************************************
  */
#include "bsp_mpu6050_dmp.h" 


//�����Ƿ������ò���
signed char gyro_orientation[9] = { 1, 0, 0,
                                    0, 1, 0,
                                    0, 0, 1};

//����inv_orientation_matrix_to_scalar()�ĸ�������			   
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
 * @brief       ���������ת��Ϊ������ʾ���Թ�DMPʹ��
 * @param       mtx: �������
 * @retval      ������ʾ�ķ������
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

//mpu6050�������Բ��Ժ���
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
  * @��  ��  MPU6050 DMP��ʼ��
  * @��  ��  ��	  
  * @����ֵ  ��
  */
void MPU6050_DMP_Init(void)
{
   uint8_t res=0;

	if(mpu_init() == 0)
	{	
		res=mpu_set_sensors(INV_XYZ_GYRO|INV_XYZ_ACCEL);//��������Ҫ�Ĵ�����
		if(res) printf("mpu_set_sensor error\r\n");
		
		res=mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL);//����FIFO
		if(res) printf("mpu_configure_fifo error\r\n"); 
		
		res=mpu_set_sample_rate(DEFAULT_MPU_HZ);	//���ò�����
		if(res) printf("mpu_set_sample_rate error\r\n");
		
		res=dmp_load_motion_driver_firmware();		//����dmp�̼�
		if(res) printf("dmp_load_motion_driver_firmware error\r\n"); 
		
		res=dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation));//���������Ƿ���
		if(res) printf("dmp_set_orientation error\r\n");; 
		
		res=dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT|DMP_FEATURE_TAP|	//����dmp����
		    DMP_FEATURE_ANDROID_ORIENT|DMP_FEATURE_SEND_RAW_ACCEL|DMP_FEATURE_SEND_CAL_GYRO|
		    DMP_FEATURE_GYRO_CAL);
		if(res) printf("dmp_enable_feature error\r\n");
		
		res=dmp_set_fifo_rate(DEFAULT_MPU_HZ);	//����DMP�������(��󲻳���200Hz)
		if(res) printf("dmp_set_fifo_rate error.\r\n");
		
		run_self_test();  //У׼
		
		res=mpu_set_dmp_state(1);  //ʹ��DMP
		if(res) printf("mpu_set_dmp_state error.\r\n"); 
	}
}


/**
  * @��  ��  MPU6050 DMP��ȡ�������ݣ���̬ŷ����	  
		data[0-2] ������
		data[3-5] ���ٶ�
		data[6-8] ŷ���� ���������������ʵ�ʽǶ�����100��
		pitch:������ ����:0.1��   ��Χ:-90.0�� <---> +90.0��
		roll:�����  ����:0.1��   ��Χ:-180.0��<---> +180.0��
		yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
  */
void MPU6050_DMP_GetData(int16_t *data)
{
	static long quat[4];
	static unsigned long sensor_timestamp;
	static short sensors;
	static unsigned char more;
	
	static float pitch,roll,yaw; 
	static float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;

    //��ȡMPU6050 FIFO�����ݵ�Ƶ�������DEFAULT_MPU_HZ�����Ƶ��һ�£���ȡ��̫���̫�������ܵ��¶�ȡʧ��
    //��ȡ̫�죺MPU6050��δ������FIFO�������ݣ���ȡʧ��  ��ȡ̫����MPU6050��FIFO�������ȡʧ��
	dmp_read_fifo(&data[0], &data[3], quat, &sensor_timestamp, &sensors, &more);
	
	if ( sensors & INV_WXYZ_QUAT )
	{
		//MPU6050��DMP���������̬��������Ԫ��������q30��ʽ����������Ŵ���2��30�η�����
		//��Ϊ��Ԫ�������ǽǶ��źţ�Ϊ�˵õ�ŷ���ǣ�����Ҫ��MPU6050��DMP����������ת��
		q0=quat[0] / q30;
		q1=quat[1] / q30;
		q2=quat[2] / q30;
		q3=quat[3] / q30;		

		//���㸩���ǡ�����ǡ������
		//57.3Ϊ����ת�Ƕȵ�ת��ϵ������180/PI
		pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 	
		roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; 
		yaw  = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;

		data[6] = roll*100;		
		data[7] = pitch*100;	
		data[8] = yaw*100;	
	}
}

