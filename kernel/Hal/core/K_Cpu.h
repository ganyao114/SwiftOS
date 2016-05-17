/************************ (C) COPYLEFT 2015 JUST_GY *************************

* File Name		: K_Cpu.h
* Author		: GY
* Date			: 25/6/2015
* Description	: Swift在STM32上的移植代码C语言部分，
*				  包括任务堆栈初始化代码和钩子函数等

******************************************************************************/

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif



/******************************************************************************
*                    定义与编译器无关的数据类型
******************************************************************************/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;			/* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;			/* Signed    8 bit quantity       */
typedef unsigned short INT16U;			/* Unsigned 16 bit quantity       */
typedef signed   short INT16S;			/* Signed   16 bit quantity       */
typedef unsigned int   INT32U;			/* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;			/* Signed   32 bit quantity       */
typedef float          FP32;			/* Single precision floating point*/
typedef double         FP64;			/* Double precision floating point*/
typedef unsigned int   Point;		    /*32位元指针宽度*/

//STM32是32位位宽的,这里OS_STK和OS_CPU_SR都应该为32位数据类型
typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/
/* 
*******************************************************************************
*                             Cortex M3
*                     Critical Section Management
*******************************************************************************
*/


#define  OS_STK_GROWTH        1 
#define  OS_TASK_SW()         OSCtxSw()
/******************************************************************************
*                               定义堆栈
******************************************************************************/


/* 
*******************************************************************************
*                             Cortex M3
*                     Critical Section Management
*******************************************************************************
*/


/******************************************************************************
*                               定义中断
******************************************************************************/

void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);
void       OSPendSV(void);
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);

/* 
*******************************************************************************
*                             Cortex M3
*                     Critical Section Management
*******************************************************************************
*/

