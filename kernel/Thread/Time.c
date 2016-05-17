#include "Time.h"
#include "K_Thread.h"
#include "K_Core.h"
#include "K_TDS_Operate.h"

#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif

void OSTimeDly(INT32U ticks){
	OS_CPU_SR  cpu_sr = 0u;
	if (OSIntNesting > 0u) {                     /* See if trying to call from an ISR                  */
        return;
    }
    if (OSCtxLockNesting > 0u) {                    /* See if called with scheduler locked                */
        return;
    }
    if (ticks > 0u) {                            /* 0 means no delay!                                  */
        OS_ENTER_CRITICAL();
        CurrentRunThread->TDS_TimeDly = ticks;
				CurrentRunThread->TDS_RunStatue = OS_TD_WAITING;
				TDS_InsertFifo(CurrentRunThread,&OS_PendFifo,TDS_INTO_PENDRESUME);
				OS_EXIT_CRITICAL();
				OSTDS_Sched();			/* Find next task to run!                             */
    }
}

