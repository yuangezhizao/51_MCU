#include <STC15F2K60S2.H>
#include <STDIO.H>
#include <intrins.h>
#include <CH452CMD.H>
#include "12864g-86-pc.h"

#define uchar unsigned char
#define uint unsigned int
uint count,num,flag;
uchar pressed=0, reset=0;
int key;
uchar beepflag=1;

void del(long x){for(;x>0;x--)_nop_();}

void beep(long d, uchar time)
{
	int i,j;
	if(beepflag)
	{
		for(j=0;j<time;j++)
		{
			for(i=0;i<(500/d);i++)
			//f↑d↑
			{
				P45=~P45;
				del(d);
			}
			del(5e4);
		}
	}
	P45=1;
}

void time() interrupt 1
{
	TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
	//重赋
  count++;
  if(count==20)
		{
			count=0;
			num--;
			if(num==0)
				{
					TR0=0;
				}
		}

}

void init()
{   
	initial_lcd();
	clear_screen();
	
	CH452_Write(CH452_RESET);
	CH452_Write(CH452_NO_BCD);
	CH452_Write(CH452_SYSON2);
	num=3;
	flag=0;
	
	TMOD=0x01;
	TH0=(65536-50000)/256;
  TL0=(65536-50000)%256;
  EA=1;
  ET0=1;
	TR0=0;
  //TR0=1;
	reset=0;
	pressed=0;

}

void main()
{
	init();
	display_GB2312_string(4,33,"开机自检");
	display_all(8888);
	
	del(5e4);
	beep(10,1);
	
	display_GB2312_string(4,33,"自检完成");
	del(1e5);	
	
	CH452_Write(CH452_RESET);
	CH452_Write(CH452_NO_BCD);
	CH452_Write(CH452_SYSON2);
	
	while(1)
	{
		display_GB2312_string(4,33,"准备开始" );
		while(1)
		{
			
			display(4,0);
			if (num==0 && pressed==0)
			{
				display_GB2312_string(4,33,"开始抢答" );
				
			}
			display(5,num);
			
			if(CH452_DOUT == 0)
			{
				key=CH452_Read();
				//display_all(key/10000);
				switch(key&0x3f)//00111111B
				{
					case 0x04: P45=~P45;break;
					case 0x05: 1==1;break;
					case 0x06: TR0=1;display_GB2312_string(4,33,"开始计时" );break;
					case 0x07: reset=1;init();display_GB2312_string(4,33,"准备开始" );break;
					
					case 0x0c: if(flag==0){if (num >0){pressed=1;display(7,1);TR0=0;flag=1;display_GB2312_string(4,33,"1 号犯规" );beep(10,1);} else{pressed=1;display(6,1);TR0=0;flag=1;display_GB2312_string(4,33,"1 号抢答" );beep(1,1);}}break;
					case 0x0d: if(flag==0){if (num >0){pressed=1;display(7,2);TR0=0;flag=1;display_GB2312_string(4,33,"2 号犯规" );beep(10,2);} else{pressed=1;display(6,2);TR0=0;flag=1;display_GB2312_string(4,33,"2 号抢答" );beep(1,2);}}break;
					case 0x0e: if(flag==0){if (num >0){pressed=1;display(7,3);TR0=0;flag=1;display_GB2312_string(4,33,"3 号犯规" );beep(10,3);} else{pressed=1;display(6,3);TR0=0;flag=1;display_GB2312_string(4,33,"3 号抢答" );beep(1,3);}}break;
					case 0x0f: if(flag==0){if (num >0){pressed=1;display(7,4);TR0=0;flag=1;display_GB2312_string(4,33,"4 号犯规" );beep(10,4);} else{pressed=1;display(6,4);TR0=0;flag=1;display_GB2312_string(4,33,"4 号抢答" );beep(1,4);}}break;
				}
				
			}
			if(reset) break;
		}
	}
}