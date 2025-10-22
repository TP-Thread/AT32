/**			                                                    
  ******************************************************************************
  * @作  者  两点一线
  * @版  本  V1.0
  * @日  期  2023-9-14
  * @内  容  LCD显示
  ******************************************************************************
  */
#include "ar_lcd.h"


//根据液晶扫描方向而变化的XY像素宽度
//调用NT35510_GramScan函数设置方向时会自动更改
uint16_t LCD_X_LENGTH = LCD_MORE_PIXEL;
uint16_t LCD_Y_LENGTH = LCD_LESS_PIXEL;

static uint16_t ForegroundColor = WHITE;//前景色
static uint16_t BackgroundColor = BLACK;//背景色


//液晶屏扫描模式，本变量主要用于方便选择触摸屏的计算参数,参数可选值为0-7
uint8_t LCD_SCAN_MODE =6;


/**
  * @brief  LCD写入命令
  * @param  cmd :要写入的命令（表寄存器地址）
  * @retval 无
  */	
void LCD_Write_Cmd(uint16_t cmd)
{
	*(__IO uint16_t *) (FSMC_LCD_CMD) = cmd;
}

/**
  * @brief  向NT35510写入数据
  * @param  data :要写入的数据
  * @retval 无
  */	
void LCD_Write_Data(uint16_t data)
{
	*(__IO uint16_t *) (FSMC_LCD_DATA) = data;
}


/**
  * @brief  从ILI9806G读取数据
  * @param  无
  * @retval 读取到的数据
  */	
uint16_t LCD_Read_Data ( void )
{
	return (* ( __IO uint16_t * )(FSMC_LCD_DATA));
}


/**
  * @brief  NT35510初始化函数，如果要用到lcd，一定要调用这个函数
  * @param  无
  * @retval 无
  */
void LCD_Init(void)
{	
	LCD_Rst ();
	LCD_REG_Config ();
	
	//设置默认扫描方向，其中 6 模式为大部分液晶例程的默认显示方向  
	LCD_GramScan(LCD_SCAN_MODE);
    
	LCD_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
	LCD_BL(1);//点亮LCD背光灯
}


/**
  * @brief  NT35510 软件复位
  * @param  无
  * @retval 无
  */
void LCD_Rst(void)
{			
	HAL_GPIO_WritePin( LCD_RST_GPIO_Port, LCD_RST_Pin ,GPIO_PIN_RESET);//低电平复位
	HAL_Delay(3);					   
	HAL_GPIO_WritePin( LCD_RST_GPIO_Port, LCD_RST_Pin ,GPIO_PIN_SET);
	HAL_Delay(3);	
}


/**
  * @brief  NT35510背光LED控制
  * @param  无
  * @retval 无
  */
void LCD_BL(uint8_t state)             
{
	if(state)
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);	
	else
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  初始化LCD寄存器
  * @param  无
  * @retval 无
  */
