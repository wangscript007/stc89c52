
/***************************************************************************************************    
工程名称：	1602
功能描述：  1602LCD的功能实现函数
硬件连接：  将1602液晶接口对应插接到P4接口，注意1-16脚不要插反（有些厂家的液晶顺序是反的）。
维护记录：  2011-8-22
***************************************************************************************************/
#include"spin_1602.h"
#include"spin_timer.h"
#include"etimer.h"
#include"process.h"
#include<reg52.h>
#include<stdio.h>
sbit rs=P2^6;    //命令/数据选择
sbit rw=P2^5;    //读写口
sbit  e=P2^7;    //锁存控制
/*DB0~DB7连到了P00~P07*/
sbit BusyFlag=P0^7;//查忙标志位
uchar  dis1[16]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',};//LCD第一行显示当前时间，格式    00:00:00  ,四五 七八 
uchar  dis2[16]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',};//LCD第2行显示The T is:xxx.x 0xeb,'C'	9 10 11 13 

//**************************************************************************************************
//查忙
//**************************************************************************************************
void checkbusy()   
{
	P0= 0xFF;            //单片机I/O口设置为输入	
	rs = 0;              //命令/数据选择,为0时选择命令
	rw = 1;              //读/写选择，为1时选择读
	e = 0;
	e = 1;                //使能
	while (BusyFlag== 1); //查忙标志位，等待标志位为0，即表示写入完毕
	e= 0;                 //关闭读写
}
//**************************************************************************************************
//向LCD写一命令
//**************************************************************************************************
void wcode(uchar t)
{
  checkbusy();    //查忙
  rs=0;           // 写的是命令
  rw=0;           // 写状态
  e=1;            //使能
  P0=t;           //写入命令 
  delay1ms();      //等待写入,如果时间太短，会导致液晶无法显示  大约延时2ms
 // delay1ms();
  e=0;            //数据的锁定
}
//**************************************************************************************************
//向LCD写一数据
//**************************************************************************************************
void wdata(uchar t)
{
  checkbusy();   //查忙
  rs=1;          // 写的是数据
  rw=0;          // 写状态
  e=1;           //使能
  P0=t;          //写入数据
  delay1ms();     //等待写入,如果时间太短，会导致液晶无法显示	  大约延时2ms
 // delay1ms();
  e=0;           //数据的锁定
}
//**************************************************************************************************
//LCD显示第一行
//**************************************************************************************************
void put_line1(uchar line1[])
{
  uchar i;
  wcode(0x80);          //设置第一行显示地址
  for(i=0;i<16;i++)     //循环16次，写完1行
    {
      wdata(line1[i]);   //写入该行数据
    }
}
//**************************************************************************************************
//LCD显示第二行
//**************************************************************************************************
void put_line2(uchar line2[])
{
   uchar i;
   wcode(0xc0);        //设置第二行显示地址
   for(i=0;i<16;i++)   //循环16次，写完1行
    {
      wdata(line2[i]);  //写入该行数据
    }
}
//**************************************************************************************************
//LCD 初始化
//**************************************************************************************************
void InitLCD(){  		 
   wcode(0x01);	  //清屏
   wcode(0x06);   //输入方式控制,增量光标不移位
   wcode(0x0e);   //显示开关控制
   wcode(0x38);   //功能设定:设置16x2显示，5x7显示,8位数据接口  38   	
}  
PROCESS(lcd,"lcd");//
extern process_data_t global_dataa;
PROCESS_THREAD(lcd, ev, dataa)
{
    PROCESS_BEGIN();
	while(1)
	{
		//等待事件到来
		PROCESS_WAIT_EVENT_UNTIL(ev==lcd_update);	
		put_line1((uchar*)global_dataa);
		put_line2(dis2);
		ev=0; //清除事件
	}
   PROCESS_END();
}