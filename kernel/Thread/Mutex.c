#include "Mutex.h"
#include "K_Malloc.h"
#include "sys.h"
#include "K_Thread.h"

#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif

static OS_Mutex_Pool mutex_pool;

BOOLEAN Mutex_Pool_Int(){
		int i;
		for(i = 0;i < MAX_MUTEX_POOL;i++)
    {	
				mutex_pool.mutexstore[i].heapOrstack = OS_TDS_STACK;
        mutex_pool.mutex[i] = &mutex_pool.mutexstore[i];
    }
    mutex_pool.remain = MAX_MUTEX_POOL;
    return OS_MUTEX_POOL_INT_SUCCESS;
}

OS_Mutex *Pop_Pool_Mutex(){
	if(mutex_pool.remain > 0)
		return mutex_pool.mutex[--mutex_pool.remain];
	else
		return NULL;
}

BOOLEAN Push_Pool_Mutex(OS_Mutex *mutex){
	if(mutex_pool.remain > MAX_MUTEX_POOL-1)
        return OS_MUTEX_POOL_FULL;
    mutex_pool.mutex[++mutex_pool.remain] = mutex;
    return OS_MUTEX_POOL_PUSHED;
}

OS_Mutex *Create_Mutex(void *lock){
	OS_CPU_SR cpu_sr = 0u;
	OS_Mutex *mutex;
	OS_Mutex *pre;
	OS_ENTER_CRITICAL();
	mutex = Get_Mutex(lock);
	if(mutex != NULL)
		goto ctmutexend;
	mutex = Pop_Pool_Mutex();
	if(mutex == NULL)
		mutex = (OS_Mutex *)Kmalloc(sizeof(OS_Mutex));
	if(mutex == NULL)
		goto ctmutexend;
	mutex->lock = lock;
	mutex->statue = MUTEX_UN_LOCKED;
	if(OSMutexList == NULL){
		OSMutexList = mutex;
		mutex->Pre = NULL;
		mutex->Next = NULL;
		OS_EXIT_CRITICAL();
		return mutex;
	}
	while(pre->Next != NULL){
		pre = pre->Next;
	}
	pre->Next = mutex;
	mutex->Pre = pre;
	mutex->Next = NULL;
	OS_EXIT_CRITICAL();
	return mutex;
ctmutexend:
	OS_EXIT_CRITICAL();
	return NULL;
}

OS_Mutex *Get_Mutex(void *lock){
	OS_CPU_SR cpu_sr = 0u;
	OS_Mutex *mutex;
	OS_ENTER_CRITICAL();
	mutex = OSMutexList;
	while(mutex != NULL){
		if(mutex->lock == lock)
				goto getmtend;
			mutex = mutex->Next;
	}
	mutex = NULL;
getmtend:
	OS_EXIT_CRITICAL();
	return mutex;
}

BOOLEAN Request_Mutex_NoWait(OS_Mutex *mutex){
	OS_CPU_SR cpu_sr = 0u;
	if(mutex->statue == MUTEX_UN_LOCKED){
		OS_ENTER_CRITICAL();
		mutex->statue = MUTEX_LOCKED;
		OS_EXIT_CRITICAL();
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOLEAN _Request_Mutex_NoWait(void *lock){
	OS_Mutex *mutex;
	mutex = Get_Mutex(lock);
	if(mutex == NULL)
		return FALSE;
	return Request_Mutex_NoWait(lock);
}

BOOLEAN Release_Mutex(OS_Mutex *mutex){
	OS_CPU_SR cpu_sr = 0u;
	OS_ENTER_CRITICAL();
	mutex->statue = MUTEX_UN_LOCKED;
	OS_EXIT_CRITICAL();
}

void Request_Mutex_Block(OS_Mutex *mutex){
	OS_CPU_SR cpu_sr = 0u;
	while(mutex->statue == MUTEX_LOCKED){}
	OS_ENTER_CRITICAL();
	mutex->statue = MUTEX_LOCKED;
	OS_EXIT_CRITICAL();
}
