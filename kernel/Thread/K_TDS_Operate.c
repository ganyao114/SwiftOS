#include "K_TDS_Operate.h"
#include "sys.h"
#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif

BOOLEAN TDS_Insert(OS_TDS *tds,OS_TDS * tab[]){
	INT8U prio = tds->TDS_Prio;
	OS_TDS *tmptds = tab[prio];
	if(tmptds == NULL){
		tab[prio] = tds;
		tds->TDS_Pre = NULL;
		tds->TDS_Next = NULL;
		goto insten;
	}
	
	while(tmptds->TDS_Next != NULL){
		tmptds = tmptds->TDS_Next;
	}
	
	tmptds->TDS_Next = tds;
	tds->TDS_Pre = tmptds;
	tds->TDS_Next = NULL;
	
insten:
	return TRUE;
}

BOOLEAN TDS_Delete(OS_TDS *tds,OS_TDS * tab[]){
	INT8U prio = tds->TDS_Prio;
	if(tds->TDS_Pre == NULL){
		tab[prio] = tds->TDS_Next;
	}else{
		tds->TDS_Pre->TDS_Next = tds->TDS_Next;
	}
	if(tds->TDS_Next != NULL){
		tds->TDS_Next->TDS_Pre = tds->TDS_Pre;
	}
	return TRUE;
}

BOOLEAN TDS_ChangeTab(OS_TDS *tds,BOOLEAN tab){
	if(tab == TDS_INTO_WAIT){
		TDS_Delete(tds,OSTDSRdyList);
		TDS_Insert(tds,OSTDSWaitList);
	}else{
		TDS_Delete(tds,OSTDSWaitList);
		TDS_Insert(tds,OSTDSRdyList);
	}
}

BOOLEAN TDS_Update_prio(){
	int i;
	for(i = 0;i < OS_MAX_TDS_LV;i++){
		if(OSTDSRdyList[i] != NULL){
			OS_HighestPrio = i;
			break;
		}
	}
	for(i = 0;i < OS_MAX_TDS_LV;i++){
		if(OSTDSWaitList[i] != NULL){
			OS_HighestWaitPrio = i;
			break;
		}
	}
	return TRUE;
}

BOOLEAN TDS_InsertFifo(OS_TDS *tds,OS_TDS **tar,INT8U select){
	if(*tar == NULL){
		*tar = tds;
		return TRUE;
	}
	if(select == TDS_INTO_CHANGEPRIO){
		while((*tar)->ChangePrio_Next!=NULL){
			*tar = (*tar)->ChangePrio_Next;
		}
		(*tar)->ChangePrio_Next = tds;
		tds->ChangePrio_Pre = *tar;
	}else{
		while((*tar)->PendResume_Next!=NULL){
			*tar = (*tar)->PendResume_Next;
		}
		(*tar)->PendResume_Next = tds;
		tds->PendResume_Pre = *tar;
	}
	return TRUE;
}

BOOLEAN TDS_DeleteFifo(OS_TDS *tds,OS_TDS **tar,INT8U select){
	if(select == TDS_INTO_CHANGEPRIO){
		if(tds->ChangePrio_Pre == NULL){
			*tar = tds->ChangePrio_Next;
		}else{
			tds->ChangePrio_Pre->ChangePrio_Next = tds->ChangePrio_Next;
		}
		if(tds->ChangePrio_Next != NULL){
			tds->ChangePrio_Next->ChangePrio_Pre = tds->ChangePrio_Pre;
		}
	}else{
		if(tds->PendResume_Pre == NULL){
			*tar = tds->PendResume_Next;
		}else{
			tds->PendResume_Pre->PendResume_Next = tds->PendResume_Next;
		}
		if(tds->PendResume_Next != NULL){
			tds->PendResume_Next->PendResume_Pre = tds->PendResume_Pre;
		}
	}
	tds->ChangePrio_Next = NULL;
	tds->ChangePrio_Pre = NULL;
	tds->PendResume_Next = NULL;
	tds->PendResume_Pre = NULL;
	return TRUE;
}