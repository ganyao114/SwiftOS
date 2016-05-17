#include "K_Malloc.h"
#include "Malloc.h"

void* Kmalloc(Point size){
	mymalloc(SRAMIN, size);
}

void* Krealloc(void * ptr, Point size){
	myrealloc(SRAMIN,ptr,size);
}

void Kfree(void * ptr){
	myfree(SRAMIN,ptr);
}

void KMemInt(void){
	mem_init(SRAMIN);
}

INT8U KMemUsed(void){
	mem_perused(SRAMIN);
}