void LCD_REG_Config(void)
{
#if defined NT35510
///NT35510-HSD43
  //PAGE1
  LCD_Write_Cmd(0xF000);    LCD_Write_Data(0x0055);
  LCD_Write_Cmd(0xF001);    LCD_Write_Data(0x00AA);
  LCD_Write_Cmd(0xF002);    LCD_Write_Data(0x0052);
  LCD_Write_Cmd(0xF003);    LCD_Write_Data(0x0008);
  LCD_Write_Cmd(0xF004);    LCD_Write_Data(0x0001);

  //Set AVDD 5.2V
  LCD_Write_Cmd(0xB000);    LCD_Write_Data(0x000D);
  LCD_Write_Cmd(0xB001);    LCD_Write_Data(0x000D);
  LCD_Write_Cmd(0xB002);    LCD_Write_Data(0x000D);

  //Set AVEE 5.2V
  LCD_Write_Cmd(0xB100);    LCD_Write_Data(0x000D);
  LCD_Write_Cmd(0xB101);    LCD_Write_Data(0x000D);
  LCD_Write_Cmd(0xB102);    LCD_Write_Data(0x000D);

  //Set VCL -2.5V
  LCD_Write_Cmd(0xB200);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xB201);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xB202);    LCD_Write_Data(0x0000);				

  //Set AVDD Ratio
  LCD_Write_Cmd(0xB600);    LCD_Write_Data(0x0044);
  LCD_Write_Cmd(0xB601);    LCD_Write_Data(0x0044);
  LCD_Write_Cmd(0xB602);    LCD_Write_Data(0x0044);

  //Set AVEE Ratio
  LCD_Write_Cmd(0xB700);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB701);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB702);    LCD_Write_Data(0x0034);

  //Set VCL -2.5V
  LCD_Write_Cmd(0xB800);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB801);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB802);    LCD_Write_Data(0x0034);
        
  //Control VGH booster voltage rang
  LCD_Write_Cmd(0xBF00);    LCD_Write_Data(0x0001); //VGH:7~18V	

  //VGH=15V(1V/step)	Free pump
  LCD_Write_Cmd(0xB300);    LCD_Write_Data(0x000f);		//08
  LCD_Write_Cmd(0xB301);    LCD_Write_Data(0x000f);		//08
  LCD_Write_Cmd(0xB302);    LCD_Write_Data(0x000f);		//08

  //VGH Ratio
  LCD_Write_Cmd(0xB900);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB901);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xB902);    LCD_Write_Data(0x0034);

  //VGL_REG=-10(1V/step)
  LCD_Write_Cmd(0xB500);    LCD_Write_Data(0x0008);
  LCD_Write_Cmd(0xB501);    LCD_Write_Data(0x0008);
  LCD_Write_Cmd(0xB502);    LCD_Write_Data(0x0008);

  LCD_Write_Cmd(0xC200);    LCD_Write_Data(0x0003);

  //VGLX Ratio
  LCD_Write_Cmd(0xBA00);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xBA01);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xBA02);    LCD_Write_Data(0x0034);

    //VGMP/VGSP=4.5V/0V
  LCD_Write_Cmd(0xBC00);    LCD_Write_Data(0x0000);		//00
  LCD_Write_Cmd(0xBC01);    LCD_Write_Data(0x0078);		//C8 =5.5V/90=4.8V
  LCD_Write_Cmd(0xBC02);    LCD_Write_Data(0x0000);		//01

  //VGMN/VGSN=-4.5V/0V
  LCD_Write_Cmd(0xBD00);    LCD_Write_Data(0x0000); //00
  LCD_Write_Cmd(0xBD01);    LCD_Write_Data(0x0078); //90
  LCD_Write_Cmd(0xBD02);    LCD_Write_Data(0x0000);

  //Vcom=-1.4V(12.5mV/step)
  LCD_Write_Cmd(0xBE00);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xBE01);    LCD_Write_Data(0x0064); //HSD:64;Novatek:50=-1.0V, 80  5f

  //Gamma (R+)
  LCD_Write_Cmd(0xD100);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD101);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD102);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD103);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD104);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD105);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD106);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD107);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD108);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD109);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD10A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD10B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD10C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD10D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD10E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD10F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD110);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD111);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD112);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD113);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD114);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD115);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD116);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD117);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD118);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD119);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD11A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD11B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD11C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD11D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD11E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD11F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD120);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD121);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD122);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD123);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD124);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD125);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD126);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD127);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD128);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD129);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD12A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD12B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD12C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD12D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD12E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD12F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD130);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD131);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD132);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD133);    LCD_Write_Data(0x006D);

  //Gamma (G+)
  LCD_Write_Cmd(0xD200);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD201);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD202);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD203);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD204);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD205);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD206);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD207);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD208);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD209);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD20A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD20B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD20C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD20D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD20E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD20F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD210);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD211);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD212);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD213);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD214);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD215);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD216);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD217);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD218);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD219);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD21A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD21B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD21C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD21D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD21E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD21F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD220);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD221);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD222);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD223);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD224);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD225);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD226);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD227);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD228);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD229);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD22A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD22B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD22C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD22D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD22E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD22F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD230);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD231);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD232);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD233);    LCD_Write_Data(0x006D);

  //Gamma (B+)
  LCD_Write_Cmd(0xD300);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD301);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD302);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD303);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD304);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD305);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD306);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD307);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD308);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD309);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD30A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD30B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD30C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD30D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD30E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD30F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD310);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD311);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD312);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD313);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD314);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD315);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD316);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD317);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD318);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD319);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD31A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD31B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD31C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD31D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD31E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD31F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD320);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD321);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD322);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD323);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD324);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD325);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD326);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD327);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD328);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD329);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD32A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD32B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD32C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD32D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD32E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD32F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD330);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD331);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD332);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD333);    LCD_Write_Data(0x006D);

  //Gamma (R-)
  LCD_Write_Cmd(0xD400);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD401);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD402);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD403);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD404);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD405);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD406);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD407);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD408);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD409);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD40A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD40B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD40C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD40D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD40E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD40F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD410);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD411);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD412);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD413);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD414);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD415);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD416);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD417);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD418);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD419);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD41A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD41B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD41C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD41D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD41E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD41F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD420);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD421);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD422);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD423);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD424);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD425);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD426);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD427);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD428);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD429);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD42A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD42B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD42C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD42D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD42E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD42F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD430);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD431);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD432);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD433);    LCD_Write_Data(0x006D);

  //Gamma (G-)
  LCD_Write_Cmd(0xD500);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD501);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD502);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD503);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD504);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD505);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD506);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD507);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD508);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD509);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD50A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD50B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD50C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD50D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD50E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD50F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD510);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD511);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD512);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD513);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD514);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD515);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD516);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD517);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD518);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD519);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD51A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD51B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD51C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD51D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD51E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD51F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD520);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD521);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD522);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD523);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD524);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD525);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD526);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD527);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD528);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD529);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD52A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD52B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD52C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD52D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD52E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD52F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD530);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD531);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD532);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD533);    LCD_Write_Data(0x006D);

  //Gamma (B-)
  LCD_Write_Cmd(0xD600);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD601);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD602);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD603);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD604);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD605);    LCD_Write_Data(0x003A);
  LCD_Write_Cmd(0xD606);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD607);    LCD_Write_Data(0x004A);
  LCD_Write_Cmd(0xD608);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD609);    LCD_Write_Data(0x005C);
  LCD_Write_Cmd(0xD60A);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD60B);    LCD_Write_Data(0x0081);
  LCD_Write_Cmd(0xD60C);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD60D);    LCD_Write_Data(0x00A6);
  LCD_Write_Cmd(0xD60E);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD60F);    LCD_Write_Data(0x00E5);
  LCD_Write_Cmd(0xD610);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD611);    LCD_Write_Data(0x0013);
  LCD_Write_Cmd(0xD612);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD613);    LCD_Write_Data(0x0054);
  LCD_Write_Cmd(0xD614);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD615);    LCD_Write_Data(0x0082);
  LCD_Write_Cmd(0xD616);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD617);    LCD_Write_Data(0x00CA);
  LCD_Write_Cmd(0xD618);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD619);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0xD61A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD61B);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xD61C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD61D);    LCD_Write_Data(0x0034);
  LCD_Write_Cmd(0xD61E);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD61F);    LCD_Write_Data(0x0067);
  LCD_Write_Cmd(0xD620);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD621);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xD622);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD623);    LCD_Write_Data(0x00A4);
  LCD_Write_Cmd(0xD624);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD625);    LCD_Write_Data(0x00B7);
  LCD_Write_Cmd(0xD626);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD627);    LCD_Write_Data(0x00CF);
  LCD_Write_Cmd(0xD628);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD629);    LCD_Write_Data(0x00DE);
  LCD_Write_Cmd(0xD62A);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD62B);    LCD_Write_Data(0x00F2);
  LCD_Write_Cmd(0xD62C);    LCD_Write_Data(0x0002);
  LCD_Write_Cmd(0xD62D);    LCD_Write_Data(0x00FE);
  LCD_Write_Cmd(0xD62E);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD62F);    LCD_Write_Data(0x0010);
  LCD_Write_Cmd(0xD630);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD631);    LCD_Write_Data(0x0033);
  LCD_Write_Cmd(0xD632);    LCD_Write_Data(0x0003);
  LCD_Write_Cmd(0xD633);    LCD_Write_Data(0x006D);

  //PAGE0
  LCD_Write_Cmd(0xF000);    LCD_Write_Data(0x0055);
  LCD_Write_Cmd(0xF001);    LCD_Write_Data(0x00AA);
  LCD_Write_Cmd(0xF002);    LCD_Write_Data(0x0052);
  LCD_Write_Cmd(0xF003);    LCD_Write_Data(0x0008);	
  LCD_Write_Cmd(0xF004);    LCD_Write_Data(0x0000); 

  //480x800
  LCD_Write_Cmd(0xB500);    LCD_Write_Data(0x0050);

  //LCD_Write_Cmd(0x2C00);    LCD_Write_Data(0x0006); //8BIT 6-6-6?

  //Dispay control
  LCD_Write_Cmd(0xB100);    LCD_Write_Data(0x00CC);	
  LCD_Write_Cmd(0xB101);    LCD_Write_Data(0x0000); // S1->S1440:00;S1440->S1:02

  //Source hold time (Nova non-used)
  LCD_Write_Cmd(0xB600);    LCD_Write_Data(0x0005);

  //Gate EQ control	 (Nova non-used)
  LCD_Write_Cmd(0xB700);    LCD_Write_Data(0x0077);  //HSD:70;Nova:77	 
  LCD_Write_Cmd(0xB701);    LCD_Write_Data(0x0077);	//HSD:70;Nova:77

  //Source EQ control (Nova non-used)
  LCD_Write_Cmd(0xB800);    LCD_Write_Data(0x0001);  
  LCD_Write_Cmd(0xB801);    LCD_Write_Data(0x0003);	//HSD:05;Nova:07
  LCD_Write_Cmd(0xB802);    LCD_Write_Data(0x0003);	//HSD:05;Nova:07
  LCD_Write_Cmd(0xB803);    LCD_Write_Data(0x0003);	//HSD:05;Nova:07

  //Inversion mode: column
  LCD_Write_Cmd(0xBC00);    LCD_Write_Data(0x0002);	//00: column
  LCD_Write_Cmd(0xBC01);    LCD_Write_Data(0x0000);	//01:1dot
  LCD_Write_Cmd(0xBC02);    LCD_Write_Data(0x0000); 

  //Frame rate	(Nova non-used)
  LCD_Write_Cmd(0xBD00);    LCD_Write_Data(0x0001);
  LCD_Write_Cmd(0xBD01);    LCD_Write_Data(0x0084);
  LCD_Write_Cmd(0xBD02);    LCD_Write_Data(0x001c); //HSD:06;Nova:1C
  LCD_Write_Cmd(0xBD03);    LCD_Write_Data(0x001c); //HSD:04;Nova:1C
  LCD_Write_Cmd(0xBD04);    LCD_Write_Data(0x0000);

  //LGD timing control(4H/4-delay_ms)
  LCD_Write_Cmd(0xC900);    LCD_Write_Data(0x00D0);	//3H:0x50;4H:0xD0	 //D
  LCD_Write_Cmd(0xC901);    LCD_Write_Data(0x0002);  //HSD:05;Nova:02
  LCD_Write_Cmd(0xC902);    LCD_Write_Data(0x0050);	//HSD:05;Nova:50
  LCD_Write_Cmd(0xC903);    LCD_Write_Data(0x0050);	//HSD:05;Nova:50	;STV delay_ms time
  LCD_Write_Cmd(0xC904);    LCD_Write_Data(0x0050);	//HSD:05;Nova:50	;CLK delay_ms time

  LCD_Write_Cmd(0x3600);    LCD_Write_Data(0x0000);
  LCD_Write_Cmd(0x3500);    LCD_Write_Data(0x0000);

  LCD_Write_Cmd(0xFF00);    LCD_Write_Data(0x00AA);
  LCD_Write_Cmd(0xFF01);    LCD_Write_Data(0x0055);
  LCD_Write_Cmd(0xFF02);    LCD_Write_Data(0x0025);
  LCD_Write_Cmd(0xFF03);    LCD_Write_Data(0x0001);

  LCD_Write_Cmd(0xFC00);    LCD_Write_Data(0x0016);
  LCD_Write_Cmd(0xFC01);    LCD_Write_Data(0x00A2);
  LCD_Write_Cmd(0xFC02);    LCD_Write_Data(0x0026);
  LCD_Write_Cmd(0x3A00);    LCD_Write_Data(0x0006);

  LCD_Write_Cmd(0x3A00);    LCD_Write_Data(0x0055);
  //Sleep out
  LCD_Write_Cmd(0x1100);	   //?
  HAL_Delay(1);

  //Display on
  LCD_Write_Cmd(0x2900);
  
