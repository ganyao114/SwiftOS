#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif


#include "K_Thread.h"
#include "K_CFG.h"
#include "K_Malloc.h"
#include "K_TDS_Operate.h"
#include "K_Core.h"
#include "K_Cpu_Core.h"


//线程池
static OS_ThreadPool OSThreadPool;

INT8U OSTDSInt(INT8U prio,OS_STK *pstk,OS_STK *pstkb,void   (*task)(void *p_arg),INT16U id,INT32U stk_size,INT16U opt)
{
    OS_TDS *ptds;
    
    //临界位置
    //ptds = OSThreadPool.GetThread(&OSThreadPool);
    ptds = OSThreadPool_Pop(&OSThreadPool);
    if(ptds==NULL)
        ptds = (OS_TDS *)Kmalloc(sizeof(OS_TDS));
    else
        goto lab1;     
    if(ptds==NULL)
    {
        return OS_TDS_MALLOC_ERR;
    }else{
        ptds->IN_Heap_OR_Stack = OS_TDS_HEAP;
    }
    //检查id是否被占用
lab1:    
    //配置tds各个参数
    ptds->TDS_Id = id;
		ptds->TDS_Prio = prio;
		ptds->TDS_TimeRunning = 0;
		ptds->TDS_TimeDly = 0;
		ptds->TDS_RunTick = 1;
		ptds->TDS_RunStatue = OS_TD_RUNNING;
    ptds->TDS_StkPtr = pstk;
		ptds->ChangePrio_Next = NULL;
		ptds->ChangePrio_Pre = NULL;
		ptds->PendResume_Next = NULL;
		ptds->PendResume_Pre = NULL;
    //插入任务就绪表
    TDS_Insert(ptds,OSTDSRdyList);
		TDS_Update_prio();
    return OS_TDS_INT_SUCCESS;    
}

INT16U OSThreadCreate(void   (*task)(void *p_arg),
                     void    *p_arg,
                     OS_STK  *ptos,
                     INT8U    prio)
{   
    OS_CPU_SR cpu_sr = 0u;
    void *pstack;
    INT16U id = 0u;
    //优先级超出范围
    if(prio < 0 && prio > OS_MAX_TDS_LV-1)
        return OS_PRIO_LEAK;
    
    /**
    *   检查其他参数
    **/
    
    OS_ENTER_CRITICAL();
    pstack = ThreadSTKInt(task,p_arg,ptos,0);
    //检查栈
	if(pstack == NULL){
        OS_EXIT_CRITICAL();
        return OS_TDS_STACK_ERR;
    }
    //分配Id
    id = OSThreadAllocId();
    if(id == 0){
       OS_EXIT_CRITICAL();
       return OS_TDS_ID_LEAK;
    }
    OSTDSInt (prio,pstack,(OS_STK *)0,task,id,0u,0u);
		++OS_THREAD_ALL;
		OS_EXIT_CRITICAL();
    if(prio<=CurrentRunThread->TDS_Prio&&OSRunning == OS_RUNNING)
    {   
       OSTDS_Sched();
		}
    return id;
}

INT8U OSGetTargetSwThread(void){
	INT8U prio = OS_HighestPrio;
	OS_TDS *tds = CurrentRunThreadNext;
	
	if(prio < CurrentRunThread->TDS_Prio)
		goto swcstep1;
	
	while(tds != NULL){
		if(tds->TDS_RunStatue == OS_TD_RUNNING){
			ThreadSwtTarget = tds;
			return OS_GET_CTXTARGET_SUCCESS;
		}
		tds = tds->TDS_Next;
	}
	
swcstep1:
	while(prio < OS_MAX_TDS_LV){
		tds = OSTDSRdyList[prio];
		while(tds != NULL){
			if(tds->TDS_RunStatue == OS_TD_RUNNING&&tds != CurrentRunThread){
				ThreadSwtTarget = tds;
				return OS_GET_CTXTARGET_SUCCESS;
			}
			tds = tds->TDS_Next;
		}
		++prio;
	}
	return 1u;
}

