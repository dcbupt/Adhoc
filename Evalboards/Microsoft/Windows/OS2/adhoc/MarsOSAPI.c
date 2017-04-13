#include "MarsOSAPI.h"

//自定义的映射任务优先级和负责管理消息队列的OS_EVENT的数组
static OS_EVENT* prio_osevent[OS_LOWEST_PRIO+1];

//任务管理相关API
INT8U MARSTaskCreate(void(*task)(void *p_arg), void*p_arg, MARS_STK *ptos, INT8U prio, INT16U max_ipc){
	//需要注意的是这里用来申请存储消息指针的mes_queue用的是malloc，暂未考虑使用ucos的内存管理方法
	INT8U err;
	void ** mes_queue;
	err=OSTaskCreate(task,p_arg,ptos,prio);
	if (err == OS_ERR_NONE){
		mes_queue = malloc(sizeof(void*)*max_ipc);
		prio_osevent[prio] = OSQCreate(mes_queue, max_ipc);
	}
	return err;
};
INT8U MARSTaskCreateExt(void(*task)(void *p_arg), void *p_arg, MARS_STK *ptos, INT8U prio, INT16U id, MARS_STK *pbos, INT32U stk_size, void *pext, INT16U max_ipc, INT16U opt){
	//需要注意的是这里用来申请存储消息指针的mes_queue用的是malloc，暂未考虑使用ucos的内存管理方法
	INT8U err;
	void ** mes_queue;
	err=OSTaskCreateExt(task,p_arg,ptos,prio,id,pbos,stk_size,pext,opt);
	if (err == OS_ERR_NONE){
		mes_queue = malloc(sizeof(void*)*max_ipc);
		prio_osevent[prio] = OSQCreate(mes_queue,max_ipc);
	}
	return err;
}
INT8U MARSTaskDel(INT8U prio){
	return OSTaskDel(prio);
}
INT8U MARSTaskDelReq(INT8U prio){
	return OSTaskDelReq(prio);
}
INT8U MARSTaskResume(INT8U prio){
	return OSTaskResume(prio);
}
INT8U MARSTaskSuspend(INT8U prio){
	return OSTaskSuspend(prio);
}
INT8U MARSTaskChangePrio(INT8U oldprio, INT8U newprio){
	//暂未实现，想法是使用OSTCBPrioTbl,注意任务优先级发生变化，映射任务优先级和OS_EVENT的数组也需要发生变化
	printf("MARSTaskChangePrio 暂未实现");
	getchar();
	return 0;
}
INT8U MARSTaskStkChk(INT8U prio, MARS_STK_DATA*p_stk_data){
	//暂未实现，想法是使用OSTCBPrioTbl
	printf("MARSTaskStkChk 暂未实现");
	getchar();
	return 0;
}
INT8U MARSTaskQuery(INT8U prio, MARS_TCB *p_task_data){
	//暂未实现，想法是使用OSTCBPrioTBL
	printf("MARSTaskQuery 暂未实现");
	getchar();
	return 0;
}
INT8U MARSTaskNameGet(INT8U prio, INT8U *pname, INT8U *perr){
	//暂未实现，想法是使用OSTCBPrioTBL
	printf("MARSTaskNameGet 暂未实现");
	getchar();
	return 0;
}
void  MARSTaskNameSet(INT8U prio, INT8U *panme, INT8U *perr) {
	//暂未实现，想法是使用OSTCBPrioTBL
	printf("MARSTaskNameSet 暂未实现");
	getchar();
}
void  MARSStatInit(void){
	OSStatInit();
}

//任务间通讯相关API
INT8U MARSIPCSend(MARS_IPC *pipc, INT16U timeout){
	//timeout参数暂未使用，具体实现方式待定
	INT8U prio = pipc->MARSIDRecv;
	OS_EVENT * os_event = prio_osevent[prio];
	INT8U err;
	err = OSQPost(os_event, pipc);
	return err;
}
INT8U MARSIPCRecv(MARS_IPC **ppipc, INT16U timeout){
	INT8U prio = OSTCBCur->OSTCBPrio;
	OS_EVENT *os_event = prio_osevent[prio];
	INT8U err;
	*ppipc = OSQPend(os_event,timeout,&err);
	return err;
}
MARS_IPC *MARSIPCGet(void){
	//需要注意这里申请MARS_IPC的地址用的是malloc，暂未考虑ucos内存管理
	MARS_IPC * mars_ipc = malloc(sizeof(MARS_IPC));
	return mars_ipc;
}
void  MARSIPCPut(MARS_IPC *pipc){
	//需要注意释放MARS_IPC的地址用的是free,暂未考虑使用ucos内存管理
	free(pipc);
}
INT8U MARSIPCMapRegist(MARS_IPCMAP ipcmap, INT16U typejob){
	//暂未实现，具体实现方式待定
	printf("MARSIPCMapRegist 暂未实现");
	getchar();
	return 0;
}

//时间管理
void  MARSTimeDly(INT16U ticks){
	OSTimeDly(ticks);
}
INT8U MARSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U ms){
	return OSTimeDlyHMSM(hours,minutes,seconds,ms);
}
INT8U MARSTimeDlyResume(INT8U prio){
	return OSTimeDlyResume(prio);
}
INT32U MARSTimeGet(void){
	return OSTimeGet();
}
void  MARSTimeSet(INT32U ticks){
	OSTimeSet(ticks);
}

//内存管理
MARS_MEM *MARSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *perr){
	return OSMemCreate(addr,nblks,blksize,perr);
}
void *MARSMemGet(MARS_MEM *pmem, INT8U *perr){
	return OSMemGet(pmem,perr);
}
INT8U MARSMemPut(MARS_MEM *pmem, void *pblk){
	return OSMemPut(pmem,pblk);
}
INT8U MARSMemQuery(MARS_MEM *pmem, MARS_MEM_DATA *p_mem_data){
	return OSMemQuery(pmem,p_mem_data);
}
INT8U MARSMemNameGet(MARS_MEM *pmem, INT8U *pname, INT8U *perr){
	//暂未实现，考虑自定义结构体，将内存控制块和名字绑定起来
	printf("MARSMemNameGet 暂未实现");
	getchar();
	return 0;
}
void  MARSMemNameSet(MARS_MEM *pmem, INT8U *pname, INT8U *perr){
	//暂未实现，考虑按上述方法，实现
	printf("MARSMemNameSet 暂未实现");
	getchar();
}

//中断管理
void MARSIntEnter(void){
	OSIntEnter();
}
void MARSIntExit(void){
	OSIntExit();
}


//其他功能
void MARSStart(void){
	OSStart();
}
void MARSInit(void){
	OSInit();
}