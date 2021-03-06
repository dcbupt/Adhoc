#include "includes.h"
#include "timer.h"
#include "event_type.h"
#include "mes_queue.h"
#include "network.h"
#include "log.h"
#include "MarsOSAPI.h"

MARS_STK Timer_Stk[TIMER_NUMBER][TASK_STK_SIZE];
TIMER_INFO TIMER_CON[TIMER_NUMBER];
INT8U TIMER_UESD[TIMER_NUMBER]={0};
//设置定时器
void set_timer(INT16U time,INT8U no){
	int i;
	for(i=0;i<TIMER_NUMBER;i++){
		if(TIMER_UESD[i]==0)
			break;
	}
	TIMER_UESD[i]=1;
	TIMER_CON[i].no=no;
	TIMER_CON[i].prior=10+i;
	TIMER_CON[i].time=time;

	MARSTaskCreate(new_timer,&TIMER_CON[i],&Timer_Stk[i][TASK_STK_SIZE-1],10+i,0);		
}

//完成计时任务
void new_timer(void *pdata){
	TIMER_INFO timer_info=*((TIMER_INFO*)pdata);
	
	MARSTimeDly(timer_info.time);

	if (timer_info.no == 1)
	{
		sendMessageToProtocol(InitTimerEvent,0,0,0,NULL);
	}
	else if (timer_info.no == 20)
	{
		sendMessageToProtocol(DataTimeUpEvent, 0, 0, 0, NULL);
	}
	else if (timer_info.no == 21)
	{
		sendMessageToProtocol(AckTimeUpEvent, 0, 0, 0, NULL);
	}
	else
	{
		sendMessageToProtocol(TimeUpEvent,0,timer_info.no-10,0,NULL);
	}

	TIMER_UESD[timer_info.prior - 10] = 0;
	MARSTaskDel(timer_info.prior);
	while(1){

	}
}
//取消定时器
void cancle_timer(INT8U no){
	INT8U i;

	for(i=0;i<TIMER_NUMBER;i++){
		if(TIMER_UESD[i]==1&&TIMER_CON[i].no==no)
			break;
	}
	if(i>=TIMER_NUMBER){
		PrintfLog(LOG_ERROR, "no such timer\n");
		return;
	}
	else{
		TIMER_UESD[i]=0;
		MARSTaskDel(TIMER_CON[i].prior);
		PrintfLog(LOG_DEBUG,"Cancle new timer %d\n",TIMER_CON[i].prior);
	}
}