/*
INT8U OSGetTargetSwThread(void)
{   
	
    if(CurrentRunThread==NULL)
        ThreadSwtTarget =  OSTDSRdyList[OS_HighestPrio];
    
		else if(OS_HighestPrio < CurrentRunThread->TDS_Prio)
			ThreadSwtTarget =  OSTDSRdyList[OS_HighestPrio];
		
    else if(CurrentRunThread->TDS_Next != NULL)
            ThreadSwtTarget = CurrentRunThread->TDS_Next;
		else if(CurrentRunThread->TDS_RunStatue == OS_TD_WAITING && CurrentRunThread->TDS_Pre == NULL){
			INT8U prio = OS_HighestPrio;
			if(prio < OS_MAX_TDS_LV - 1)
						++prio;
						for(;prio < OS_MAX_TDS_LV;prio++){
								if(OSTDSRdyList[prio]!=NULL){
									ThreadSwtTarget =  OSTDSRdyList[prio];
									break;
								}
						}
		}
		else
        ThreadSwtTarget =  OSTDSRdyList[OS_HighestPrio];
    
    return OS_GET_CTXTARGET_SUCCESS;
}
*/




OS_TDS *OSThreadPool_Pop()
{
    if(OSThreadPool.Remain > 0u)
        return OSThreadPool.Threads[--OSThreadPool.Remain];
    else
        return NULL;
}

INT8U OSThreadPool_Push(OS_TDS *tds)
{
    if(OSThreadPool.Remain > OS_THREAD_POOL_SIZE-1)
        return OS_THREADPOOL_FULL;
    OSThreadPool.Threads[++OSThreadPool.Remain] = tds;
    return 0;
}

INT8U OSThreadPoolInit()
{
    int i;
		
    for(i = 0;i < OS_THREAD_POOL_SIZE;i++)
    {	
				OSThreadPool.ThreadStore[i].IN_Heap_OR_Stack = OS_TDS_STACK;
        OSThreadPool.Threads[i] = &OSThreadPool.ThreadStore[i];
    }
    OSThreadPool.Remain = OS_THREAD_POOL_SIZE;
    return OS_STACK_POOL_INT_SUCCESS;
}

OS_TDS *OSGgtTDSbyId(INT16U id)
{
    OS_TDS *ptds;
    INT8U i;
    for(i=0;i<OS_THREAD_POOL_SIZE;i++)
    {   
        if(OSThreadPool.ThreadStore[i].TDS_Id == id)
            return &OSThreadPool.ThreadStore[i];
    }
    ptds = ScanTb(id,OSTDSRdyList);
    if(ptds == NULL)
        ptds = ScanTb(id,OSTDSWaitList);
    return ptds;
}

INT16U OSThreadAllocId(void)
{   
    INT16U id = 1u;
    INT16U *ids = (INT16U *)Kmalloc(OS_THREAD_ALL*sizeof(INT16U));
    INT16U i = 0u;
    INT8U prio;
    OS_TDS *ptds;
    
    if(ids == NULL)
        return NULL;
    
    prio = OS_HighestPrio;
    ptds = OSTDSRdyList[prio];
    while(prio<OS_MAX_TDS_LV)
    {   
				ptds = OSTDSRdyList[prio];
        while(ptds!=NULL)
        {   
            ids[i] = ptds->TDS_Id;
            ++i;
            ptds = ptds->TDS_Next;
        }
        ++prio;
    }
    prio = OS_HighestWaitPrio;
    ptds = OSTDSWaitList[prio];
    while(prio<OS_MAX_TDS_LV)
    {   
				ptds = OSTDSWaitList[prio];
        while(ptds!=NULL)
        {   
            ids[i] = ptds->TDS_Id;
            ++i;
            ptds = ptds->TDS_Next;
        }
        ++prio;
    }
    if(i!=OS_THREAD_ALL)
        goto end;
    while(id<=65535u)
    {   
        INT16U x;
        INT8U state = 0u;
        for(x=0;x<OS_THREAD_ALL;x++)
        {
            if(id == ids[x])
            {
                state = 1u;
                break;
            }
        }
        if(state == 0u)
        {
            Kfree(ids);
            return id;
        }
        ++id;
    }
end:    
    Kfree(ids);
    return NULL;
}

INT8U OSTDSDel(INT16U id)
{

}

OS_TDS *ScanTb(INT16U id,OS_TDS *tb[])
{
    INT8U prio;
    OS_TDS *ptds;
    if(tb == OSTDSRdyList)
        prio = OS_HighestPrio;
    else
        prio = OS_HighestWaitPrio;
    ptds = tb[prio];
    while(prio<OS_MAX_TDS_LV)
    {	
				ptds = tb[prio];
        while(ptds!=NULL)
        {   
            if(id == ptds->TDS_Id)
                return ptds;
            ptds = ptds->TDS_Next;
        }
        ++prio;
    }
    return NULL;
}

