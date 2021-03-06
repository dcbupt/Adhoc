#include "mes_queue.h"

/*包含自己实现的MarsOSAPI,仅PC上仿真需要，移植时不需要*/
#include "MarsOSAPI.h"
/******************************************************/

#include "log.h"

//往消息队列里面写入消息
void sendMessageToProtocol(INT8U mesType, INT32U len, INT8U desID, INT8U memType, void *data){
	MARS_IPC *pIPC = MARSIPCGet();
	Protocol_Message * protocolMessage = NULL;
	INT8U err=10;

	if (pIPC != NULL){
		pIPC->MARSIDRecv = Protocol_Prio;
		pIPC->MARSMsgLen = (INT16U)sizeof(Protocol_Message);

		protocolMessage = (Protocol_Message*)pIPC->MARSMsgBuf;
		protocolMessage->mesType = mesType;
		protocolMessage->len = len;
		protocolMessage->desID = desID;
		protocolMessage->memType = memType;
		protocolMessage->data = data;

		err = MARSIPCSend(pIPC, 10);
		switch (err){
			case MARS_ERR_IPC_TYPE_OVERFLOW:
				//PrintfLog(LOG_ERROR, "Time:%d,send Message to Protocol failed,the Protocol IPC is full.\n", MARSTimeGet());
				break;
			case MARS_ERR_NONE:
				//PrintfLog(LOG_DEBUG, "Time:%d,send Message to Protocol succeed.\n", MARSTimeGet());
				break;
			default:
				//PrintfLog(LOG_ERROR, "Time:%d,send Message to Protocol failed,the wrong no is %d.\n", MARSTimeGet(), err);
				break;
		}
	}
	else{
		//PrintfLog(LOG_ERROR,"Get pIPC failed!\n");
	}
}