#ifndef _OSTDS_OPERATE_
#define _OSTDS_OPERATE_
#include "K_Cpu.h"
#include "K_Thread.h"

#define TDS_INTO_RDY 0
#define TDS_INTO_WAIT 1

#define TDS_INTO_CHANGEPRIO 0u
#define TDS_INTO_PENDRESUME 1u

BOOLEAN TDS_Insert(OS_TDS *tds,OS_TDS * tab[]);
BOOLEAN TDS_Delete(OS_TDS *tds,OS_TDS * tab[]);
BOOLEAN TDS_ChangeTab(OS_TDS *tds,BOOLEAN tab);
BOOLEAN TDS_ChangePrio(OS_TDS *tds,OS_TDS * tab[],INT8U tarprio);
BOOLEAN TDS_Update_prio();
BOOLEAN TDS_InsertFifo(OS_TDS *tds,OS_TDS **tar,INT8U select);
BOOLEAN TDS_DeleteFifo(OS_TDS *tds,OS_TDS **tar,INT8U select);

#endif