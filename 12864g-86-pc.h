sbit lcd_sclk=P0^4;  //接口定义:lcd_sclk 就是 LCD 的 SCLK 
sbit lcd_sid=P0^3;   //接口定义:lcd_sid 就是 LCD 的 SDA 
sbit lcd_rs=P0^2;    //接口定义:lcd_rs 就是 LCD 的 RS,也叫“CD” 
sbit lcd_reset=P0^1; //接口定义:lcd_reset 就是 LCD 的 RESET 
sbit lcd_cs1=P0^0;   //接口定义:lcd_cs1 就是 LCD 的 CS1  
sbit Rom_IN=P2^0;    //字库 IC 接口定义:Rom_IN 就是字库 IC 的 SI 
sbit Rom_OUT=P2^1;   //字库 IC 接口定义:Rom_OUT 就是字库 IC 的 SO 
sbit Rom_SCK=P2^2;   //字库 IC 接口定义:Rom_SCK 就是字库 IC 的 SCK 
sbit Rom_CS=P2^3;    //字库 IC 接口定义 Rom_CS 就是字库 IC 的 CS#  
//sbit key=P2^0;  //定义一个按键

#define uchar unsigned char  
#define uint unsigned int 
#define ulong unsigned long 

uchar code bmp1[];  
uchar code jiong1[]={//--  文字:  囧  -- //--  宋体 12;  此字体下对应的点阵为：宽 x 高=16x16   --
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00,
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00};  
uchar code lei1[]={//--  文字:  畾  -- //--  宋体 12;  此字体下对应的点阵为：宽 x 高=16x16   --
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};   


//延时 
void delay(int n_ms)                
{
	int j,k;  
	for(j=0;j<n_ms;j++)
	for(k=0;k<110;k++); 
} 
//短延时 
void delay_us(int n_us)
{
	int j,k;
	for(j=0;j<n_us;j++)
	for(k=0;k<1;k++);
}

//写指令到 LCD 模块 
void transfer_command_lcd(int data1)    
{
	char i;
	lcd_cs1=0;
	lcd_rs=0;
	for(i=0;i<8;i++)
	{   
		lcd_sclk=0;
		//delay_us(10);  //加少量延时
		if(data1&0x80) lcd_sid=1;  
		else lcd_sid=0;   
		lcd_sclk=1;   
		//delay_us(10);  //加少量延时    
		data1=data1<<=1;
	}  
	lcd_cs1=1; 
} 

//写数据到 LCD 模块 
void transfer_data_lcd(int data1) 
{  
	char i; 
	lcd_cs1=0;
	lcd_rs=1;
	for(i=0;i<8;i++)
	{
		lcd_sclk=0;
		if(data1&0x80) lcd_sid=1;
		else lcd_sid=0;
		lcd_sclk=1;
		data1=data1<<=1;
	} 
	lcd_cs1=1;
} 

//LCD 模块初始化 
void initial_lcd() 
{
	lcd_reset=0;          //低电平复位 
	delay(100);  lcd_reset=1;       //复位完毕 
	delay(100);         
	transfer_command_lcd(0xe2);  //软复位
	delay(5);  
	transfer_command_lcd(0x2c);  //升压步聚 1  
	delay(50);   
	transfer_command_lcd(0x2e);  //升压步聚 2 
	delay(50);  
	transfer_command_lcd(0x2f);  //升压步聚 3
	delay(5);  
	transfer_command_lcd(0x24);  //粗调对比度，可设置范围 0x20～0x27  
	transfer_command_lcd(0x81);  //微调对比度  
	transfer_command_lcd(0x23);  //微调对比度的值，可设置范围 0x00～0x3f 
	transfer_command_lcd(0xa2);  //1/9 偏压比（bias）  
	transfer_command_lcd(0xc8);  //行扫描顺序：从上到下  
	transfer_command_lcd(0xa0);  //列扫描顺序：从左到右 
	transfer_command_lcd(0x40);  //起始行：第一行开始  
	transfer_command_lcd(0xaf);  //开显示	
}

void lcd_address(uint page,uint column) 
{  
	column=column-0x01; 
	transfer_command_lcd(0xb0+page-1); //设置页地址，每 8 行为一页，全屏共 64 行，被分成 8 页  
	transfer_command_lcd(0x10+(column>>4&0x0f)); //设置列地址的高 4 位  
	transfer_command_lcd(column&0x0f); //设置列地址的低 4 位 
 }  
//全屏清屏 
void clear_screen() 
{
	unsigned char i,j;  
	for(i=0;i<9;i++) 
	{  
		transfer_command_lcd(0xb0+i); 
		transfer_command_lcd(0x10);
		transfer_command_lcd(0x00); 
		for(j=0;j<132;j++)  
		{      
			transfer_data_lcd(0x00); 
		}  
	} 
}

