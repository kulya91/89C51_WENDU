#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include"temp.h"	
#include"Usart.h"
 #include"1602.h"

typedef unsigned int u16;	  //���������ͽ�����������
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
	if(temp< 0)				//���¶�ֵΪ����
  	{
		DisplayData[0] = 0x40; 	  //   -
		//��Ϊ��ȡ���¶���ʵ���¶ȵĲ��룬���Լ�1����ȡ�����ԭ��
		temp=temp-1;
		temp=~temp;
		tp=temp;
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
 
  	}
 	else
  	{			
		DisplayData[0] = 0x00;
		tp=temp;//��Ϊ���ݴ�����С�������Խ��¶ȸ���һ�������ͱ���
		//����¶���������ô����ô������ԭ����ǲ���������
		temp=tp*0.0625*100+0.5;	
		//������С�����*100��+0.5���������룬��ΪC���Ը�����ת��Ϊ���͵�ʱ���С����
		//��������Զ�ȥ���������Ƿ����0.5����+0.5֮�����0.5�ľ��ǽ�1�ˣ�С��0.5�ľ�
		//�����0.5��������С������档
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
		datapros(Ds18b20ReadTemp()); //���ݴ�����
	}		
}

  void Usart() interrupt 1
{  u16 i;
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
	TL0=0X18;
	i++;
	if(i==1000)
	{
		i=0;
		SendData();
		xianshi();		
	}
}	