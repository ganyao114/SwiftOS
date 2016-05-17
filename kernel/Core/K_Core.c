#include "K_TDS_Operate.h"
#include "K_Thread.h"
#include "K_Core.h"

#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif


/**
*   OS_TimeTick()
*   心脏脉动函数
*   核心函数
*   调用者:时钟中断
*   功能：时间计数器减一,任务切换等等
**/

void OS_TimeTick(void)
{   
	OS_CPU_SR  cpu_sr = 0u;
  OS_TDS  *ptds;
	INT8U RdyPrio = OS_HighestPrio;
	INT8U WaitPrio;
	OS_TDS *PendFifo;
	if(OSRunning == OS_UN_RUNNING)
		return;
	OS_ENTER_CRITICAL();
  ++OSRunningTime;
	
	PendFifo = OS_PendFifo;
	if(PendFifo == NULL)
		goto tickstep1;
	while(PendFifo != NULL){
		OS_TDS *tmp = PendFifo;
		TDS_ChangeTab(PendFifo,TDS_INTO_WAIT);
		TDS_Update_prio();
		PendFifo = PendFifo->PendResume_Next;
		TDS_DeleteFifo(tmp,&OS_PendFifo,TDS_INTO_PENDRESUME);
	} 
	
	
tickstep1:
	PendFifo = OS_ResumeFifo;
	if(PendFifo == NULL)
		goto tickstep2;
	while(PendFifo != NULL){
		OS_TDS *tmp = PendFifo;
		TDS_ChangeTab(PendFifo,TDS_INTO_RDY);
		TDS_Update_prio();
		PendFifo = PendFifo->PendResume_Next;
		TDS_DeleteFifo(tmp,&OS_ResumeFifo,TDS_INTO_PENDRESUME);
	}
	
	
	
tickstep2:
	  WaitPrio = OS_HighestWaitPrio;
		while(WaitPrio < OS_MAX_TDS_LV){
			ptds = OSTDSWaitList[WaitPrio];
			while(ptds!=NULL){				
				if(ptds->TDS_TimeDly > 0){
					--ptds->TDS_TimeDly;
					if(ptds->TDS_TimeDly == 0)
							ptds->TDS_RunStatue = OS_TD_RUNNING;
				}else if(ptds->TDS_TimeDly == 0&&ptds->TDS_RunStatue == OS_TD_RUNNING){
							TDS_ChangeTab(ptds,TDS_INTO_RDY);
							TDS_Update_prio();
							OSThreadSwtHook();
				}					
				ptds = ptds->TDS_Next;
			} 
			++WaitPrio;
		}
    OS_EXIT_CRITICAL();
}


/*
void OS_TimeTick(void)
{   
	OS_CPU_SR  cpu_sr = 0u;
    OS_TDS  *ptds;
	INT8U RdyPrio = OS_HighestPrio;
	INT8U WaitPrio = OS_HighestWaitPrio;
	if(OSRunning == OS_UN_RUNNING)
		return;
	OS_ENTER_CRITICAL();
    ++OSRunningTime;
	while(RdyPrio < OS_MAX_TDS_LV){
		ptds = OSTDSRdyList[RdyPrio];
		while(ptds!=NULL){
			if(ptds->TDS_RunStatue == OS_TD_WAITING||ptds->TDS_TimeDly > 0){
				TDS_ChangeTab(ptds,TDS_INTO_WAIT);
				TDS_Update_prio();
				if(CurrentRunThread == ptds)
					CurrentRunThread = NULL;
				}
				ptds = ptds->TDS_Next;
			}
			++RdyPrio;
		}
		while(WaitPrio < OS_MAX_TDS_LV){
			ptds = OSTDSWaitList[WaitPrio];
			while(ptds!=NULL){				
				if(ptds->TDS_TimeDly > 0){
					if(--ptds->TDS_TimeDly)
							ptds->TDS_RunStatue = OS_TD_RUNNING;
				}else if(ptds->TDS_TimeDly == 0&&ptds->TDS_RunStatue == OS_TD_RUNNING){
							TDS_ChangeTab(ptds,TDS_INTO_RDY);
							TDS_Update_prio();
				}					
				ptds = ptds->TDS_Next;
			} 
			++WaitPrio;
		}
    OS_EXIT_CRITICAL();
}
*/

//通知系统进入中断
void OSEnterInt(void)
{		
	if(OSRunning == OS_RUNNING){
		if(OSIntNesting < 255u)
				++OSIntNesting;
	}
}

//通知系统退出中断
void OSExtInt(void)
{
    OS_CPU_SR  cpu_sr = 0u;
	if(OSRunning == OS_UN_RUNNING)
			return;
    OS_ENTER_CRITICAL();
    if (OSIntNesting > 0u)                    
        --OSIntNesting;
    if(OSIntNesting==0u){
        if(OSCtxLockNesting==0u){
            OSGetTargetSwThread();
            if(CurrentRunThread != ThreadSwtTarget)
                OS_ThreadIntSWt();
        }
    }
    OS_EXIT_CRITICAL(); 
}

void OSTDS_Sched(void)
{   
    OS_CPU_SR  cpu_sr = 0u;
    OS_ENTER_CRITICAL();
    if(OSIntNesting!=0u||OSGetTargetSwThread()!=OS_GET_CTXTARGET_SUCCESS
        ||OSCtxLockNesting!=0u){   
        OS_EXIT_CRITICAL();
        return;
    }
	OSGetTargetSwThread();
	if(CurrentRunThread != ThreadSwtTarget)
		OS_ThreadSwt();
    OS_EXIT_CRITICAL();
}