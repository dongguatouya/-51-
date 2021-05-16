#include<reg51.h>
#include<intrins.h>
 
#define uchar unsigned char 
#define uint  unsigned int

sbit SCL=P2^0; //ʱ���߽ӿ� 
sbit SDA=P2^1; //�����߽ӿ�
sbit D1=P2^2;  //�����ź�
sbit beep=P2^4;//�������ӿ�
sbit E=P2^5;   //Һ��ʹ�� 
sbit RW=P2^6;  //��д����
sbit RS=P2^7;  //��������ѡ�� 
sbit k1=P3^2;  //�ָ���ʼ���밴��

uchar idata table1[6]={0,8,0,8,0,8}; //��ʼ������
uchar dd; 
uchar aa; //����������Ĵ������ﵽ���ν�����
uchar bb;
uchar cc;
//��ʱ�Ӻ���
void delay1(uint z)
{
	uint x,y;
	for(x=0;x<z;x++)
		for(y=0;y<110;y++);
}
void delay2() 
{
  ;;
}
//���߳�ʼ���Ӻ���
void chushi()
{
	SDA=1;
	delay2();
	SCL=1;
	delay2();
}
//�����ź��Ӻ���
void start()
{
	SDA=1;
	SCL=1;
	delay2();
	SDA=0;
	delay2();
}
//ֹͣ�ź��Ӻ���
void stop()
{
	SDA=0;
	delay2();
	SCL=1;
	delay2();
	SDA=1;
	delay2();
}
//Ӧ���ź��Ӻ���
void respons()
{
	uchar i=0;
	SCL=1;
	delay2();
	while(SDA==1&&i<255)//�ȴ�Ӧ�𣬹�һ��ʱ�䲻Ӧ���˳�ѭ��
		i++;
	SCL=0;
	delay2();
}
//дһ���ֽ��Ӻ���
void writebyte(uchar date)
{
	uchar i,temp;
	temp=date;
	for(i=0;i<8;i++)
	{
		temp<<=1;//temp����һλ���λ��CY
		SCL=0;
		delay2();
		SDA=CY;
		delay2();
		SCL=1;
		delay2();
	}	
	SCL=0;//Ӧ���ź���SCL = 1����������Ҫ��0
	delay2();
	SDA=1;//����Ҫ�ͷ���������
	delay2();
}
//��һ���ֽ��Ӻ���
uchar readbyte() 
{
	uchar i,k;
	SCL=0;
	delay2();
	SDA=1;
	for(i=0;i<8;i++)
	{
		SCL=1;	
		delay2();
		k=(k<<1)|SDA; //�����λ��һλλ�͵�K
		SCL=0;
		delay2();
	}
	delay2();
	return k;
}
//���ַдһ���ֽ������Ӻ���
void write_add(uchar address,uchar date)
{
	start();
	writebyte(0xa0);//A0,A1,A2�ӵأ�AT24C02оƬ��ַΪ1010���Ϳ�����Ϊ1010A2A1A0R/~W
	respons();
	writebyte(address);
	respons();
	writebyte(date);
	respons();
	stop();
}
//���ַ��һ���ֽ������Ӻ���
uchar read_add(uchar address)
{
	uchar date;
	start();
	writebyte(0xa0); //A0,A1,A2�ӵأ�AT24C02оƬ��ַΪ1010���Ϳ�����Ϊ1010A2A1A0R/~W
	respons();
	writebyte(address);
	respons();
	start();
	writebyte(0xa1); //A0,A1,A2�ӵأ�AT24C02оƬ��ַΪ1010���Ϳ�����Ϊ1010A2A1A0R/~W
	respons();
	date=readbyte();
	stop();
	return date;
}
//���ַдn���ֽ������Ӻ���
void write_n_add(uchar *p,uchar address,uchar n)
{
	uchar i;
	for(i=0;i<n;i++)
	{
		write_add((address+i),*(p+i));
		delay1(20);
	}
}
//���ַ��n���ֽ������Ӻ���
void read_n_add(uchar *p,uchar address,uchar n)
{
	uchar i;
	for(i=0;i<n;i++)
	{
		*(p+i)=read_add(address+i);	
	}
}
//LCD1602Һ����æ���Ӻ���
bit LCD1602_busy()
{                          
    bit a;
    RS=0;
    RW=1;
    E=1;
    _nop_();
    _nop_();
    a=(bit)(P0&0x80);
    E=0;
    return a; 
}
//LCD1602Һ��д�����Ӻ���
void write_com(uchar com) 
{
	while(LCD1602_busy());
    RW=0;
	RS=0;
    E=0;
	P0=com;
	delay1(5);
    E=1;
	delay1(5);
    E=0;
}
//LCD1602Һ��д�����Ӻ���
void write_date(uchar date)
{
	while(LCD1602_busy());
    RW=0;
  	RS=1;
    E=0;
  	P0=date;
	delay1(5);
    E=1;
	delay1(5);
    E=0;          
}
//дһ���ַ��Ӻ���
void write_1_char(uchar zifu)
{
	write_date(zifu);	
}
//��Һ��дn���ַ��Ӻ���
void write_n_char(uchar zifu[])
{
	uchar i;
	for(i=0;;i++)
	{
		write_1_char(zifu[i]);
		if(zifu[i+1]=='\0')
			break;
	}
}
//����Һ����ʾλ���Ӻ���
void LCD1602_pos(uchar x,uchar y) 
{
	uchar pos;
	if(x==0)
		x=0x80;
	else if(x==1)
		x=0x80+0x40;
	pos=x+y;
	write_com(pos);
}
//LCD1602Һ����ʼ���Ӻ���
void LCD1602_init()
{
    E=0;
    write_com(0x38);//����16x2��ʾ��5x7����8λ���ݿ� 
    write_com(0x0c);//���ÿ���ʾ������ʾ��� 
    write_com(0x06);//дһ���ַ����ַָ���1 
	write_com(0x01);//��ʾ��0������ָ����0         
}
//�����Ӻ���
void baojing1()
{
	int i=0;
	for(i=0;i<5;i++)
	{
		beep=1;
		D1=0;
		delay1(5);
		beep=0;
		D1=1;
		delay1(5);	
	}
}
void baojing2()
{
	uchar i;
	for(i=0;i<10;i++)
	{
		baojing1();
	}
}
//���󰴼�ɨ���Ӻ���
uchar keyscan()
{
	uchar temp,key;
	key=0xff;
	P1=0xfe; //����һ�����õ͵�ƽ
	temp=P1;//��JPJK�ĵ�ǰ״̬�� temp
	temp&=0xf0;// temp = temp & 0xf0 ��λ�� 
	if(temp!=0xf0)
	{
		delay1(10);//��ʱȥ��
		temp=P1;
		temp&=0xf0;
	   	if(temp!=0xf0)//��һ���м�������
		{
			temp=P1;//�������µļ�
			baojing1();
			switch(temp)
			{
				case 0xee: 	key=1;
							break;
				case 0xde: 	key=2;
							break;
				case 0xbe: 	key=3;
							break;
				case 0x7e: 	key=0x0a;
							break;
			}
			while(temp!=0xf0)//�ȴ������ͷ�
			{
				temp=P1;
				temp&=0xf0;
			}
		}
	}
	P1=0xfd; //���ڶ������õ͵�ƽ
	temp=P1;//��JPJK�ĵ�ǰ״̬�� temp
	temp&=0xf0;// temp = temp & 0xf0
	if(temp!=0xf0)
	{
		delay1(10);//��ʱȥ��
		temp=P1;
		temp&=0xf0;
	   	if(temp!=0xf0)//�ڶ����м�������
		{
			temp=P1;//�������µļ�
			baojing1();
			switch(temp)
			{
				case 0xed: 	key=4;
							break;
				case 0xdd: 	key=5;
							break;
				case 0xbd: 	key=6;
							break;
				case 0x7d: 	key=0x0b;
							break;
			}
			while(temp!=0xf0)//�ȴ������ͷ�
			{
				temp=P1;
				temp&=0xf0;
			}
		}
	}
	P1=0xfb; //���� �������õ͵�ƽ
	temp=P1;//��JPJK�ĵ�ǰ״̬�� temp
	temp&=0xf0;// temp = temp & 0xf0
	if(temp!=0xf0)
	{
		delay1(10);//��ʱȥ��
		temp=P1;
		temp&=0xf0;
	   	if(temp!=0xf0)//�������м�������
		{
			temp=P1;//�������µļ�
			baojing1();
			switch(temp)
			{
				case 0xeb: 	key=7;
							break;
				case 0xdb: 	key=8;
							break;
				case 0xbb: 	key=9;
							break;
				case 0x7b: 	key=0x0c;
							break;
			}
			while(temp!=0xf0)//�ȴ������ͷ�
			{
				temp=P1;
				temp&=0xf0;
			}
		}
	}
	P1=0xf7; //�����������õ͵�ƽ
	temp=P1;//��JPJK�ĵ�ǰ״̬�� temp
	temp&=0xf0;// temp = temp & 0xf0
	if(temp!=0xf0)
	{
		delay1(10);//��ʱȥ��
		temp=P1;
		temp&=0xf0;
	   	if(temp!=0xf0)//�������м�������
		{
			temp=P1;//�������µļ�
			baojing1();
			switch(temp)
			{
				case 0xe7: 	key=0;
							break;
				case 0xd7: 	key=0;
							break;
				case 0xb7: 	key=0x0f;
							break;
				case 0x77: 	key=0x0d;
							break;
			}
			while(temp!=0xf0)//�ȴ������ͷ�
			{
				temp=P1;
				temp&=0xf0;
			}
		}
	}
   	return key; //���ذ��µļ� 
}
//�Ƚ������Ӻ���
bit sfj1(uchar *string1,uchar *string2)
{
	uchar i;
	for(i=0;i<6;i++)
	{
		if(string1[i]!=string2[i])
			return 0;
	}
	return 1;
}
//ѡ������������޸����뺯��
uchar step_choose()
{
	uchar key;
	key=0xff;
	write_com(0x06);//дһ���ַ����ַָ��� 1 
    write_com(0x01);//��ʾ���㣬����ָ������
	LCD1602_pos(0,0);
	write_n_char(" Please Input ");
	LCD1602_pos(1,0);
	write_n_char(" Press key begin  ");	
	while((key!=0x0a)&&(key!=0x0b))
		key=keyscan();	
	return key;
}
//���������Ӻ���
bit input_mima(uchar * mima)
{
	uchar i,key;
	LCD1602_pos(1,0);
	for(i=0;i<7;i++)
	{
		delay1(100);
		if(i<6)
		{
			do
			{
				key=keyscan();
			}//ɨ�����
			while(key==0xff);
			if((key!=0x0f)&&(key!=0x0a)&&(key!=0x0c))//�����˸�Ҳ����ȷ�ϼ�
			{
				write_date('*');//�����ּ���ʾ*
				mima[i]=key;
			}
			if(key==0x0f)//���˸��
			{
				if(i>0)
				{
				    LCD1602_pos(1,--i);//���ǰ��һλ
					write_date(' ');//���һλ
					mima[i]=' ';//д��
					LCD1602_pos(1,i);
					i--;//�����������һ ,��Ϊѭ�����+1������������Ҫ��1
				}
			}
			if(key==0x0c)//û����������뷵�ش�����Ϣ
			{
				LCD1602_pos(0,0);
				return(0);
			}
		}
		if(i==6)
		{
			do
			{
				key=keyscan();
			}
			while((key!=0x0f)&&(key!=0x0c));
			if(key==0x0f)
			{
				LCD1602_pos(1,--i);
				write_date(' ');
				mima[i]=' ';
				LCD1602_pos(1,i);
				i--;
			}
			if(key==0x0c)//����λ����ȷ
			{
				return(1);//����1��ȷ�ź�
			}
		}
	}	
}
//���봦���Ӻ���
void sfj2() 
{
	uchar key,i;
	uchar idata table2[6]={' ',' ',' ',' ',' ',' '};//������뻺����
	uchar idata table3[6]={' ',' ',' ',' ',' ',' '};
	uchar idata table4[6]={' ',' ',' ',' ',' ',' '};
	key=step_choose();
	if(key==0x0a)//begin������,�����������룬����
	{
		read_n_add(table2,0x00,6);
		write_com(0x06);//дһ���ַ����ַָ��� 1 
		write_com(0x01);//��ʾ���㣬����ָ������
		write_com(0x0f);//��ʾ���
		LCD1602_pos(0,0);
		write_n_char(" Please Input ");		
		if(input_mima(table3)) //������������
		{
			if(sfj1(table3,table1) || sfj1(table3,table2)) //������ȷ
			{
				LCD1602_pos(0,0);
				write_com(0x0c);
	        	write_com(0x06);//дһ���ַ����ַָ��� 1 
	         	write_com(0x01);//��ʾ���㣬����ָ������
				write_n_char(" password right ");
				aa=0; //�������������
				D1=0; //����
				beep=1; //��һ��
				delay1(400);
				beep=0;
				TR0=1;
				cc=1;
				while(key!=0x0d&&cc) //lockû����һֱ��
				{			
					key=keyscan();
				}
				TR0=0;
				D1=1;//lock�����˹���
			}
			else //���� ����ȷ
			{
				LCD1602_pos(0,0);
				write_com(0x0c); //�ع��
				write_com(0x06);//дһ���ַ����ַָ��� 1 
				write_com(0x01);//��ʾ���㣬����ָ������
				write_n_char(" password wrong");
				delay1(400);
				aa++;
				if(aa==4)
				{
					aa=0;
			    	i=20; //���벻��ȷ����
					while(i--)
					 	baojing2();
				}
			}
		}
		else //����û��������ȷ�����
		{
			LCD1602_pos(0,0);
			write_com(0x0c); //�ع��
	   		write_com(0x06);//дһ���ַ����ַָ��� 1 
		   	write_com(0x01);//��ʾ���㣬����ָ������
			write_n_char(" password wrong");
			delay1(400);
			aa++;
			if(aa==4)
			{
				aa=0;
		    	i=20; //���벻��ȷ����
				while(i--)
				 	baojing2();
			}
		}
	}
	if(key==0x0b)//set������,�޸�����
	{
		read_n_add(table2,0x00,6);
		write_com(0x06);//дһ���ַ����ַָ��� 1 
		write_com(0x01);//��ʾ���㣬����ָ������
		write_com(0x0f);//��ʾ���
		LCD1602_pos(0,0);
		write_n_char(" input password");		
		write_com(0x0f);//��ʾ���	
		if(input_mima(table3)) //������������
		{
			if(sfj1(table3,table1) || sfj1(table3,table2)) //������ȷ
			{
				LCD1602_pos(0,0);
				write_com(0x0c);
	      		write_com(0x06);//дһ���ַ����ַָ��� 1 
	      		write_com(0x01);//��ʾ���㣬����ָ������
				write_n_char("password right ");
				aa=0; //�������������
				delay1(500);
				dd=1;
				while(dd) //���濪ʼ�޸�����
				{
					write_com(0x06);//дһ���ַ����ַָ��� 1 
					write_com(0x01);//��ʾ���㣬����ָ������
					write_com(0x0f);//��ʾ���
					LCD1602_pos(0,0);
					write_n_char("In new password");
					delay1(500);
					if(input_mima(table3)) //������������
					{
						LCD1602_pos(0,0);
						write_com(0x0c);
			         	write_com(0x06);//дһ���ַ����ַָ��� 1 
			         	write_com(0x01);//��ʾ���㣬����ָ������
						write_n_char("Input new pass"); //ȷ��������
						LCD1602_pos(1,0);
						write_n_char("word again");
						delay1(800);
						LCD1602_pos(0,0);
						write_com(0x0f);//��ʾ���
			         	write_com(0x06);//дһ���ַ����ַָ��� 1 
			         	write_com(0x01);//��ʾ���㣬����ָ������
						write_n_char("In new password");						
						if(input_mima(table4)) //������������
						{
							if(sfj1(table3,table4)) //������ȷ
							{
								LCD1602_pos(0,0);
								write_com(0x0c);
						      	write_com(0x06);//дһ���ַ����ַָ��� 1 
						      	write_com(0x01);//��ʾ���㣬����ָ������
								write_n_char("password has");
								LCD1602_pos(1,0);
								write_n_char("change already");
								write_n_add(table4,0x00,6); //���޸ĵ�������24C02
								delay1(1000);
								dd=0;
							}
							else //���� ����ȷ
							{
								LCD1602_pos(0,0);
								write_com(0x0c); //�ع��
								write_com(0x06);//дһ���ַ����ַָ��� 1 
								write_com(0x01);//��ʾ���㣬����ָ������
								write_n_char("password wrong");
								delay1(600);
								aa++;
								if(aa==4)
								{
									aa=0;
								   	i=20; //3���������벻��ȷ����
									while(i--)
										baojing2();
								}
								dd=0;
							}
						}
						else //����û��������ȷ�����
						{
							LCD1602_pos(0,0);
							write_com(0x0c); //�ع��
						   	write_com(0x06);//дһ���ַ����ַָ��� 1 
							write_com(0x01);//��ʾ���㣬����ָ������
							write_n_char(" password wrong");
							delay1(600);
							aa++;
							if(aa==4)
							{
								aa=0;
							    i=20; //3���������벻��ȷ����
								while(i--)
									baojing2();
							}
						}	
					}
					else //����û��������ȷ�����
					{
						LCD1602_pos(0,0);
						write_com(0x0c); //�ع��
				   		write_com(0x06);//дһ���ַ����ַָ��� 1 
					   	write_com(0x01);//��ʾ���㣬����ָ������
						write_n_char("password wrong");
						delay1(600);
						dd=0;
						aa++;
						if(aa==4)
						{
							aa=0;
					    	i=20; //3���������벻��ȷ����
							while(i--)
							 	baojing2();
						}
					}									
				}
			}
			else //���� ����ȷ
			{
				LCD1602_pos(0,0);
				write_com(0x0c); //�ع��
				write_com(0x06);//дһ���ַ����ַָ��� 1 
				write_com(0x01);//��ʾ���㣬����ָ������
				write_n_char("password wrong");
				delay1(600);
				aa++;
				if(aa==4)
				{
					aa=0;
			    	i=20; //3���������벻��ȷ����
					while(i--)
					 	baojing2();
				}
			}
		}
		else //����û��������ȷ�����
		{
			LCD1602_pos(0,0);
			write_com(0x0c); //�ع��
	   		write_com(0x06);//дһ���ַ����ַָ��� 1 
		   	write_com(0x01);//��ʾ���㣬����ָ������
			write_n_char("password wrong");
			delay1(600);
			aa++;
			if(aa==4)
			{
				aa=0;
		    	i=20; //3���������벻��ȷ����
				while(i--)
				 	baojing2();
			}
		}		
	}
}
//������
void main()
{
	LCD1602_init();//����Һ����ʼ���Ӻ���
	chushi();//����24C02���߳�ʼ���Ӻ���
	beep=0;
	D1=1; //����
	TMOD=0x01;//ѡ��ʱ��0��ʽ1
	EA=1;//�����ж�
	ET0=1;//�򿪶�ʱ��0�ж�
	EX0=1; //���ⲿ�ж�0
	IT0=1;//�½��ش���
	TR0=0;//�رն�ʱ��
	TH0=0x4c;//50msװ��ֵ
	TL0=0x00;
	while(1)
	{
	  	sfj2();//�������봦���Ӻ��� 
	}
}
//�ⲿ�ж�0�Ӻ���
void wb0() interrupt 0
{
	delay1(2000);
	if(!k1)
	{
		LCD1602_pos(0,0);
		write_com(0x0c); //�ع��
	   	write_com(0x06);//дһ���ַ����ַָ��� 1 
		write_com(0x01);//��ʾ���㣬����ָ������
		chushi();//24C02���߳�ʼ��
		write_n_add(table1,0x00,6);
		write_n_char("password renew");
		LCD1602_pos(1,0);
		write_n_char("already");
		delay1(1000);
		LCD1602_pos(0,0);
		write_com(0x0c); //�ع��
	   	write_com(0x06);//дһ���ַ����ַָ��� 1 
		write_com(0x01);//��ʾ���㣬����ָ������
		write_n_char(" Please Input ");
	  LCD1602_pos(1,0);
	  write_n_char(" Press key begin  ");
	}
}
//��ʱ��0�ж��Ӻ���
void t0() interrupt 1 
{
	TH0=0x4c;//50ms��ʱ
	TL0=0x00;
	bb++;
	if(bb==200)//10��ʱ�䵽
	{
		bb=0;
		cc=0;
	}
}
				