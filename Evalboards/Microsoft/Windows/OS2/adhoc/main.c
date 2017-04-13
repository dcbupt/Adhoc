/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "MarsOSAPI.h"

#include "send_mes.h"
#include "network.h"
#include "mes_queue.h"
#include "event_type.h"
#include "mem_man.h"
#include "mac_help.h"
#include "lvds.h"
#include "mac_laye.h"
#include "packetBuffer.h"
#include "init_node.h"
#include "log.h"
#include "publicstorage.h"
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 2048       /* Size of each task's stacks (# of WORDs)            */
/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/



static MARS_STK        Protocol_Stk[TASK_STK_SIZE];
static MARS_STK        App_Layer_Stk[TASK_STK_SIZE];
static MARS_STK        Recv_Data_From_LVDS_STK[TASK_STK_SIZE];

extern route_table_map  rt_table;			
extern connect_info_t  connect_matrix[NUM_OF_NODES][NUM_OF_NODES];		
extern packet_queue packet_buffer_queue;		
extern INT8U blocked_queue_flag;
extern INT32U THIS_NODE;
/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void AppLayer(void *pParam);
static void Protocol(void *pParam);
static void Recv_Data_From_LVDS(void *pParam);

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/
void main(void)
{
	 MARSInit();                                              /* Initialize uC/OS-II                      */	 
	 init_Node();
	 init_Mem();
	 init_packet_queue(&packet_buffer_queue,10,0.5,0.6);
	 variable_initialize();	
	
	 MARSTaskCreate(Protocol, (void *)0, &Protocol_Stk[TASK_STK_SIZE - 1], Protocol_Prio, 20);
	 MARSTaskCreate(Recv_Data_From_LVDS,(void *)0,&Recv_Data_From_LVDS_STK[TASK_STK_SIZE-1],22,0);
	 set_node_msg_timer();
	 MARSStart();                                            // Start multitasking                       
	
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
static void AppLayer(void *pParam){
	void * data = mem_Malloc(DatapackageMemType);
	memcpy(data,"1234567890",10);
	appSendData(7, 10, data);
	mem_Free(DatapackageMemType, data);
	while (1){
	
	}
}
static void Protocol(void *pParam){
	INT8U command[5];
	MARS_IPC *pIPC;
	Protocol_Message *message;
	while(1){
		MARSIPCRecv(&pIPC,0xFFFF);

		if (pIPC != (MARS_IPC*)0){
			message = (Protocol_Message*)pIPC->MARSMsgBuf;
			switch (message->mesType){
				case App2NetEvent:
					PrintfLog(LOG_DEBUG,"Event App2NetEvent\n");
					packetProcess(message->desID, message->data, message->len);
					break;
				case Phy2MacOnWideEvent:
					PrintfLog(LOG_DEBUG, "Event Phy2MacOnWideEvent\n");
					read_node_msg(message->data,message->memType);
					break;
				case Phy2MacOnNarrowEvent:
					PrintfLog(LOG_DEBUG, "Event Phy2MacOnNarrowEvent\n");
					read_narrowBeam_msg(message->data, message->len);
					break;
				case SetWideBeamEvent:
					PrintfLog(LOG_DEBUG, "Event SetWideBeamEvent\n");
					read_setWideBeamRsp(message->data,message->memType,message->len);
					break;
				case SetNarrowBeamEvent:
					PrintfLog(LOG_DEBUG, "Event SetNarrowBeamEvent\n");
					read_setNarrowBeamRsp(message->data,message->memType,message->len);
					break;
				case SetLinkEvent:
					PrintfLog(LOG_DEBUG, "Event SetLinkEvent\n");
					read_setLinkRsp(message->data, message->memType, message->len);
					break;
				case InitTimerEvent:
					PrintfLog(LOG_DEBUG, "Event InitTimerEvent\n");
					send_node_msg();
					break;
				case TimeUpEvent:
					PrintfLog(LOG_DEBUG, "\^^^^^node:%d timer up\^^^^^\n", message->desID);
					quit_connect_matrix(message->desID);
					break;
				case DataTimeUpEvent:
					PrintfLog(LOG_DEBUG, "wait data timeUp\n");
					dataTimeUp_Process();
					break;
				case AckTimeUpEvent:
					PrintfLog(LOG_DEBUG, "wait ack timeUp\n");
					ackTimeUp_Process();
					break;
				default:
					PrintfLog(LOG_ERROR, "Event type unknown\n");
					break;
			}
			MARSIPCPut(pIPC);
		}
	}
}
  

/*模拟从中频基带控制器里面获取数据，数据内容是lvds类型和数据*/
static void Recv_Data_From_LVDS(){
	INT8U *data_from_lvds=NULL;
	INT32U len=0;
	INT8U new_lvds_data_arrived=0;
	while(1){
		new_lvds_data_arrived=lvds_has_data();		
		if(new_lvds_data_arrived==1){
			data_from_lvds = malloc(sizeof(INT8U)*Max_LVDS_LEN);
			if (data_from_lvds != NULL){
				len = get_from_lvds(data_from_lvds);		
				if (len>0){
					handle_lvds_data(data_from_lvds, len);
				}
			}
			free(data_from_lvds);
		}
		new_lvds_data_arrived=0;
	}
}