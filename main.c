#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include"temp.h"	
#include"Usart.h"
 #include"1602.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

u8 DisplayData[8];
u8 code smgduan[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
void xianshi();
void delay(u16 i)
{
	while(i--);	
}


void datapros(int temp) 	 
{
   	float tp;  
	if(temp< 0)				//当温度值为负数
  	{
		DisplayData[0] = 0x40; 	  //   -
		//因为读取的温度是实际温度的补码，所以减1，再取反求出原码
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//因为数据处理有小数点所以将温度赋给一个浮点型变量
		//如果温度是正的那么，那么正数的原码就是补码它本身
		temp=tp*0.0625*100+0.5;	
		//留两个小数点就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
		//后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
		//算加上0.5，还是在小数点后面。
	}
	DisplayData[1] = smgduan[temp / 10000];
	DisplayData[2] = smgduan[temp % 10000 / 1000];
	DisplayData[3] = smgduan[temp % 1000 / 100] | 0x80;
	DisplayData[4] = smgduan[temp % 100 / 10];
	DisplayData[5] = smgduan[temp % 10];
	my[0]=temp / 10000;
	my[1]=temp % 10000 / 1000;
	my[2]=temp % 1000 / 100;
	my[3]=temp % 100 / 10;
	my[4]=temp % 10;
}

 void xianshi()
{
	lcdcom(0xc5);
	lcddat(my[0]+0x30);
	lcddat(	my[1]+0x30);
	lcddat(my[2]+0x30);
	lcddat('.');
	lcddat(my[3]+0x30);
	lcddat(my[4]+0x30);
	
}

void main()
{
	lcdinit();
UsartInit();

	while(1)
	{
		datapros(Ds18b20ReadTemp()); //数据处理函数
	}		
}

  void Usart() interrupt 1
{  u16 i;
	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;
	i++;
	if(i==1000)
	{
		i=0;
		SendData();
		xianshi();		
	}
}	