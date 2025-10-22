#ifndef	DRV_LCD_H
#define DRV_LCD_H

#include "main.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

////SRAM ��ʼ��ַ �洢�ռ� 4 ����ʼ��ַ
//#define Bank1_SRAM4_ADDR ((uint32_t)0x6C000000)
////SRAM ��С��1M �ֽ�
//#define IS62WV51216_SIZE 0x100000
****************************************************************************************/
//FSMC_Bank1_NORSRAM ���� LCD ��������ĵ�ַ
#define FSMC_LCD_CMD 	((uint32_t)0x68000000)
//FSMC_Bank1_NORSRAM ���� LCD ���ݲ����ĵ�ַ
#define FSMC_LCD_DATA 	((uint32_t)0x68000002)

#define		LCD_DispWindow_X_Star	0     //��ʼ���X����
#define		LCD_DispWindow_Y_Star	0     //��ʼ���Y����

//����Һ��ɨ�跽����仯��XY���ؿ��
//����NT35510_GramScan�������÷���ʱ���Զ�����
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

/******************************* ������ʾ��������ɫ ********************************/
#define      WHITE		 		           0xFFFF	   //��ɫ
#define      BLACK                         0x0000	   //��ɫ 
#define      GREY                          0xF7DE	   //��ɫ 
#define      BLUE                          0x001F	   //��ɫ 
#define      BLUE2                         0x051F	   //ǳ��ɫ 
#define      RED                           0xF800	   //��ɫ 
#define      MAGENTA                       0xF81F	   //����ɫ�����ɫ 
#define      GREEN                         0x07E0	   //��ɫ 
#define      CYAN                          0x7FFF	   //����ɫ����ɫ 
#define      YELLOW                        0xFFE0	   //��ɫ 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

/******************************* оƬѡ�� ********************************/
#define NT35510

#if defined NT35510
	#define		LCD_LESS_PIXEL	  		480	  //Һ�����϶̷�������ؿ��
	#define		LCD_MORE_PIXEL	 		800	  //Һ�����ϳ���������ؿ�� 

	#define      CMD_SetCoordinateX		 		0x2A00	     //����X����
	#define      CMD_SetCoordinateY		 		0x2B00	     //����Y����
	#define      CMD_SetPixel		 		    0x2C00	     //�������
#elif defined ILI9806G
	#define		LCD_LESS_PIXEL	  		480	  //Һ�����϶̷�������ؿ��
	#define		LCD_MORE_PIXEL	 		854	  //Һ�����ϳ���������ؿ��

	#define      CMD_SetCoordinateX		 		0x2A	     //����X����
	#define      CMD_SetCoordinateY		 		0x2B	     //����Y����
	#define      CMD_SetPixel		 		    0x2C	     //�������
#endif


void LCD_Write_Cmd(uint16_t cmd);
void LCD_Write_Data(uint16_t data);

void LCD_Init(void);
void LCD_Test(void);
void LCD_SetForegroundColor(uint16_t Color);
void LCD_SetBackgroundColor(uint16_t Color);
void LCD_Rst(void);
void LCD_BL(uint8_t state);
void LCD_REG_Config(void);
void LCD_GramScan(uint8_t ucOption);
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);
void LCD_SetCursor(uint16_t usX, uint16_t usY);
void LCD_Clear(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight);
void LCD_SetPointPixel(uint16_t usX, uint16_t usY);
void LCD_DrawLine(uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2);
void LCD_DrawRectangle(uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled);
void LCD_DrawCircle(uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled);



#endif
