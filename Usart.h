unsigned char my[5];
 void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X21;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍

	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;

	TH0=0XFd;	//给定时器赋初值，定时1ms
	TL0=0Xfd;
	ET0=1;
	TR0=1;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}
void Timer0Init()
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1，仅用TR0打开启动。

	TH0=0XFC;	//给定时器赋初值，定时1ms
	TL0=0X18;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}

  void SendChar(unsigned char dat)
 {
	TI=0;	
	SBUF=dat;
	while(!TI);			 //等待发送数据完成
	TI=0;
 }
 void SendString(unsigned char dat[])
 {		
   unsigned int i;
	 unsigned int len;
	 len=sizeof(dat)*2;
   for(i=0;i<len;i++)
   {	   
   SendChar(dat[i]);
   }
}
void SendData()
{	
    unsigned int i;
    unsigned char a[]="温度:";		   
	   SendString(a);
	   for(i=0;i<5;i++)
	   {   if(i==3)
	   		 SendChar('.');
	     SendChar(my[i]+0x30);
    	}  SendChar('C');
	    SendChar('\r');
	    SendChar('\n');
	
 }		

			