//显示 128x64 点阵图像 
void display_128x64(uchar *dp) 
{
	uint i,j; 
	for(j=0;j<8;j++) 
	{  
		lcd_address(j+1,1); 
		for (i=0;i<128;i++) 
		{   
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1   
			dp++;   
		}  
	} 
}   
//显示 16x16 点阵图像、汉字、生僻字或 16x16 点阵的其他图标 
void display_graphic_16x16(uchar page,uchar column,uchar *dp)
 {
	 uint i,j;
	 for(j=0;j<2;j++)
	 {   
		lcd_address(page+j,column*16);
		for (i=0;i<16;i++)
		{  
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1  
			dp++;   
		} 
	} 
}   
//显示 8x16 点阵图像、ASCII, 或 8x16 点阵的自造字符、其他图标
 void display_graphic_8x16(uchar page,uchar column,uchar *dp)
 {
	 uint i,j;
	 for(j=0;j<2;j++)
	 {
		 lcd_address(page+j,column);
		 for (i=0;i<8;i++)
		{
			transfer_data_lcd(*dp);     //写数据到 LCD,每写完一个 8 位的数据后列地址自动加 1
			dp++;
		} 
	} 
}   
//显示 5X8 点阵图像、ASCII, 或 5x8 点阵的自造字符、其他图标 
void display_graphic_5x8(uchar page,uchar column,uchar *dp) 
{
	uint i;
	lcd_address(page,column);
	for (i=0;i<6;i++)  
	{
		transfer_data_lcd(*dp);
		dp++;  
	}   
}  
//送指令到晶联讯字库 IC 
void send_command_to_ROM( uchar datu ) 
{
	 uchar i;
	 for(i=0;i<8;i++ )  
	 {
		 Rom_SCK=0;
		 delay_us(10);
		 if(datu&0x80)Rom_IN = 1;
		 else Rom_IN = 0;
		 datu = datu<<1;
		 Rom_SCK=1;
		 delay_us(10);  
	} 
}  
//从晶联讯字库 IC 中取汉字或字符数据（1 个字节）
static uchar get_data_from_ROM( )
{
	uchar i;
	uchar ret_data=0;
	for(i=0;i<8;i++)  
	{
		Rom_OUT=1;
		Rom_SCK=0;   //delay_us(1);
		ret_data=ret_data<<1;
		if( Rom_OUT )    ret_data=ret_data+1;
		else    ret_data=ret_data+0;
		Rom_SCK=1;
		//delay_us(1);
	} 
	return(ret_data);
}   
//从指定地址读出数据写到液晶屏指定（page,column)座标中
 void get_and_write_16x16(ulong fontaddr,uchar page,uchar column) 
 {
	 uchar i,j,disp_data;
	 Rom_CS = 0;
	 send_command_to_ROM(0x03);
	 send_command_to_ROM((fontaddr&0xff0000)>>16);  //地址的高 8 位,共 24 位
	 send_command_to_ROM((fontaddr&0xff00)>>8);   //地址的中 8 位,共 24 位
	 send_command_to_ROM(fontaddr&0xff);     //地址的低 8 位,共 24 位  
	 for(j=0;j<2;j++)  
	 {
		lcd_address(page+j,column);
		for(i=0; i<16; i++ )      
		{
			 disp_data=get_data_from_ROM();
			 transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
		}
	}
	Rom_CS=1;
}  
//从指定地址读出数据写到液晶屏指定（page,column)座标中
 void get_and_write_8x16(ulong fontaddr,uchar page,uchar column) 
 {
	 uchar i,j,disp_data;
	 Rom_CS = 0;
	 send_command_to_ROM(0x03);
	 send_command_to_ROM((fontaddr&0xff0000)>>16);  //地址的高 8 位,共 24 位
	 send_command_to_ROM((fontaddr&0xff00)>>8);   //地址的中 8 位,共 24 位
	 send_command_to_ROM(fontaddr&0xff);     //地址的低 8 位,共 24 位  
	 for(j=0;j<2;j++)
	 {
		 lcd_address(page+j,column);
		 for(i=0; i<8; i++ )      
		 {
			 disp_data=get_data_from_ROM();
			 transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
		 }  
	 } 
	 Rom_CS=1; 
}  
//从指定地址读出数据写到液晶屏指定（page,column)座标中 
void get_and_write_5x8(ulong fontaddr,uchar page,uchar column)  
{
	uchar i,disp_data;
	Rom_CS = 0;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);  //地址的高 8 位,共 24 位
	send_command_to_ROM((fontaddr&0xff00)>>8);   //地址的中 8 位,共 24 位
	send_command_to_ROM(fontaddr&0xff);     //地址的低 8 位,共 24 位
	lcd_address(page,column);
	for(i=0; i<5; i++ )  
	{
		disp_data=get_data_from_ROM();
		transfer_data_lcd(disp_data); //写数据到 LCD,每写完 1 字节的数据后列地址自动加 1
	}
	Rom_CS=1;
}   
//****************************************************************   
ulong  fontaddr=0;
void display_GB2312_string(uchar page,uchar column,uchar *text) 
{
	uchar i= 0;
	while((text[i]>0x00))  
	{
		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))   
		{       //国标简体（GB2312）汉字在晶联讯字库 IC 中的地址由以下公式来计算：
				//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
				//由于担心 8 位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xb0)*94;
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);  
			get_and_write_16x16(fontaddr,page,column);  //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;   
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))   
		{       //国标简体（GB2312）15x16 点的字符在晶联讯字库 IC 中的地址由以下公式来计算：    
				//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0    
				//由于担心 8 位单片机有乘法溢出问题，所以分三部取地址
			fontaddr = (text[i]- 0xa1)*94;
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			get_and_write_16x16(fontaddr,page,column);  //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e)) 
	   {
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);
			get_and_write_8x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
			i+=1;
			column+=8;
		}
		else i++;
	}

}
void display_string_5x8(uchar page,uchar column,uchar *text)
{
	unsigned char i= 0;
	while((text[i]>0x00))
	{
		if((text[i]>=0x20) &&(text[i]<=0x7e))
		{
			 fontaddr = (text[i]- 0x20);
			 fontaddr = (unsigned long)(fontaddr*8);
			 fontaddr = (unsigned long)(fontaddr+0x3bfc0);
			 get_and_write_5x8(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
			 i+=1;
			 column+=6;
		}
		 else i++;
	}
}