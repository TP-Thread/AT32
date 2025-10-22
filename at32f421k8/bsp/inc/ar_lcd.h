#ifndef	DRV_LCD_H
#define DRV_LCD_H

#include "main.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

////SRAM 起始地址 存储空间 4 的起始地址
//#define Bank1_SRAM4_ADDR ((uint32_t)0x6C000000)
////SRAM 大小，1M 字节
//#define IS62WV51216_SIZE 0x100000
****************************************************************************************/
//FSMC_Bank1_NORSRAM 用于 LCD 命令操作的地址
#define FSMC_LCD_CMD 	((uint32_t)0x68000000)
//FSMC_Bank1_NORSRAM 用于 LCD 数据操作的地址
#define FSMC_LCD_DATA 	((uint32_t)0x68000002)

#define		LCD_DispWindow_X_Star	0     //起始点的X坐标
#define		LCD_DispWindow_Y_Star	0     //起始点的Y坐标

//根据液晶扫描方向而变化的XY像素宽度
//调用NT35510_GramScan函数设置方向时会自动更改
extern uint16_t LCD_X_LENGTH,LCD_Y_LENGTH; 

/******************************* 定义显示屏常用颜色 ********************************/
#define      WHITE		 		           0xFFFF	   //白色
#define      BLACK                         0x0000	   //黑色 
#define      GREY                          0xF7DE	   //灰色 
#define      BLUE                          0x001F	   //蓝色 
#define      BLUE2                         0x051F	   //浅蓝色 
#define      RED                           0xF800	   //红色 
#define      MAGENTA                       0xF81F	   //红紫色，洋红色 
#define      GREEN                         0x07E0	   //绿色 
#define      CYAN                          0x7FFF	   //蓝绿色，青色 
#define      YELLOW                        0xFFE0	   //黄色 
#define      BRED                          0xF81F
#define      GRED                          0xFFE0
#define      GBLUE                         0x07FF

/******************************* 芯片选择 ********************************/
#define NT35510

#if defined NT35510
	#define		LCD_LESS_PIXEL	  		480	  //液晶屏较短方向的像素宽度
	#define		LCD_MORE_PIXEL	 		800	  //液晶屏较长方向的像素宽度 

	#define      CMD_SetCoordinateX		 		0x2A00	     //设置X坐标
	#define      CMD_SetCoordinateY		 		0x2B00	     //设置Y坐标
	#define      CMD_SetPixel		 		    0x2C00	     //填充像素
#elif defined ILI9806G
	#define		LCD_LESS_PIXEL	  		480	  //液晶屏较短方向的像素宽度
	#define		LCD_MORE_PIXEL	 		854	  //液晶屏较长方向的像素宽度

	#define      CMD_SetCoordinateX		 		0x2A	     //设置X坐标
	#define      CMD_SetCoordinateY		 		0x2B	     //设置Y坐标
	#define      CMD_SetPixel		 		    0x2C	     //填充像素
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
