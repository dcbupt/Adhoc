#ifndef MARSOSAPI_H_
#define MARSOSAPI_H_

#include "includes.h"

//使用到的错误码定义
#define MARS_ERR_NONE OS_ERR_NONE
#define MARS_ERR_IPC_TYPE_OVERFLOW OS_ERR_Q_FULL

//任务管理相关API
#define MARS_STK OS_STK
#define MARS_TCB OS_TCB

typedef struct MARS_stk_data{
	INT32U MARSFree;
	INT32U MARSUsed;
}MARS_STK_DATA;

INT8U MARSTaskCreate(void (*task)(void *p_arg),void*p_arg,MARS_STK *ptos,INT8U prio,INT16U max_ipc);
INT8U MARSTaskCreateExt(void (*task)(void *p_arg),void *p_arg,MARS_STK *ptos,INT8U prio,INT16U id,MARS_STK *pbos,INT32U stk_size,void *pext,INT16U max_ipc,INT16U opt);
INT8U MARSTaskDel(INT8U prio);
INT8U MARSTaskDelReq(INT8U prio);
INT8U MARSTaskResume(INT8U prio);
INT8U MARSTaskSuspend(INT8U prio);
INT8U MARSTaskChangePrio(INT8U oldprio,INT8U newprio);
INT8U MARSTaskStkChk(INT8U prio,MARS_STK_DATA*p_stk_data);
INT8U MARSTaskQuery(INT8U prio,MARS_TCB *p_task_data);
INT8U MARSTaskNameGet(INT8U prio,INT8U *pname,INT8U *perr);
void  MARSTaskNameSet(INT8U prio,INT8U *panme,INT8U *perr);
void  MARSStatInit(void);

//任务间通讯相关API
#define MARS_IPC_SIZE 1024

typedef struct MARS_ipc{
	INT32U MARSIDSend;
	INT32U MARSIDRecv;
	INT16U MARSMsgLen;
	INT8U  MARS_RealPriMARSend;
	INT8U  MARS_RealPrioRecv;
	INT8U  MARSMsgBuf[MARS_IPC_SIZE];
}MARS_IPC;
typedef INT8U(*MARS_IPCMAP)(MARS_IPC*);

INT8U MARSIPCSend(MARS_IPC *pipc, INT16U timeout);
INT8U MARSIPCRecv(MARS_IPC **ppipc, INT16U timeout);
MARS_IPC *MARSIPCGet(void);
void  MARSIPCPut(MARS_IPC *pipc);
INT8U MARSIPCMapRegist(MARS_IPCMAP ipcmap,INT16U typejob);

//时间管理
void  MARSTimeDly(INT16U ticks);
INT8U MARSTimeDlyHMSM(INT8U hours,INT8U minutes,INT8U seconds,INT16U ms);
INT8U MARSTimeDlyResume(INT8U prio);
INT32U MARSTimeGet(void);
void  MARSTimeSet(INT32U ticks);

//内存管理
#define MARS_MEM OS_MEM
#define MARS_MEM_DATA OS_MEM_DATA

MARS_MEM *MARSMemCreate(void *addr,INT32U nblks,INT32U blksize,INT8U *perr);
void *MARSMemGet(MARS_MEM *pmem,INT8U *perr);
INT8U MARSMemPut(MARS_MEM *pmem,void *pblk);
INT8U MARSMemQuery(MARS_MEM *pmem,MARS_MEM_DATA *p_mem_data);
INT8U MARSMemNameGet(MARS_MEM *pmem,INT8U *pname,INT8U *perr);
void  MARSMemNameSet(MARS_MEM *pmem,INT8U *pname,INT8U *perr);

//中断管理
#define MARS_ENTER_CRITICAL() OS_ENTER_CRITICAL()
#define MARS_EXIT_CRITICAL()  OS_EXIT_CRITICAL()
void MARSIntEnter(void);
void MARSIntExit(void);


//其他功能
void MARSStart(void);
void MARSInit(void);
#endif