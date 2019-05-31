#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器
#include "string.h"
#include"temp.h"	
#include"Usart.h"
 #include"1602.h"

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;
u8 bai,shi,ge,fen,li;
u8 DisplayData[8];
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
	bai=temp / 10000;
shi=temp % 10000 / 1000;
	ge=temp % 1000 / 100;
	fen=temp % 100 / 10;
	li=temp % 10;
	 if((li>=0&&li<=9)&&(fen>=0&&fen<=9)&&(ge>=0&&ge<=9)&(shi>=1&&shi<=5)&(bai==0))
	 {
	ttldat[0]=bai; ttldat[1]=shi;ttldat[2]=ge;ttldat[3]=fen;ttldat[4]=li;
		}
}

 void xianshi()
{
	lcdcom(0xc5);
	lcddat(ttldat[0]+0x30);
	lcddat(	ttldat[1]+0x30);
	lcddat(ttldat[2]+0x30);
	lcddat('.');
	lcddat(ttldat[3]+0x30);
	lcddat(ttldat[4]+0x30);
	
}

void main()
{
	lcdinit("temperature:");
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
	if(i==100)
	{
		i=0;
		SendData();
		xianshi();		
	}
}	