INT8U KillThread(OS_TDS *tds)
{
    OS_CPU_SR cpu_sr = 0u;
    OS_TDS **tb;
    INT8U prio = tds->TDS_Prio;;
    if(OSIntNesting>0u)
        return OS_TDS_DEL_ERR;
    
    if(tds==(OS_TDS*)0)
        return OS_TDS_DEL_ERR;
    
    if(tds->TDS_Id==0u)
        return OS_TDS_DEL_ERR;
    
   
    OS_ENTER_CRITICAL();
    ++OSCtxLockNesting;
		//如果删除的是正在运行的任务
	if(tds == CurrentRunThread){
		return OS_TDS_DEL_ERR;
	}
   
    
    //tds就绪表操作
    
    tb = tds->TDS_RunStatue==OS_TD_RUNNING ? OSTDSRdyList : OSTDSWaitList;
    
    TDS_Delete(tds,tb);
		TDS_Update_prio();
    
    //tds结构体复位
    if(tds->IN_Heap_OR_Stack==OS_TDS_STACK)
    {
        tds->TDS_Pre = NULL;
        tds->TDS_Next = NULL;
        tds->TDS_TimeDly = NULL;
        tds->TDS_Id = NULL;
        tds->TDS_StkBase = NULL;
        tds->TDS_Prio = NULL;
//      tds->TDS_Hex = NULL;
        tds->TDS_StkPtr = NULL;
				tds->TDS_RunStatue = OS_TD_DESTORYED;
    }
    
    //tds结构体释放
    if(tds->IN_Heap_OR_Stack==OS_TDS_STACK)
    {
        OSThreadPool_Push(tds);
    }else{
        Kfree(tds);
    }
		//调度一次
    OSGetTargetSwThread();
    
	if(OSCtxLockNesting>0u)
		--OSCtxLockNesting;
	OS_EXIT_CRITICAL();
	OS_ThreadSwt();
}

void OS_Thread_Pend(OS_TDS *tds){
	OS_CPU_SR cpu_sr = 0u;
	OS_ENTER_CRITICAL();
	tds->TDS_RunStatue = OS_TD_WAITING;
	TDS_InsertFifo(tds,&OS_PendFifo,TDS_INTO_PENDRESUME);
	OS_EXIT_CRITICAL();
	OSTDS_Sched();
	if(tds == CurrentRunThread)
		OS_ThreadBlock(tds);
}

void OS_Thread_Resume(OS_TDS *tds){
	OS_CPU_SR cpu_sr = 0u;
	OS_ENTER_CRITICAL();
	tds->TDS_RunStatue = OS_TD_RUNNING;
	tds->TDS_TimeDly = 0;
	OS_EXIT_CRITICAL();
	OSTDS_Sched();
}


INT8U OS_ThreadChangePrio(OS_TDS *tds,INT8U prio){

}

INT8U OSIDLETDSInt(OS_STK *pstk,OS_STK *pstkb,INT32U stk_size,INT16U opt){
		OS_TDS *ptds;
    ptds = OSThreadPool_Pop();
    if(ptds==NULL)
        ptds = (OS_TDS *)Kmalloc(sizeof(OS_TDS));
    else
        goto lab2;     
    if(ptds==NULL)
    {
        return OS_TDS_MALLOC_ERR;
    }else{
        ptds->IN_Heap_OR_Stack = OS_TDS_HEAP;
    }
lab2:
		ptds->TDS_Id = 1u;
    ptds->TDS_Prio = OS_MAX_TDS_LV-1;
    //ptds->TDS_Hex = task;
    ptds->TDS_StkPtr = pstk;
		ptds->TDS_TimeRunning = 0;
		ptds->TDS_RunTick = 1;
		ptds->TDS_RunStatue = OS_TD_RUNNING;
		ptds->TDS_TimeDly = 0;
		OS_HighestPrio = OS_MAX_TDS_LV-1;
		OSTDSRdyList[OS_HighestPrio] = ptds;
		OS_HighestPrio = OS_MAX_TDS_LV-1;
    ThreadSwtTarget =  OSTDSRdyList[OS_MAX_TDS_LV-1];
    ++OS_THREAD_ALL;
    return OS_TDS_INT_SUCCESS;
}


void OSThreadSwtHook(void)
{
		CurrentRunThreadNext = CurrentRunThread->TDS_Next;
}

void OS_ThreadBlock(OS_TDS* tds){
		while(tds->TDS_TimeDly > 0||tds->TDS_RunStatue != OS_TD_RUNNING){
		
		}
}


