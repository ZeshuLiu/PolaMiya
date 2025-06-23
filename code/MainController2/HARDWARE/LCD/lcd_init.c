#include "lcd_init.h"

void delay(int t)
{
	while(t--);
}

/*  定义使用的参数 */
SPI_HandleTypeDef LCD_SPI;


/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	// uint8_t i;
	LCD_CS_Clr();
	/*
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
	*/
	while(HAL_SPI_GetState(&LCD_SPI) != HAL_SPI_STATE_READY);//?ì?é????±ê????
	HAL_SPI_Transmit(&LCD_SPI, &dat, 1, 10000);

  LCD_CS_Set();	
	
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set();//写数据
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	LCD_DC_Set();//写数据
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
	//LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+80);
		LCD_WR_DATA(y2+80);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置CMD
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
}

/*
* @brief 初始化led显示
* @param SPI_LCD 使用的硬件spi口
*/

void LCD_Init(SPI_HandleTypeDef SPI_LCD)
{
	//SPI1_Init();
	//LCD_GPIO_Init();//初始化GPIO
	//MX_SPI1_Init();
	LCD_SPI = SPI_LCD;
	
	LCD_RES_Clr();//复位
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(100);
	
	LCD_BLK_Set();//打开背光
  	HAL_Delay(100);
	
	//return;
	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11);                                             //无此指令，不能正常初始化芯片，无显示
	HAL_Delay(120); 
	LCD_WR_REG(0x11);                                             //! 执行两遍才比较稳定，否则有几率启动不起来
	HAL_Delay(120); 

	LCD_WR_REG(0x36);                                             //设置内存扫描方向，0X00正常扫描，从上往下，从左往右，RGB方式
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);
 
	LCD_WR_REG(0x3A);                                             //数据格式，65K色,565
	LCD_WR_DATA8(0x05);
 
	LCD_WR_REG(0xB2);                                             //帧频设置
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33); 
 
  	LCD_WR_REG(0xC6);                                               //正常模式下的帧速率控制
 	LCD_WR_DATA8(0x01);                                            //正常模式下：屏幕刷新率 111Hz
//   LCD_WR_DATA8(0x1F);                                            //正常模式下：屏幕刷新率 39Hz
  
	LCD_WR_REG(0xB7);                                             //GATE 设置
	LCD_WR_DATA8(0x35);  
 
	LCD_WR_REG(0xBB);                                             //VCOM设置 
	LCD_WR_DATA8(0x19);
 
	LCD_WR_REG(0xC0);                                             //LCM设置,默认0x2c
	LCD_WR_DATA8(0x2C);
 
	LCD_WR_REG(0xC2);                                             //VDV&VRH SET ,默认0x01
	LCD_WR_DATA8(0x01);
 
	LCD_WR_REG(0xC3);                                             //VRHS SET，默认0x0b
	LCD_WR_DATA8(0x12);                                           //此处根据实际情况修正
 
	LCD_WR_REG(0xC4);                                             //VDV SET，默认0x20
	LCD_WR_DATA8(0x20);  
 
	LCD_WR_REG(0xC6);                                             // FR SET, 默认0x0F
	LCD_WR_DATA8(0x0F);    
 
	LCD_WR_REG(0xD0);                                             //电源控制1
	LCD_WR_DATA8(0xA4);                                           //该参数不变  
	LCD_WR_DATA8(0xA1);                                           //此处根据实际情况修改
 
	LCD_WR_REG(0xE0);                                             //正极性GAMMA调整 
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);
 
	LCD_WR_REG(0xE1);                                              //负极性GAMMA调整
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);
 
	LCD_WR_REG(0x21);                                             //反显开，默认是0X20，正常模式
  
	LCD_WR_REG(0x29);                                             //显示开，等待MCU数传送
} 








