#include "contiki.h"
#include "ctimer.h"
#include "stimer.h"
#include "sys/clock.h"
#include"spin_common.h"
#include"spin_userstart.h"
#include"spin_interupt.h"
#include"spin_timer.h"
#include"spin_exinterupt.h"
#include"spin_gpio.h"
#include"spin_uart.h"
#include"spin_watchdog.h"
#include"spin_pwrmgr.h"
#include"xprintf.h"
#include"spin_1602.h"
#include"spin_at24c02.h"
#include"spin_pwm.h"
void hardware_init(){
   //硬件初始化设置
   spin_set_work_register_group(0);
   spin_interupt_init();
   spin_interupt_enable();
   spin_uart_init();
   spin_watchdog_enable();
   spin_watchdog_overtime(128,FSOC); //返回溢出时间ms 现在大约是4000
   spin_exint0_start(down_eage);
   spin_exint1_start(low_vol);
   //串口测试
   xdev_in(u_getc);
   xdev_out(u_putc);
   InitLCD();
}
PROCESS(led,"led");//定义led翻转任务
PROCESS_THREAD(led, ev, dataa)
{
	static struct etimer et;
    PROCESS_BEGIN();
	//延时1s的时钟
	etimer_set(&et,CLOCK_SECOND/2);
	while(1)
	{
		//等待1s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
	    //点亮led
		spin_set_gpio_bit_value(GPIO2,2,0);
		//等待1s
		etimer_restart(&et);
    	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		//关闭led
		spin_set_gpio_bit_value(GPIO2,2,1);
		etimer_restart(&et);
	}
   PROCESS_END();
}
PROCESS(dog,"dog");//看门狗任务
PROCESS_THREAD(dog, ev, dataa)
{
	static struct etimer et;
    PROCESS_BEGIN();
	//延时1s的时钟
	etimer_set(&et,CLOCK_SECOND*2);
	while(1)
	{
		//等待1s
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));	
	    spin_watchdog_clear();//喂狗
		etimer_restart(&et);
	}
   PROCESS_END();
}
process_event_t lcd_update;
int
main(void)
{
  hardware_init();
  clock_init();
  rtimer_init();
  ctimer_init();
  process_init();
  lcd_update = process_alloc_event();	//初始化事件
 /* start services */
  process_start(&etimer_process, NULL);
  process_start(&dog, NULL);
  process_start(&led, NULL);
  process_start(&lcd, NULL);
  process_start(&pwm_led,NULL);
  process_start(&at24c02,NULL);
  for(;;)process_run();
  return 0;
}
