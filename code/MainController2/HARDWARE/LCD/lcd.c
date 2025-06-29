#include "lcd.h"


/*
* @brief 在指定区域填充颜色
* @param xsta,ysta 起始坐标
* @param xend,yend 终止坐标
* @param color 要填充的颜色
* @return None
*/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color){
	uint16_t i,j;
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	}
}


/*
* @brief 在指定区域填充颜色
* @param xsta,ysta 起始坐标
* @param xend,yend 终止坐标
* @param color 要填充的颜色
* @return None
*/
void LCD_DrawArea(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t *color_p)
{
    uint32_t size   = (xend - xsta + 1) * (yend - ysta + 1);

    LCD_Address_Set(xsta, ysta, xend, yend);  // 注意：xend/yend 不减1
		
		hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
		hspi1.Instance->CR1|= (1<<11);
		LCD_DC_Set();//写数据
		LCD_CS_Clr();
    HAL_SPI_Transmit(&hspi1, (uint8_t *)color_p, size, HAL_MAX_DELAY);
		LCD_CS_Set();	
		hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
		hspi1.Instance->CR1&=~(1<<11);
}


/*
* @brief 画点
* @param x,y 画点坐标
* @param color 颜色
* @return None
*/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//?è????±ê???? 
	LCD_WR_DATA(color);
} 


/*
* @brief 画线
* @param x1,y1 起始坐标
* @param x2,y2 终止坐标
* @param color 颜色
* @return None
*/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //????×?±ê???? 
	delta_y=y2-y1;
	uRow=x1;//????????×?±ê
	uCol=y1;
	if(delta_x>0)incx=1; //?è??????·??ò 
	else if (delta_x==0)incx=0;//???±?? 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//?????? 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //?????ù±?????×?±ê?á 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//????
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/*
* @brief 画矩形（无填充）
* @param x1,y1 起始坐标
* @param x2,y2 终止坐标
* @param color 颜色
* @return None
*/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/*
* @brief 画圆
* @param x0,y0 圆心坐标
* @param r 半径
* @param color 颜色
* @return None
*/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//??????????????·?????
		{
			b--;
		}
	}
}


/*
* @brief 显示单个字符
* @param x,y 显示坐标
* @param num 字符
* @param fc 字颜色
* @param bc 背景颜色
* @param sizey 字的大小 
* @param Mode 1-叠加 0-非叠加
* @return None
*/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//????×?·??ù??×????ó??
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //?????????ó????
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //?è????±ê???? 
	for(i=0;i<TypefaceNum;i++)
	{ 
		// if(sizey==12)temp=ascii_1206[num][i];		       //?÷??6x12×???
		// else if(sizey==16)temp=ascii_1608[num][i];		 //?÷??8x16×???
		// else if(sizey==24)temp=ascii_2412[num][i];		 //?÷??12x24×???
		// else if(sizey==32)temp=ascii_3216[num][i];		 //?÷??16x32×???
		if(sizey==12)temp= 0;		       //?÷??6x12×???
		else if(sizey==16)temp= 0;		 //?÷??8x16×???
		else if(sizey==24)temp= 0;		 //?÷??12x24×???
		else if(sizey==32)temp= 0;		 //?÷??16x32×???
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//·?????????
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//????????
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//????????
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/*
* @brief 显示字符串
* @param x,y 显示坐标
* @param p 字符串
* @param fc 字颜色
* @param bc 背景颜色
* @param sizey 字的大小 
* @param Mode 1-叠加 0-非叠加
* @return None
*/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{   
		if (*p<128)
		{
			LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
			x+=sizey/2;
		}
		p++;
	}  
}

/*
* @brief 显示字符串，满行
* @param x,y 显示坐标
* @param p 字符串
* @param fc 字颜色
* @param bc 背景颜色
* @param sizey 字的大小 
* @param Mode 1-叠加 0-非叠加
* @return None
*/
void LCD_ShowString_CL(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{   
		if (*p<128)
		{
			LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
			x+=sizey/2;
		}
		p++;
	}  
	while(x<240-sizey){
		LCD_ShowChar(x,y, 0x20,fc,bc,sizey,mode);
		x += sizey/2;
	}
}

/*
* @brief 显示数字
* @param m 底数
* @param n 指数
* @param num 字符

* @return None
*/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/*
* @brief 显示整数变量
* @param x,y 显示坐标
* @param num 整数变量
* @param len 显示的位数
* @param fc 字颜色
* @param bc 背景颜色
* @param sizey 字的大小 
* @return None
*/
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      ???????÷??????????????±???
      ??????????x,y????×?±ê
                num ??????????±???
                len ????????????
                fc ×???????
                bc ×???±??°??
                sizey ×???
      ·???????  ??
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      ???????÷??????????
      ??????????x,y????×?±ê
                length ?????¤??
                width  ?????í??
                pic[]  ??????×é    
      ·???????  ??
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}


