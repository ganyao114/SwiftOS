#ifndef _K_THREAD_
#define _K_THREAD_
#include "K_Cpu.h"
#include "K_CFG.h"
#include "Mutex.h"
/******************************************************************************
*                              内核线程核心结构体
******************************************************************************/

/**
*   OS_TDS
*   作者:gy
*   beta1
**/

typedef struct ThreadDataStructure{
	//任务堆栈指针
		OS_STK *TDS_StkPtr;
   	OS_STK *TDS_StkBase;
    OS_STK *TDS_Heap;
    //前后趋链表指针
    struct ThreadDataStructure *TDS_Pre;
    struct ThreadDataStructure *TDS_Next;
		struct ThreadDataStructure *TDS_Parent;
	
		//队列指针
		struct ThreadDataStructure *PendResume_Pre;
		struct ThreadDataStructure *PendResume_Next;
		struct ThreadDataStructure *ChangePrio_Pre;
		struct ThreadDataStructure *ChangePrio_Next;
    //线程状态
    INT8U TDS_RunStatue;
		INT32U TDS_RunTick;
    INT32U TDS_TimeDly;
		INT32U TDS_TimeRunning;
    INT8U TDS_Prio;
	
		//互斥锁
		OS_Mutex *mutex;
		
		//消息队列
    
	//线程信息
    INT8U IN_Heap_OR_Stack;
    INT16U TDS_Id;
    INT32U TDS_Name;
}OS_TDS;


/******************************************************************************
*                              内核线程核心结构体
******************************************************************************/


/******************************************************************************
*                              线程池结构体
******************************************************************************/
typedef struct osthreadpool{
        INT8U Remain;
				OS_TDS ThreadStore[OS_THREAD_POOL_SIZE];
        OS_TDS* Threads[OS_THREAD_POOL_SIZE];
}OS_ThreadPool;

/******************************************************************************
*                              线程池结构体
******************************************************************************/


/******************************************************************************
*                              线程池方法
******************************************************************************/

INT8U OSThreadPoolInit();

OS_TDS *OSThreadPool_Pop();

INT8U OSThreadPool_Push(OS_TDS *tds);

/******************************************************************************
*                              线程池方法
******************************************************************************/

/******************************************************************************
*                              内核线程方法
******************************************************************************/

INT8U OSGetTargetSwThread(void);

INT8U OSTDSInt(INT8U prio,OS_STK *pstk,OS_STK *pstkb,void   (*task)(void *p_arg),INT16U id,INT32U stk_size,INT16U opt);

INT8U OSIDLETDSInt(OS_STK *pstk,OS_STK *pstkb,INT32U stk_size,INT16U opt);

INT16U OSThreadCreate(void   (*task)(void *p_arg),
                     void    *p_arg,
                     OS_STK  *ptos,
                     INT8U    prio);
			
void OS_ThreadBlock(OS_TDS* tds);

INT8U OS_ThreadChangePrio(OS_TDS *tds,INT8U prio);

INT16U OSThreadAllocId(void);

OS_TDS *OSGgtTDSbyId(INT16U id);

INT8U OSTDSDel(INT16U id);

void OS_ThreadKillSelf(void);

void OSThreadSwtHook(void);

OS_TDS *ScanTb(INT16U id,OS_TDS *tb[]);

INT8U KillThread(OS_TDS *tds);

void OS_Thread_Pend(OS_TDS *tds);

void OS_Thread_Resume(OS_TDS *tds);


/******************************************************************************
*                              内核线程方法
******************************************************************************/



/******************************************************************************
*                              内核线程错误标志
******************************************************************************/

#define OS_PRIO_LEAK    0u
#define OS_THREAD_RETURN 1u
#define OS_THREADPOOL_FULL 2u
#define OS_TDS_MALLOC_ERR 3u
#define OS_TDS_STACK_ERR 4u
#define OS_STACK_POOL_INT_SUCCESS 5u
#define OS_TDS_INT_SUCCESS 6u
#define OS_GET_CTXTARGET_SUCCESS 7u
#define OS_TDS_ID_LEAK 8u
#define OS_TDS_DEL_ERR 9u
#define OS_TDS_DEL_SUCCESS 0u
#define OS_RAM_LEAK 10u


/******************************************************************************
*                              内核线程错误标志
******************************************************************************/

//线程状态
#define OS_TD_RUNNING 0u
#define OS_TD_WAITING 1u
#define OS_TD_CHANGING_PRIO 2u
#define OS_TD_MUTEX 3u
#define OS_TD_DESTORYED 4u

//存储标志位
#define OS_TDS_STACK 0u
#define OS_TDS_HEAP 1u

#define OS_TDS_SIZE sizeof(OS_TDS)


#endif

