#ifndef _SWIFT_OS_
#define _SWIFT_OS_


#include "K_Cpu.h"
#include "K_CFG.h"
#include "K_Thread.h"
#include "stdlib.h"
#include "Mutex.h"

#ifdef   OS_GLOBALS
#define  OS_EXT
#else
#define  OS_EXT  extern
#endif

/******************************************************************************
*                               ���ĺ궨��
******************************************************************************/

//�����ж�
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}


//���������л���
#define OS_ThreadSwt()  OSCtxSw()
#define OS_ThreadIntSWt()   OSIntCtxSw()


//ϵͳ����״̬
#define OS_RUNNING 1u
#define OS_UN_RUNNING 0u

//ϵͳ�̵߳��Ȳ���
#define OS_REALTIME_STRATEGY 0u //ʵʱ
#define OS_TIMERR_STRATEGY 1u //ʱ��Ƭ��ת

/******************************************************************************
*                               ���ĺ궨��
******************************************************************************/



/******************************************************************************
*                               ����ȫ�ֱ���
******************************************************************************/

//ϵͳ����״̬
OS_EXT INT8U OSRunning;

//ϵͳ�̵߳��Ȳ���
OS_EXT INT8U OSStrategy;

//ϵͳ����ʱ��(��Ҫ���˳�ʼ��)
OS_EXT INT32U OSRunningTime;
//�������ȼ�������
OS_EXT OS_TDS *OSTDSRdyList[OS_MAX_TDS_LV];
//�������ȼ������
OS_EXT OS_TDS *OSTDSWaitList[OS_MAX_TDS_LV]; 

//�������е��߳�
OS_EXT OS_TDS *CurrentRunThread;
//�����л���Ŀ������
OS_EXT OS_TDS *ThreadSwtTarget;

OS_EXT OS_TDS *CurrentRunThreadNext;

//��߾������ȼ�
OS_EXT INT8U OS_HighestPrio;
//��ߵȴ����ȼ�
OS_EXT INT8U OS_HighestWaitPrio;

//���ȼ���־λ
OS_EXT INT8U OS_TDSRdyMark[OS_MAX_TDS_LV];

//��������
OS_EXT INT16U OS_THREAD_ALL;

//�ж�Ƕ�ײ���
OS_EXT INT8U OSIntNesting;

//������
OS_EXT INT8U OSCtxLockNesting;

//����������
OS_EXT OS_Mutex *OSMutexList;

//TimeTick�������
OS_EXT OS_TDS *OS_PendFifo;
OS_EXT OS_TDS *OS_ResumeFifo;
OS_EXT OS_TDS *OS_ChangePrioFifo;

/******************************************************************************
*                               ����ȫ�ֱ���
******************************************************************************/
#endif