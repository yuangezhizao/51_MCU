sbit lcd_sclk=P0^4;  //�ӿڶ���:lcd_sclk ���� LCD �� SCLK 
sbit lcd_sid=P0^3;   //�ӿڶ���:lcd_sid ���� LCD �� SDA 
sbit lcd_rs=P0^2;    //�ӿڶ���:lcd_rs ���� LCD �� RS,Ҳ�С�CD�� 
sbit lcd_reset=P0^1; //�ӿڶ���:lcd_reset ���� LCD �� RESET 
sbit lcd_cs1=P0^0;   //�ӿڶ���:lcd_cs1 ���� LCD �� CS1  
sbit Rom_IN=P2^0;    //�ֿ� IC �ӿڶ���:Rom_IN �����ֿ� IC �� SI 
sbit Rom_OUT=P2^1;   //�ֿ� IC �ӿڶ���:Rom_OUT �����ֿ� IC �� SO 
sbit Rom_SCK=P2^2;   //�ֿ� IC �ӿڶ���:Rom_SCK �����ֿ� IC �� SCK 
sbit Rom_CS=P2^3;    //�ֿ� IC �ӿڶ��� Rom_CS �����ֿ� IC �� CS#  
//sbit key=P2^0;  //����һ������

#define uchar unsigned char  
#define uint unsigned int 
#define ulong unsigned long 

uchar code bmp1[];  
uchar code jiong1[]={//--  ����:  ��  -- //--  ���� 12;  �������¶�Ӧ�ĵ���Ϊ���� x ��=16x16   --
0x00,0xFE,0x82,0x42,0xA2,0x9E,0x8A,0x82,0x86,0x8A,0xB2,0x62,0x02,0xFE,0x00,0x00,
0x00,0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x7F,0x40,0x40,0x7F,0x00,0x00};  
uchar code lei1[]={//--  ����:  ��  -- //--  ���� 12;  �������¶�Ӧ�ĵ���Ϊ���� x ��=16x16   --
0x80,0x80,0x80,0xBF,0xA5,0xA5,0xA5,0x3F,0xA5,0xA5,0xA5,0xBF,0x80,0x80,0x80,0x00,
0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00,0x7F,0x24,0x24,0x3F,0x24,0x24,0x7F,0x00};   


//��ʱ 
void delay(int n_ms)                
{
	int j,k;  
	for(j=0;j<n_ms;j++)
	for(k=0;k<110;k++); 
} 
//����ʱ 
void delay_us(int n_us)
{
	int j,k;
	for(j=0;j<n_us;j++)
	for(k=0;k<1;k++);
}

//дָ� LCD ģ�� 
void transfer_command_lcd(int data1)    
{
	char i;
	lcd_cs1=0;
	lcd_rs=0;
	for(i=0;i<8;i++)
	{   
		lcd_sclk=0;
		//delay_us(10);  //��������ʱ
		if(data1&0x80) lcd_sid=1;  
		else lcd_sid=0;   
		lcd_sclk=1;   
		//delay_us(10);  //��������ʱ    
		data1=data1<<=1;
	}  
	lcd_cs1=1; 
} 

//д���ݵ� LCD ģ�� 
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

//LCD ģ���ʼ�� 
void initial_lcd() 
{
	lcd_reset=0;          //�͵�ƽ��λ 
	delay(100);  lcd_reset=1;       //��λ��� 
	delay(100);         
	transfer_command_lcd(0xe2);  //��λ
	delay(5);  
	transfer_command_lcd(0x2c);  //��ѹ���� 1  
	delay(50);   
	transfer_command_lcd(0x2e);  //��ѹ���� 2 
	delay(50);  
	transfer_command_lcd(0x2f);  //��ѹ���� 3
	delay(5);  
	transfer_command_lcd(0x24);  //�ֵ��Աȶȣ������÷�Χ 0x20��0x27  
	transfer_command_lcd(0x81);  //΢���Աȶ�  
	transfer_command_lcd(0x23);  //΢���Աȶȵ�ֵ�������÷�Χ 0x00��0x3f 
	transfer_command_lcd(0xa2);  //1/9 ƫѹ�ȣ�bias��  
	transfer_command_lcd(0xc8);  //��ɨ��˳�򣺴��ϵ���  
	transfer_command_lcd(0xa0);  //��ɨ��˳�򣺴����� 
	transfer_command_lcd(0x40);  //��ʼ�У���һ�п�ʼ  
	transfer_command_lcd(0xaf);  //����ʾ	
}

void lcd_address(uint page,uint column) 
{  
	column=column-0x01; 
	transfer_command_lcd(0xb0+page-1); //����ҳ��ַ��ÿ 8 ��Ϊһҳ��ȫ���� 64 �У����ֳ� 8 ҳ  
	transfer_command_lcd(0x10+(column>>4&0x0f)); //�����е�ַ�ĸ� 4 λ  
	transfer_command_lcd(column&0x0f); //�����е�ַ�ĵ� 4 λ 
 }  
//ȫ������ 
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

