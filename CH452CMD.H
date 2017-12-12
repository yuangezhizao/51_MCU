/* 常量定义 */

/* CH451和CH452的常用命令码 */
#define CH452_NOP		0x0000					// 空操作
#define CH452_RESET     0x0201					// 复位
#define CH452_LEVEL		0x0100					// 加载光柱值,需另加7位数据
#define CH452_CLR_BIT	0x0180					// 段位清0,需另加6位数据
#define CH452_SET_BIT	0x01C0					// 段位置1,需另加6位数据
#define CH452_SLEEP		0x0202					// 进入睡眠状态
#define CH452_LEFTMOV   0x0300		            // 设置移动方式-左移
#define CH452_LEFTCYC   0x0301		            // 设置移动方式-左循环
#define CH452_RIGHTMOV  0x0302		            // 设置移动方式-右移
#define CH452_RIGHTCYC  0x0303		            // 设置移动方式-右循环	
#define CH452_SELF_BCD	0x0380					// 自定义BCD码,需另加7位数据
#define CH452_SYSOFF    0x0400					// 关闭显示、关闭键盘
#define CH452_SYSON1    0x0401					// 开启显示
#define CH452_SYSON2    0x0403					// 开启显示、键盘
#define CH452_SYSON2W   0x0423					// 开启显示、键盘, 真正2线接口
#define CH452_NO_BCD    0x0500					// 设置默认显示方式,可另加3位扫描极限
#define CH452_BCD       0x0580					// 设置BCD译码方式,可另加3位扫描极限
#define CH452_TWINKLE   0x0600		            // 设置闪烁控制,需另加8位数据
#define CH452_GET_KEY	0x0700					// 获取按键,返回按键代码
#define CH452_DIG0      0x0800					// 数码管位0显示,需另加8位数据
#define CH452_DIG1      0x0900		            // 数码管位1显示,需另加8位数据
#define CH452_DIG2      0x0a00		            // 数码管位2显示,需另加8位数据
#define CH452_DIG3      0x0b00		            // 数码管位3显示,需另加8位数据
#define CH452_DIG4      0x0c00		            // 数码管位4显示,需另加8位数据
#define CH452_DIG5      0x0d00					// 数码管位5显示,需另加8位数据
#define CH452_DIG6      0x0e00					// 数码管位6显示,需另加8位数据
#define CH452_DIG7      0x0f00		            // 数码管位7显示,需另加8位数据

// BCD译码方式下的特殊字符
#define		CH452_BCD_SPACE		0x10
#define		CH452_BCD_PLUS		0x11
#define		CH452_BCD_MINUS		0x12
#define		CH452_BCD_EQU		0x13
#define		CH452_BCD_LEFT		0x14
#define		CH452_BCD_RIGHT		0x15
#define		CH452_BCD_UNDER		0x16
#define		CH452_BCD_CH_H		0x17
#define		CH452_BCD_CH_L		0x18
#define		CH452_BCD_CH_P		0x19
#define		CH452_BCD_DOT		0x1A
#define		CH452_BCD_SELF		0x1E
#define		CH452_BCD_TEST		0x88
#define		CH452_BCD_DOT_X		0x80



// 有效按键代码
#define		CH452_KEY_MIN		0x40
#define		CH452_KEY_MAX		0x7F
#define KEY_1 0x04;
#define KEY_2 0x05;
#define KEY_3 0x06;
#define KEY_4 0x07;
#define KEY_5 0x0c;
#define KEY_6 0x0d;
#define KEY_7 0x0e;
#define KEY_8 0x0f;

// 2线接口的CH452定义
#define		CH452_I2C_ADDR0		0x40			// CH452的ADDR=0时的地址
#define		CH452_I2C_ADDR1		0x60			// CH452的ADDR=1时的地址,默认值
#define		CH452_I2C_MASK		0x3E			// CH452的2线接口高字节命令掩码

// 对外子程序
extern	unsigned char CH452_Read(void);				// 从CH452读取按键代码
extern  void CH452_Write(unsigned short cmd);	// 向CH452发出操作命令


sbit CH452_DCLK=P2^4;
sbit CH452_DIN=P2^5;
sbit CH452_LOAD=P2^6;
sbit CH452_DOUT=P2^7;


int SEG[]={0xbd,0x18,0xd5,0xd9,0x78,0xe9,0xed,0x98,0xfd,0xf9};


int DIG[]={0x0800,0x0900,0x0a00,0x0b00,0x0c00,0x0d00,0x0e00,0x0f00};

//display(数码管编号左起4~7,数码管字符0~9) 板子型号不一样不一定对应
void display(int m, int n) {CH452_Write(DIG[m]+SEG[n]);} 

void display_all(int a){display(7,a%10);display(6,(a/=10)%10);display(5,(a/=10)%10);display(4,(a/10)%10);}



//写入指令CH452_Write(0x[1S2位指令代码的16进制])
void CH452_Write(unsigned short cmd)
{
	unsigned char i;
	CH452_LOAD=0;
	for(i=0;i<12;i++)//12次
	{
		CH452_DIN=cmd&1;//只看最后一位
		CH452_DCLK=0;
		CH452_DCLK=1;
		cmd>>=1;
	}
	CH452_LOAD=1;
}

/*

由于CH452只在LOAD的上升沿加载命令数据并处理，所以单片机不能将LOAD信号线用于其它用途，但是DIN和DCLK可以用于其它用途。
当单片机I/O引脚资源紧张时，CH452只需要独占使用LOAD信号线，DIN和DCLK信号线可以与其它接口电路共用。
单片机向CH452输出串行数据的过程是（不是唯一的过程，可以有很多变化）：
①输出一位数据，即向DIN输出最低位数据B0，并向DCLK输出低电平脉冲（从高电平变为低电平再恢复为高电平），其中包括一个上升沿使CH452输入位数据；
②以同样的方式，输出位数据B1～B11；
③向LOAD输出低电平脉冲，其中包括一个上升沿使CH452加载串行数据。

*/


//读取并返回键值
unsigned char CH452_Read(void)
{
	unsigned char i,keyw=0,cmd=0x07;
	//6.11.读取按键代码：0111xxxxxxxxB
//	EX0=0;	
	CH452_LOAD = 0;
	for(i=0;i<4;i++)
	{
		CH452_DIN=cmd&1;
		CH452_DCLK=0;
		CH452_DCLK=1;
		cmd>>=1;
	}
	CH452_LOAD=1;
	for(i=0;i<7;i++)
	{
		keyw<<=1;
		keyw|=CH452_DOUT;
		CH452_DCLK=0;
		CH452_DCLK=1;
	}
	//DOUT→keyw
//	EX0=1;
//	IE0=0;
	return keyw;
}

