#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "malloc.h"
#include "sram.h"
#include "K_Cpu_Core.h"
#include "K_Thread.h"
#include "Start.h"
#include "K_Cpu.h"

#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif
OS_STK IDLE_THREAD_STK[128];
OS_STK FIRST_THREAD_STK[128];
OS_STK SEC_THREAD_STK[128];


void FIRST_TASK(void)
{	
	OS_CPU_SR cpu_sr = 0u;
	u8 t;
	for(;;)
    {
			INT32U j = 0;
			t++;
		//OS_Thread_Pend(CurrentRunThread);
	delay_ms(10);
		if(t==8)LED0=1;	//LED0灭
		if(t==100)		//LED0亮
		{
			t=0;
			LED0=0;
		}
		
    }
}

void SC_TASK(void)
{
	u8 t;
	for(;;)
    {
			INT32U j = 0;
			t++;
		delay_ms(100);
		if(t==8)LED0=1;	//LED0灭
		if(t==100)		//LED0亮
		{
			t=0;
			LED0=0;
		}
    }
}

void OS_IDLE(){
		OS_CPU_SR cpu_sr = 0u;
			u8 t;
		OS_ENTER_CRITICAL();
		OSThreadCreate(FIRST_TASK,NULL,&FIRST_THREAD_STK[127],6u);
		OSThreadCreate(SC_TASK,NULL,&SEC_THREAD_STK[127],5u);
		OS_EXIT_CRITICAL();
	for(;;){
		
	}

}

static void IntStaticData(void)
{	
    OSRunning = OS_UN_RUNNING;
    OS_THREAD_ALL = 0;
    OSIntNesting = 0;
    OSCtxLockNesting = 0;
    OSRunningTime = 0;
	  OS_HighestWaitPrio = OS_MAX_TDS_LV - 1;
		OS_HighestPrio = OS_MAX_TDS_LV - 1;
}

static void CreateInitThread(void)
{   
    OS_CPU_SR cpu_sr = 0u;
    void *pstack;
   
    OS_ENTER_CRITICAL();
    pstack = ThreadSTKInt(OS_IDLE,NULL,&IDLE_THREAD_STK[127],0);
    //检查栈
		if(pstack == NULL){
        OS_EXIT_CRITICAL();
        return;
    }
  	OSIDLETDSInt(pstack,(OS_STK *)0,0u,0u);
    OS_EXIT_CRITICAL();
}
INT8U OS_COREHWInt(void)
{
  delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);
	LED_Init();		  		//初始化与LED连接的硬件接口
	FSMC_SRAM_Init();		//初始化外部SRAM
	mem_init(SRAMIN);		//初始化内部内存池
	mem_init(SRAMEX);		//初始化外部内存池
  return 0u;
}

INT8U OSInt(void)
{   
    OSThreadPoolInit();
    IntStaticData();
    CreateInitThread();
    OSStartHighRdy();
}