#include "MarsOSAPI.h"

//�Զ����ӳ���������ȼ��͸��������Ϣ���е�OS_EVENT������
static OS_EVENT* prio_osevent[OS_LOWEST_PRIO+1];

//����������API
INT8U MARSTaskCreate(void(*task)(void *p_arg), void*p_arg, MARS_STK *ptos, INT8U prio, INT16U max_ipc){
	//��Ҫע�����������������洢��Ϣָ���mes_queue�õ���malloc����δ����ʹ��ucos���ڴ������
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
	//��Ҫע�����������������洢��Ϣָ���mes_queue�õ���malloc����δ����ʹ��ucos���ڴ������
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
	//��δʵ�֣��뷨��ʹ��OSTCBPrioTbl,ע���������ȼ������仯��ӳ���������ȼ���OS_EVENT������Ҳ��Ҫ�����仯
	printf("MARSTaskChangePrio ��δʵ��");
	getchar();
	return 0;
}
INT8U MARSTaskStkChk(INT8U prio, MARS_STK_DATA*p_stk_data){
	//��δʵ�֣��뷨��ʹ��OSTCBPrioTbl
	printf("MARSTaskStkChk ��δʵ��");
	getchar();
	return 0;
}
INT8U MARSTaskQuery(INT8U prio, MARS_TCB *p_task_data){
	//��δʵ�֣��뷨��ʹ��OSTCBPrioTBL
	printf("MARSTaskQuery ��δʵ��");
	getchar();
	return 0;
}
INT8U MARSTaskNameGet(INT8U prio, INT8U *pname, INT8U *perr){
	//��δʵ�֣��뷨��ʹ��OSTCBPrioTBL
	printf("MARSTaskNameGet ��δʵ��");
	getchar();
	return 0;
}
void  MARSTaskNameSet(INT8U prio, INT8U *panme, INT8U *perr) {
	//��δʵ�֣��뷨��ʹ��OSTCBPrioTBL
	printf("MARSTaskNameSet ��δʵ��");
	getchar();
}
void  MARSStatInit(void){
	OSStatInit();
}

//�����ͨѶ���API
INT8U MARSIPCSend(MARS_IPC *pipc, INT16U timeout){
	//timeout������δʹ�ã�����ʵ�ַ�ʽ����
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
	//��Ҫע����������MARS_IPC�ĵ�ַ�õ���malloc����δ����ucos�ڴ����
	MARS_IPC * mars_ipc = malloc(sizeof(MARS_IPC));
	return mars_ipc;
}
void  MARSIPCPut(MARS_IPC *pipc){
	//��Ҫע���ͷ�MARS_IPC�ĵ�ַ�õ���free,��δ����ʹ��ucos�ڴ����
	free(pipc);
}
INT8U MARSIPCMapRegist(MARS_IPCMAP ipcmap, INT16U typejob){
	//��δʵ�֣�����ʵ�ַ�ʽ����
	printf("MARSIPCMapRegist ��δʵ��");
	getchar();
	return 0;
}

//ʱ�����
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

//�ڴ����
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
	//��δʵ�֣������Զ���ṹ�壬���ڴ���ƿ�����ְ�����
	printf("MARSMemNameGet ��δʵ��");
	getchar();
	return 0;
}
void  MARSMemNameSet(MARS_MEM *pmem, INT8U *pname, INT8U *perr){
	//��δʵ�֣����ǰ�����������ʵ��
	printf("MARSMemNameSet ��δʵ��");
	getchar();
}

//�жϹ���
void MARSIntEnter(void){
	OSIntEnter();
}
void MARSIntExit(void){
	OSIntExit();
}


//��������
void MARSStart(void){
	OSStart();
}
void MARSInit(void){
	OSInit();
}