#elif defined ILI9806G
	HAL_Delay(1);
	LCD_Write_Cmd(0xFF);
	LCD_Write_Data(0xFF);
	LCD_Write_Data(0x98);
	LCD_Write_Data(0x06);
	HAL_Delay(1);
	LCD_Write_Cmd(0xBA);
	LCD_Write_Data(0x60);  
	HAL_Delay(1);
	LCD_Write_Cmd(0xBC);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0E);
	LCD_Write_Data(0x61);
	LCD_Write_Data(0xFF);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x1B);
	LCD_Write_Data(0x10);
	LCD_Write_Data(0x73);
	LCD_Write_Data(0x63);
	LCD_Write_Data(0xFF);
	LCD_Write_Data(0xFF);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xD5);
	LCD_Write_Data(0xD0);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x40); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xBD);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x23);
	LCD_Write_Data(0x45);
	LCD_Write_Data(0x67);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x23);
	LCD_Write_Data(0x45);
	LCD_Write_Data(0x67);  
	HAL_Delay(1);
	LCD_Write_Cmd(0xBE);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x2D);
	LCD_Write_Data(0xCB);
	LCD_Write_Data(0xA2);
	LCD_Write_Data(0x62);
	LCD_Write_Data(0xF2);
	LCD_Write_Data(0xE2);
	LCD_Write_Data(0x22);
	LCD_Write_Data(0x22);
	HAL_Delay(1);
	LCD_Write_Cmd(0xC7);
	LCD_Write_Data(0x63); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xED);
	LCD_Write_Data(0x7F);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x00);
	HAL_Delay(1);
	LCD_Write_Cmd(0xC0);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x0B);
	LCD_Write_Data(0x00);   
	HAL_Delay(1);
	LCD_Write_Cmd(0xFC);
	LCD_Write_Data(0x08); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xDF);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x20);
	HAL_Delay(1);
	LCD_Write_Cmd(0xF3);
	LCD_Write_Data(0x74);
	HAL_Delay(1);
	LCD_Write_Cmd(0xF9);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0xFD);
	LCD_Write_Data(0x80);
	LCD_Write_Data(0x80);
	LCD_Write_Data(0xC0);
	HAL_Delay(1);
	LCD_Write_Cmd(0xB4);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x02); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xF7);
	LCD_Write_Data(0x81);
	HAL_Delay(1);
	LCD_Write_Cmd(0xB1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x13); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xF2);
	LCD_Write_Data(0xC0);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x40);
	LCD_Write_Data(0x28);  
	HAL_Delay(1);
	LCD_Write_Cmd(0xC1);
	LCD_Write_Data(0x17);
	LCD_Write_Data(0x75);
	LCD_Write_Data(0x75);
	LCD_Write_Data(0x20); 
	HAL_Delay(1);
	LCD_Write_Cmd(0xE0);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x0C);
	LCD_Write_Data(0x0F);
	LCD_Write_Data(0x15);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x01);
	LCD_Write_Data(0x06);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x16);
	LCD_Write_Data(0x14);
	LCD_Write_Data(0x3E);
	LCD_Write_Data(0x3E);
	LCD_Write_Data(0x00);
	HAL_Delay(1);
	LCD_Write_Cmd(0xE1);
	LCD_Write_Data(0x00);
	LCD_Write_Data(0x09);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x12);
	LCD_Write_Data(0x13);
	LCD_Write_Data(0x1c);
	LCD_Write_Data(0x07);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x05);
	LCD_Write_Data(0x08);
	LCD_Write_Data(0x03);
	LCD_Write_Data(0x02);
	LCD_Write_Data(0x04);
	LCD_Write_Data(0x1E);
	LCD_Write_Data(0x1B);
	LCD_Write_Data(0x00);
	HAL_Delay(1);
	LCD_Write_Cmd(0x3A);
	LCD_Write_Data(0x55); 
	HAL_Delay(1);
	LCD_Write_Cmd(0x35);
	LCD_Write_Data(0x00); 
	HAL_Delay(1);
	LCD_Write_Cmd(0x11);
	HAL_Delay(1);
	LCD_Write_Cmd(0x29);	   
	HAL_Delay(1); 
