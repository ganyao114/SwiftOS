#ifndef _K_CORE_
#define _K_CORE_

#define OS_GET_CTXTARGET_SUCCESS 7u

void OS_TimeTick(void);
void OSEnterInt(void);
void OSExtInt(void);
void OSTDS_Sched(void);


#endif