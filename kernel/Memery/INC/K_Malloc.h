#ifndef _K_MALLOC_
#define _K_MALLOC_
#include "K_Cpu.h"


void* Kmalloc(Point size);

void Kfree(void *ptr);

void* Krealloc(void *ptr,Point size);

void KMemInt(void);

INT8U KMemUsed(void);


#endif