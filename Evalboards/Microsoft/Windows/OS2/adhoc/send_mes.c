#include "send_mes.h"
#include "mes_queue.h"
#include "mem_man.h"
#include "event_type.h"
#include "log.h"

//应用层需要发送数据的时候调用
/*
参数定义：
desID 应用层指定的目的节点
len 应用要发送的数据的长度
data 指向应用层要发送的数据的指针
*/
void appSendData(INT8U desID, INT32U len, void* data){
	void *package;

	PrintfLog(LOG_DEBUG, "App layer send data to protocol.\n");
	package = mem_Malloc(DatapackageMemType);
	if (package != NULL){	
		memcpy((void*)((INT32U)package + NetAndMacHeaderLen), data, len);
		sendMessageToProtocol(App2NetEvent, len + NetAndMacHeaderLen, desID, DatapackageMemType, package);
	}		
}
