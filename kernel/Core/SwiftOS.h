#ifndef _SWIFT_OS_
#define _SWIFT_OS_


#include "K_Cpu.h"
#include "K_CFG.h"
#include "K_Thread.h"
#include "stdlib.h"
#include "Mutex.h"

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

/******************************************************************************
*                               核心宏定义
******************************************************************************/

//开关中断
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}


//定义任务切换宏
#define OS_ThreadSwt()  OSCtxSw()
#define OS_ThreadIntSWt()   OSIntCtxSw()


//系统运行状态
#define OS_RUNNING 1u
#define OS_UN_RUNNING 0u

//系统线程调度策略
#define OS_REALTIME_STRATEGY 0u //实时
#define OS_TIMERR_STRATEGY 1u //时间片轮转

/******************************************************************************
*                               核心宏定义
******************************************************************************/



/******************************************************************************
*                               定义全局变量
******************************************************************************/

//系统运行状态
OS_EXT INT8U OSRunning;

//系统线程调度策略
OS_EXT INT8U OSStrategy;

//系统运行时间(不要忘了初始化)
OS_EXT INT32U OSRunningTime;
//任务优先级就绪表
OS_EXT OS_TDS *OSTDSRdyList[OS_MAX_TDS_LV];
//任务优先级挂起表
OS_EXT OS_TDS *OSTDSWaitList[OS_MAX_TDS_LV]; 

//正在运行的线程
OS_EXT OS_TDS *CurrentRunThread;
//即将切换的目标任务
OS_EXT OS_TDS *ThreadSwtTarget;

OS_EXT OS_TDS *CurrentRunThreadNext;

//最高就绪优先级
OS_EXT INT8U OS_HighestPrio;
//最高等待优先级
OS_EXT INT8U OS_HighestWaitPrio;

//优先级标志位
OS_EXT INT8U OS_TDSRdyMark[OS_MAX_TDS_LV];

//任务总数
OS_EXT INT16U OS_THREAD_ALL;

//中断嵌套层数
OS_EXT INT8U OSIntNesting;

//调度锁
OS_EXT INT8U OSCtxLockNesting;

//互斥锁链表
OS_EXT OS_Mutex *OSMutexList;

//TimeTick任务队列
OS_EXT OS_TDS *OS_PendFifo;
OS_EXT OS_TDS *OS_ResumeFifo;
OS_EXT OS_TDS *OS_ChangePrioFifo;

/******************************************************************************
*                               定义全局变量
******************************************************************************/
#endif