#endif
}


/**
 * @brief  设置的GRAM的扫描方向 
 * @param  ucOption ：选择GRAM的扫描方向 
 *     @arg 0-7 :参数可选值为0-7这八个方向
 *
 *	！！！其中0、3、5、6 模式适合从左至右显示文字，
 *				不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 *		
 *	其中0、2、4、6 模式的X方向像素为480，Y方向像素为854
 *	其中1、3、5、7 模式下X方向像素为854，Y方向像素为480
 *
 *	其中 6 模式为大部分液晶例程的默认显示方向
 *	其中 3 模式为摄像头例程使用的方向
 *	其中 0 模式为BMP图片显示例程使用的方向
 *
 * @retval 无
 * @note  坐标图例：A表示向上，V表示向下，<表示向左，>表示向右
					X表示X轴，Y表示Y轴

------------------------------------------------------------
模式0：				.		模式1：		.	模式2：			.	模式3：					
					A		.					A		.		A					.		A									
					|		.					|		.		|					.		|							
					Y		.					X		.		Y					.		X					
					0		.					1		.		2					.		3					
	<--- X0 o		.	<----Y1	o		.		o 2X--->  .		o 3Y--->	
------------------------------------------------------------	
模式4：				.	模式5：			.	模式6：			.	模式7：					
	<--- X4 o		.	<--- Y5 o		.		o 6X--->  .		o 7Y--->	
					4		.					5		.		6					.		7	
					Y		.					X		.		Y					.		X						
					|		.					|		.		|					.		|							
					V		.					V		.		V					.		V		
---------------------------------------------------------				
											 LCD屏示例
								|-----------------|
								|			野火Logo		|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|									|
								|-----------------|
								屏幕正面（宽480，高854）

 *******************************************************/
