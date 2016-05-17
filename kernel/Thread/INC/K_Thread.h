#ifndef _K_THREAD_
#define _K_THREAD_
#include "K_Cpu.h"
#include "K_CFG.h"
#include "Mutex.h"
/******************************************************************************
*                              �ں��̺߳��Ľṹ��
******************************************************************************/

/**
*   OS_TDS
*   ����:gy
*   beta1
**/

typedef struct ThreadDataStructure{
	//�����ջָ��
		OS_STK *TDS_StkPtr;
   	OS_STK *TDS_StkBase;
    OS_STK *TDS_Heap;
    //ǰ��������ָ��
    struct ThreadDataStructure *TDS_Pre;
    struct ThreadDataStructure *TDS_Next;
		struct ThreadDataStructure *TDS_Parent;
	
		//����ָ��
		struct ThreadDataStructure *PendResume_Pre;
		struct ThreadDataStructure *PendResume_Next;
		struct ThreadDataStructure *ChangePrio_Pre;
		struct ThreadDataStructure *ChangePrio_Next;
    //�߳�״̬
    INT8U TDS_RunStatue;
		INT32U TDS_RunTick;
    INT32U TDS_TimeDly;
		INT32U TDS_TimeRunning;
    INT8U TDS_Prio;
	
		//������
		OS_Mutex *mutex;
		
		//��Ϣ����
    
	//�߳���Ϣ
    INT8U IN_Heap_OR_Stack;
    INT16U TDS_Id;
    INT32U TDS_Name;
}OS_TDS;


/******************************************************************************
*                              �ں��̺߳��Ľṹ��
******************************************************************************/


/******************************************************************************
*                              �̳߳ؽṹ��
******************************************************************************/
typedef struct osthreadpool{
        INT8U Remain;
				OS_TDS ThreadStore[OS_THREAD_POOL_SIZE];
        OS_TDS* Threads[OS_THREAD_POOL_SIZE];
}OS_ThreadPool;

/******************************************************************************
*                              �̳߳ؽṹ��
******************************************************************************/


/******************************************************************************
*                              �̳߳ط���
******************************************************************************/

INT8U OSThreadPoolInit();

OS_TDS *OSThreadPool_Pop();

INT8U OSThreadPool_Push(OS_TDS *tds);

/******************************************************************************
*                              �̳߳ط���
******************************************************************************/

/******************************************************************************
*                              �ں��̷߳���
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
*                              �ں��̷߳���
******************************************************************************/



/******************************************************************************
*                              �ں��̴߳����־
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
*                              �ں��̴߳����־
******************************************************************************/

//�߳�״̬
#define OS_TD_RUNNING 0u
#define OS_TD_WAITING 1u
#define OS_TD_CHANGING_PRIO 2u
#define OS_TD_MUTEX 3u
#define OS_TD_DESTORYED 4u

//�洢��־λ
#define OS_TDS_STACK 0u
#define OS_TDS_HEAP 1u

#define OS_TDS_SIZE sizeof(OS_TDS)


#endif