//��ʾ 128x64 ����ͼ�� 
void display_128x64(uchar *dp) 
{
	uint i,j; 
	for(j=0;j<8;j++) 
	{  
		lcd_address(j+1,1); 
		for (i=0;i<128;i++) 
		{   
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1   
			dp++;   
		}  
	} 
}   
//��ʾ 16x16 ����ͼ�񡢺��֡���Ƨ�ֻ� 16x16 ���������ͼ�� 
void display_graphic_16x16(uchar page,uchar column,uchar *dp)
 {
	 uint i,j;
	 for(j=0;j<2;j++)
	 {   
		lcd_address(page+j,column*16);
		for (i=0;i<16;i++)
		{  
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1  
			dp++;   
		} 
	} 
}   
//��ʾ 8x16 ����ͼ��ASCII, �� 8x16 ����������ַ�������ͼ��
 void display_graphic_8x16(uchar page,uchar column,uchar *dp)
 {
	 uint i,j;
	 for(j=0;j<2;j++)
	 {
		 lcd_address(page+j,column);
		 for (i=0;i<8;i++)
		{
			transfer_data_lcd(*dp);     //д���ݵ� LCD,ÿд��һ�� 8 λ�����ݺ��е�ַ�Զ��� 1
			dp++;
		} 
	} 
}   
//��ʾ 5X8 ����ͼ��ASCII, �� 5x8 ����������ַ�������ͼ�� 
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
//��ָ�����Ѷ�ֿ� IC 
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
//�Ӿ���Ѷ�ֿ� IC ��ȡ���ֻ��ַ����ݣ�1 ���ֽڣ�
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
//��ָ����ַ��������д��Һ����ָ����page,column)������
 void get_and_write_16x16(ulong fontaddr,uchar page,uchar column) 
 {
	 uchar i,j,disp_data;
	 Rom_CS = 0;
	 send_command_to_ROM(0x03);
	 send_command_to_ROM((fontaddr&0xff0000)>>16);  //��ַ�ĸ� 8 λ,�� 24 λ
	 send_command_to_ROM((fontaddr&0xff00)>>8);   //��ַ���� 8 λ,�� 24 λ
	 send_command_to_ROM(fontaddr&0xff);     //��ַ�ĵ� 8 λ,�� 24 λ  
	 for(j=0;j<2;j++)  
	 {
		lcd_address(page+j,column);
		for(i=0; i<16; i++ )      
		{
			 disp_data=get_data_from_ROM();
			 transfer_data_lcd(disp_data); //д���ݵ� LCD,ÿд�� 1 �ֽڵ����ݺ��е�ַ�Զ��� 1
		}
	}
	Rom_CS=1;
}  
//��ָ����ַ��������д��Һ����ָ����page,column)������
 void get_and_write_8x16(ulong fontaddr,uchar page,uchar column) 
 {
	 uchar i,j,disp_data;
	 Rom_CS = 0;
	 send_command_to_ROM(0x03);
	 send_command_to_ROM((fontaddr&0xff0000)>>16);  //��ַ�ĸ� 8 λ,�� 24 λ
	 send_command_to_ROM((fontaddr&0xff00)>>8);   //��ַ���� 8 λ,�� 24 λ
	 send_command_to_ROM(fontaddr&0xff);     //��ַ�ĵ� 8 λ,�� 24 λ  
	 for(j=0;j<2;j++)
	 {
		 lcd_address(page+j,column);
		 for(i=0; i<8; i++ )      
		 {
			 disp_data=get_data_from_ROM();
			 transfer_data_lcd(disp_data); //д���ݵ� LCD,ÿд�� 1 �ֽڵ����ݺ��е�ַ�Զ��� 1
		 }  
	 } 
	 Rom_CS=1; 
}  
//��ָ����ַ��������д��Һ����ָ����page,column)������ 
void get_and_write_5x8(ulong fontaddr,uchar page,uchar column)  
{
	uchar i,disp_data;
	Rom_CS = 0;
	send_command_to_ROM(0x03);
	send_command_to_ROM((fontaddr&0xff0000)>>16);  //��ַ�ĸ� 8 λ,�� 24 λ
	send_command_to_ROM((fontaddr&0xff00)>>8);   //��ַ���� 8 λ,�� 24 λ
	send_command_to_ROM(fontaddr&0xff);     //��ַ�ĵ� 8 λ,�� 24 λ
	lcd_address(page,column);
	for(i=0; i<5; i++ )  
	{
		disp_data=get_data_from_ROM();
		transfer_data_lcd(disp_data); //д���ݵ� LCD,ÿд�� 1 �ֽڵ����ݺ��е�ַ�Զ��� 1
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
		{       //������壨GB2312�������ھ���Ѷ�ֿ� IC �еĵ�ַ�����¹�ʽ�����㣺
				//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
				//���ڵ��� 8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i]- 0xb0)*94;
			fontaddr += (text[i+1]-0xa1)+846;
			fontaddr = (ulong)(fontaddr*32);  
			get_and_write_16x16(fontaddr,page,column);  //��ָ����ַ��������д��Һ����ָ����page,column)������
			i+=2;
			column+=16;   
		}
		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))   
		{       //������壨GB2312��15x16 ����ַ��ھ���Ѷ�ֿ� IC �еĵ�ַ�����¹�ʽ�����㣺    
				//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0    
				//���ڵ��� 8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
			fontaddr = (text[i]- 0xa1)*94;
			fontaddr += (text[i+1]-0xa1);
			fontaddr = (ulong)(fontaddr*32);
			get_and_write_16x16(fontaddr,page,column);  //��ָ����ַ��������д��Һ����ָ����page,column)������
			i+=2;
			column+=16;
		}
		else if((text[i]>=0x20) &&(text[i]<=0x7e)) 
	   {
			fontaddr = (text[i]- 0x20);
			fontaddr = (unsigned long)(fontaddr*16);
			fontaddr = (unsigned long)(fontaddr+0x3cf80);
			get_and_write_8x16(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
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
			 get_and_write_5x8(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
			 i+=1;
			 column+=6;
		}
		 else i++;
	}
}