void LCD_GramScan(uint8_t ucOption)
{	
	//参数检查，只可输入0-7
	if(ucOption >7)
		return;
	
	//根据模式更新LCD_SCAN_MODE的值，主要用于触摸屏选择计算参数
	LCD_SCAN_MODE = ucOption;
	
	//根据模式更新XY方向的像素宽度
	if(ucOption%2 == 0)	
	{
		//0 2 4 6模式下X方向像素宽度为480，Y方向为854
		LCD_X_LENGTH = LCD_LESS_PIXEL;
		LCD_Y_LENGTH = LCD_MORE_PIXEL;
	}
	else				
	{
		//1 3 5 7模式下X方向像素宽度为854，Y方向为480
		LCD_X_LENGTH = LCD_MORE_PIXEL;
		LCD_Y_LENGTH = LCD_LESS_PIXEL; 
	}
	
#if defined NT35510
	//0x36命令参数的高3位可用于设置GRAM扫描方向	
	LCD_Write_Cmd ( 0x3600 ); 
	LCD_Write_Data (0x00 | (ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式
	LCD_Write_Cmd ( CMD_SetCoordinateX ); 
	LCD_Write_Data ( 0x00 );		/* x 起始坐标高8位 */
    LCD_Write_Cmd ( CMD_SetCoordinateX + 1 ); 
	LCD_Write_Data ( 0x00 );		/* x 起始坐标低8位 */
	LCD_Write_Cmd ( CMD_SetCoordinateX + 2 ); 
	LCD_Write_Data ( ((LCD_X_LENGTH-1)>>8)&0xFF ); /* x 结束坐标高8位 */	
	LCD_Write_Cmd ( CMD_SetCoordinateX + 3 ); 
	LCD_Write_Data ( (LCD_X_LENGTH-1)&0xFF );				/* x 结束坐标低8位 */

	LCD_Write_Cmd ( CMD_SetCoordinateY ); 
	LCD_Write_Data ( 0x00 );		/* y 起始坐标高8位 */
	LCD_Write_Cmd ( CMD_SetCoordinateY + 1 ); 
	LCD_Write_Data ( 0x00 );		/* y 起始坐标低8位 */
	LCD_Write_Cmd ( CMD_SetCoordinateY + 2 ); 
	LCD_Write_Data ( ((LCD_Y_LENGTH-1)>>8)&0xFF );	/* y 结束坐标高8位 */	 
	LCD_Write_Cmd ( CMD_SetCoordinateY + 3 ); 
	LCD_Write_Data ( (LCD_Y_LENGTH-1)&0xFF );				/* y 结束坐标低8位 */

	/* write gram start */
	LCD_Write_Cmd ( CMD_SetPixel );	
#elif defined ILI9806G	
	//0x36命令参数的高3位可用于设置GRAM扫描方向	
	LCD_Write_Cmd(0x36); 
	LCD_Write_Data(0x00 | (ucOption<<5));//根据ucOption的值设置LCD参数，共0-7种模式
	LCD_Write_Cmd(CMD_SetCoordinateX ); 
	LCD_Write_Data(0x00);		/* x 起始坐标高8位 */
	LCD_Write_Data(0x00);		/* x 起始坐标低8位 */
	LCD_Write_Data(((LCD_X_LENGTH-1)>>8)&0xFF); /* x 结束坐标高8位 */	
	LCD_Write_Data((LCD_X_LENGTH-1)&0xFF);				/* x 结束坐标低8位 */

	LCD_Write_Cmd(CMD_SetCoordinateY); 
	LCD_Write_Data(0x00);		/* y 起始坐标高8位 */
	LCD_Write_Data(0x00);		/* y 起始坐标低8位 */
	LCD_Write_Data(((LCD_Y_LENGTH-1)>>8)&0xFF);	/* y 结束坐标高8位 */	 
	LCD_Write_Data((LCD_Y_LENGTH-1)&0xFF);				/* y 结束坐标低8位 */

	/* write gram start */
	LCD_Write_Cmd(CMD_SetPixel);
#endif
}


/**
  * @brief  在显示器上开辟一个窗口
  * @param  usX ：在特定扫描方向下窗口的起点X坐标
  * @param  usY ：在特定扫描方向下窗口的起点Y坐标
  * @param  usWidth ：窗口的宽度
  * @param  usHeight ：窗口的高度
  * @retval 无
  */
void LCD_OpenWindow(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{	
#if defined NT35510
	//设置列x地址范围，先高8位，然后低8位，先设置起始点再结束点
	LCD_Write_Cmd(CMD_SetCoordinateX);			
	LCD_Write_Data(usX >> 8);	 				
	LCD_Write_Cmd(CMD_SetCoordinateX + 1); 
	LCD_Write_Data(usX & 0xff);	 				
	LCD_Write_Cmd(CMD_SetCoordinateX + 2);
	LCD_Write_Data((usX + usWidth - 1 ) >> 8);
	LCD_Write_Cmd(CMD_SetCoordinateX + 3);
	LCD_Write_Data((usX + usWidth - 1 ) & 0xff);

	//设置行y地址范围
	LCD_Write_Cmd(CMD_SetCoordinateY); 		
	LCD_Write_Data(usY >> 8);
	LCD_Write_Cmd(CMD_SetCoordinateY + 1);
	LCD_Write_Data(usY & 0xff);
	LCD_Write_Cmd(CMD_SetCoordinateY + 2);
	LCD_Write_Data((usY + usHeight - 1 ) >> 8);
	LCD_Write_Cmd(CMD_SetCoordinateY + 3);
	LCD_Write_Data((usY + usHeight - 1) & 0xff);
#elif defined ILI9806G
	LCD_Write_Cmd(CMD_SetCoordinateX); 				 /* 设置X坐标 */
	LCD_Write_Data(usX >> 8);	 /* 先高8位，然后低8位 */
	LCD_Write_Data(usX & 0xff);	 /* 设置起始点和结束点*/
	LCD_Write_Data((usX + usWidth - 1 ) >> 8);
	LCD_Write_Data((usX + usWidth - 1 ) & 0xff);

	LCD_Write_Cmd(CMD_SetCoordinateY); 			     /* 设置Y坐标*/
	LCD_Write_Data(usY >> 8);
	LCD_Write_Data(usY & 0xff);
	LCD_Write_Data(( usY + usHeight - 1 ) >> 8);
	LCD_Write_Data(( usY + usHeight - 1) & 0xff);
#endif
}


/**
  * @brief  设定NT35510的光标坐标
  * @param  usX ：在特定扫描方向下光标的X坐标
  * @param  usY ：在特定扫描方向下光标的Y坐标
  * @retval 无
  */
void LCD_SetCursor(uint16_t usX, uint16_t usY)	
{
	LCD_OpenWindow(usX, usY, 1, 1);
}


/**
  * @brief  设置LCD的前景(字体)颜色,RGB565
  * @param  Color: 指定前景(字体)颜色 
  * @retval None
  */
void LCD_SetForegroundColor(uint16_t Color)
{
	ForegroundColor = Color;
}

/**
  * @brief  设置LCD的背景颜色,RGB565
  * @param  Color: 指定背景颜色 
  * @retval None
  */
void LCD_SetBackgroundColor(uint16_t Color)
{
  BackgroundColor = Color;
}


/**
  * @brief  在NT35510显示器上以某一颜色填充像素点
  * @param  ulAmout_Point ：要填充颜色的像素点的总数目
  * @param  usColor ：颜色
  * @retval 无
  */
static __inline void LCD_FillColor(uint32_t ulAmout_Point, uint16_t usColor)
{
	uint32_t i = 0;
	
	/* memory write */
	LCD_Write_Cmd(CMD_SetPixel);	
		
	for(i = 0; i < ulAmout_Point; i ++)
		LCD_Write_Data(usColor);
}


/**
  * @brief  对显示器的某一窗口以某种颜色进行清屏
  * @param  usX ：在特定扫描方向下窗口的起点X坐标
  * @param  usY ：在特定扫描方向下窗口的起点Y坐标
  * @param  usWidth ：窗口的宽度
  * @param  usHeight ：窗口的高度
  * @note   可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_Clear(uint16_t usX, uint16_t usY, uint16_t usWidth, uint16_t usHeight)
{
	LCD_OpenWindow(usX, usY, usWidth, usHeight);
	LCD_FillColor(usWidth * usHeight, BackgroundColor);		
}


/**
  * @brief  对显示器的某一点以某种颜色进行填充
  * @param  usX ：在特定扫描方向下该点的X坐标
  * @param  usY ：在特定扫描方向下该点的Y坐标
  * @note   可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_SetPointPixel(uint16_t usX, uint16_t usY)	
{	
	if((usX < LCD_X_LENGTH) && (usY < LCD_Y_LENGTH))
	{
		LCD_SetCursor(usX, usY);
		LCD_FillColor(1, ForegroundColor);
	}
}


/**
 * @brief  读取ILI9806G GRAN 的一个像素数据
 * @param  无
 * @retval 像素数据
 */
uint16_t LCD_Read_PixelData ( void )	
{	
	uint16_t usR=0, usG=0, usB=0 ;

	LCD_Write_Cmd ( 0x2E );   /* 读数据 */
	
	usR = LCD_Read_Data (); 	/*FIRST READ OUT DUMMY DATA*/
	
	usR = LCD_Read_Data ();  	/*READ OUT RED DATA  */
	usB = LCD_Read_Data ();  	/*READ OUT BLUE DATA*/
	usG = LCD_Read_Data ();  	/*READ OUT GREEN DATA*/	
	
	return ( ( ( usR >> 11 ) << 11 ) | ( ( usG >> 10 ) << 5 ) | ( usB >> 11 ) );
}


/**
 * @brief  获取显示器上某一个坐标点的像素数据
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @retval 像素数据
 */
uint16_t LCD_GetPointPixel ( uint16_t usX, uint16_t usY )
{ 
	uint16_t usPixelData;

	LCD_SetCursor ( usX, usY );
	
	usPixelData = LCD_Read_PixelData ();
	
	return usPixelData;	
}


/**
  * @brief  在显示器上使用 Bresenham 算法画线段 
  * @param  usX1 ：在特定扫描方向下线段的一个端点X坐标
  * @param  usY1 ：在特定扫描方向下线段的一个端点Y坐标
  * @param  usX2 ：在特定扫描方向下线段的另一个端点X坐标
  * @param  usY2 ：在特定扫描方向下线段的另一个端点Y坐标
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_DrawLine(uint16_t usX1, uint16_t usY1, uint16_t usX2, uint16_t usY2)
{
	uint16_t us; 
	uint16_t usX_Current, usY_Current;
	
	int32_t lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance; 
	int32_t lIncrease_X, lIncrease_Y; 	
	
	lDelta_X = usX2 - usX1; //计算坐标增量 
	lDelta_Y = usY2 - usY1; 
	
	usX_Current = usX1; 
	usY_Current = usY1; 
	
	if(lDelta_X > 0) 
		lIncrease_X = 1; //设置单步方向 
	else if(lDelta_X == 0) 
		lIncrease_X = 0;//垂直线 
	else 
	{ 
		lIncrease_X = -1;
		lDelta_X = - lDelta_X;
	} 
	
	if(lDelta_Y > 0)
		lIncrease_Y = 1; 
	else if(lDelta_Y == 0)
		lIncrease_Y = 0;//水平线 
	else 
	{
		lIncrease_Y = -1;
		lDelta_Y = - lDelta_Y;
	} 

	if(lDelta_X > lDelta_Y)
		lDistance = lDelta_X; //选取基本增量坐标轴 
	else 
		lDistance = lDelta_Y; 

	for(us = 0; us <= lDistance + 1; us ++)//画线输出 
	{  
		LCD_SetPointPixel(usX_Current, usY_Current);//画点 
		
		lError_X += lDelta_X ; 
		lError_Y += lDelta_Y ; 
		
		if(lError_X > lDistance) 
		{ 
			lError_X -= lDistance; 
			usX_Current += lIncrease_X; 
		}  
		
		if(lError_Y > lDistance) 
		{ 
			lError_Y -= lDistance; 
			usY_Current += lIncrease_Y; 
		} 		
	}  	
}   
 

/**
  * @brief  在显示器上画一个矩形
  * @param  usX_Start ：在特定扫描方向下矩形的起始点X坐标
  * @param  usY_Start ：在特定扫描方向下矩形的起始点Y坐标
  * @param  usWidth：矩形的宽度（单位：像素）
  * @param  usHeight：矩形的高度（单位：像素）
  * @param  ucFilled ：选择是否填充该矩形
  *   		该参数为以下值之一：
  *     @arg 0 :空心矩形
  *     @arg 1 :实心矩形 
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_DrawRectangle(uint16_t usX_Start, uint16_t usY_Start, uint16_t usWidth, uint16_t usHeight, uint8_t ucFilled)
{
	if(ucFilled)
	{
		LCD_OpenWindow(usX_Start, usY_Start, usWidth, usHeight);
		LCD_FillColor(usWidth * usHeight ,ForegroundColor);	
	}
	else
	{
		LCD_DrawLine(usX_Start, usY_Start, usX_Start + usWidth - 1, usY_Start);
		LCD_DrawLine(usX_Start, usY_Start + usHeight - 1, usX_Start + usWidth - 1, usY_Start + usHeight - 1);
		LCD_DrawLine(usX_Start, usY_Start, usX_Start, usY_Start + usHeight - 1);
		LCD_DrawLine(usX_Start + usWidth - 1, usY_Start, usX_Start + usWidth - 1, usY_Start + usHeight - 1);		
	}
}


/**
  * @brief  在 ILI9806G 显示器上使用 Bresenham 算法画圆
  * @param  usX_Center ：在特定扫描方向下圆心的X坐标
  * @param  usY_Center ：在特定扫描方向下圆心的Y坐标
  * @param  usRadius：圆的半径（单位：像素）
  * @param  ucFilled ：选择是否填充该圆,该参数为以下值之一：
  *     @arg 0 :空心圆
  *     @arg 1 :实心圆
  * @note 可使用LCD_SetBackColor、LCD_SetTextColor、LCD_SetColors函数设置颜色
  * @retval 无
  */
void LCD_DrawCircle(uint16_t usX_Center, uint16_t usY_Center, uint16_t usRadius, uint8_t ucFilled)
{
	int16_t sCurrentX, sCurrentY;
	int16_t sError;
	
	sCurrentX = 0; 
	sCurrentY = usRadius;	  
	
	sError = 3 - ( usRadius << 1 );     //判断下个点位置的标志
	
	while ( sCurrentX <= sCurrentY )
	{
		int16_t sCountY;
		
		if ( ucFilled ) 			
			for ( sCountY = sCurrentX; sCountY <= sCurrentY; sCountY ++ ) 
			{                      
				LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCountY );           //1，研究对象 
				LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCountY );           //2       
				LCD_SetPointPixel ( usX_Center - sCountY,   usY_Center + sCurrentX );           //3
				LCD_SetPointPixel ( usX_Center - sCountY,   usY_Center - sCurrentX );           //4
				LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCountY );           //5    
				LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCountY );           //6
				LCD_SetPointPixel ( usX_Center + sCountY,   usY_Center - sCurrentX );           //7 	
				LCD_SetPointPixel ( usX_Center + sCountY,   usY_Center + sCurrentX );           //0				
			}
		else
		{          
			LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center + sCurrentY );             //1，研究对象
			LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center + sCurrentY );             //2      
			LCD_SetPointPixel ( usX_Center - sCurrentY, usY_Center + sCurrentX );             //3
			LCD_SetPointPixel ( usX_Center - sCurrentY, usY_Center - sCurrentX );             //4
			LCD_SetPointPixel ( usX_Center - sCurrentX, usY_Center - sCurrentY );             //5       
			LCD_SetPointPixel ( usX_Center + sCurrentX, usY_Center - sCurrentY );             //6
			LCD_SetPointPixel ( usX_Center + sCurrentY, usY_Center - sCurrentX );             //7 
			LCD_SetPointPixel ( usX_Center + sCurrentY, usY_Center + sCurrentX );             //0
		}				
		sCurrentX ++;
		
		if ( sError < 0 ) 
			sError += 4 * sCurrentX + 6;	  	
		else
		{
			sError += 10 + 4 * ( sCurrentX - sCurrentY );   
			sCurrentY --;
		} 	
	}	
}




