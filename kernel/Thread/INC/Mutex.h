#ifndef _OS_MUTEX_
#define _OS_MUTEX_
#include "K_Cpu.h"

#define MAX_MUTEX_POOL 8

#define OS_MUTEX_POOL_INT_SUCCESS 0u
#define OS_MUTEX_POOL_FULL 1u
#define OS_MUTEX_POOL_PUSHED 2u

#define MUTEX_IN_POOL TRUE
#define MUTEX_IN_STACK FALSE

#define MUTEX_UN_LOCKED 0u
#define MUTEX_LOCKED 1u


typedef struct K_Mutex{
	BOOLEAN statue;
	BOOLEAN heapOrstack;
	struct K_Mutex *Pre;
	struct K_Mutex *Next;
	void *lock;
}OS_Mutex;

typedef struct K_Mutex_Pool{
	INT16U remain;
	OS_Mutex mutexstore[MAX_MUTEX_POOL];
	OS_Mutex *mutex[MAX_MUTEX_POOL];
}OS_Mutex_Pool;

OS_Mutex *Create_Mutex(void *lock);

OS_Mutex *Get_Mutex(void *lock);

BOOLEAN _Request_Mutex_NoWait(void *lock);

void _Request_Mutex_Block(void *lock);

void _Request_Mutex_Pend(void *lock);

BOOLEAN _Release_Mutex(void *lock);

BOOLEAN Request_Mutex_NoWait(OS_Mutex *mutex);

void Request_Mutex_Block(OS_Mutex *mutex);

void Request_Mutex_Pend(OS_Mutex *mutex);

BOOLEAN Release_Mutex(OS_Mutex *mutex);

BOOLEAN Destory_Mutex(OS_Mutex *mutex);



BOOLEAN Mutex_Pool_Int();

OS_Mutex *Pop_Pool_Mutex();

BOOLEAN Push_Pool_Mutex(OS_Mutex *mutex);

#endif