#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "string.h"
#include"temp.h"	
#include"Usart.h"
 #include"1602.h"

typedef unsigned int u16;	  //���������ͽ�����������
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
		datapros(Ds18b20ReadTemp()); //���ݴ�����
	}		
}

  void Usart() interrupt 1
{  u16 i;
	TH0=0XFC;	//����ʱ������ֵ����ʱ1ms
	TL0=0X18;
	i++;
	if(i==100)
	{
		i=0;
		SendData();
		xianshi();		
	}
}	