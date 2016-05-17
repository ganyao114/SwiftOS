#include "K_Cpu.h"
#ifndef K_CORE_CPU_H
#define K_CORE_CPU_H
OS_STK *ThreadSTKInt (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);
#endif
