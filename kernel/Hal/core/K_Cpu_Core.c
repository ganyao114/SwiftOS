//#include "OS_CPU_CORE.h"
#ifndef  OS_MASTER_FILE
#include "SwiftOS.h"
#endif
OS_STK *ThreadSTKInt (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt)
{
    OS_STK *stk;


    (void)opt;                                   /* 'opt' is not used, prevent warning                 */
    stk       = ptos;                            /* Load stack pointer                                 */

                                                 /* Registers stacked as if auto-saved on exception    */
    *(stk)    = (INT32U)0x01000000L;             /* xPSR                                               */
    *(--stk)  = (INT32U)task;                    /* Entry Point                                        */
    *(--stk)  = (INT32U)0xFFFFFFFEL;             /* R14 (LR) (init value will cause fault if ever used)*/
    *(--stk)  = (INT32U)0x12121212L;             /* R12                                                */
    *(--stk)  = (INT32U)0x03030303L;             /* R3                                                 */
    *(--stk)  = (INT32U)0x02020202L;             /* R2                                                 */
    *(--stk)  = (INT32U)0x01010101L;             /* R1                                                 */
    *(--stk)  = (INT32U)p_arg;                   /* R0 : argument                                      */

                                                 /* Remaining registers saved on process stack         */
    *(--stk)  = (INT32U)0x11111111L;             /* R11                                                */
    *(--stk)  = (INT32U)0x10101010L;             /* R10                                                */
    *(--stk)  = (INT32U)0x09090909L;             /* R9                                                 */
    *(--stk)  = (INT32U)0x08080808L;             /* R8                                                 */
    *(--stk)  = (INT32U)0x07070707L;             /* R7                                                 */
    *(--stk)  = (INT32U)0x06060606L;             /* R6                                                 */
    *(--stk)  = (INT32U)0x05050505L;             /* R5                                                 */
    *(--stk)  = (INT32U)0x04040404L;             /* R4                                                 */

    return (stk);
}
