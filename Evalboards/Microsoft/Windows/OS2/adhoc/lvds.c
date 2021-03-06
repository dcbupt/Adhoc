#include "lvds.h"
#include "event_type.h"
#include "mem_man.h"
#include "mes_queue.h"
#include "vardef.h"
#include "log.h"
#include "send_mes.h"
#include "MarsOSAPI.h"
#include "mac_help.h"
#include "packetBuffer.h"
#include "publicstorage.h"

extern void updateNodeInfo(INT8U no, FP32 x, FP32 y, FP32 z, INT16U x_speed, INT16U y_speed, INT16U z_speed, INT16U time);
extern move_info node_move[NUM_OF_NODES];
extern speed_info node_speed[NUM_OF_NODES];
extern INT32U current_time;
extern INT32U THIS_NODE;
extern packet_queue packet_buffer_queue;

void send_to_lvds(INT8U* header,INT32U header_len,INT8U* data,INT32U data_len){
	FILE * fp;
	fp=fopen("SendToLvdsCache.txt","wb");
	fwrite(header,sizeof(INT8U),header_len,fp);
	fwrite(data,sizeof(INT8U),data_len,fp);
	fclose(fp);
	system("SendLvdsData.exe");
	remove("SendToLvdsCache.txt");
}

INT8U lvds_has_data(){
	FILE * fp;
	system("GetLvdsData.exe");
	MARSTimeDly(1);
	fp=fopen("GetFromLvdsCache.txt","rb");
	if(fp==NULL)
		return 0;
	fclose(fp);
	return 1;
}

INT32U get_from_lvds(INT8U* data){
	FILE * fp;
	INT32U actual_len;
	
	fp=fopen("GetFromLvdsCache.txt","rb");
	if(fp==NULL)
		return 0;
	actual_len = fread(data, sizeof(INT8U), Max_LVDS_LEN, fp);
	fclose(fp);
	remove("GetFromLvdsCache.txt");
	return actual_len;
}

//接收天线的处理方式，由原来的handle_lvds_data取出来，更改为统一的放入data里面，由相应的针对事件处理的函数取出来处理
//内存释放，需要注意根据消息里面内存区域的类型来释放
void handle_lvds_data(INT8U *data_from_lvds,INT32U len){
	FP32 x, y;
	INT16U z;
	INT16U vx, vy, vz;
	INT32U time_info;
	void *package;

	INT8U desID;
	
	INT8U type = data_from_lvds[0];
	//lvds协议里面数据长度部分表示方式暂定下面这种方式
	INT32U data_len = data_from_lvds[1] * 256 * 256 + data_from_lvds[2] * 256 + data_from_lvds[3];
	//目前针对所有的lvds数据部分全部采用DatapackageMemType
	void *data;

	if (type == 0xB2)//宽波束设置响应
	{
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is SetWideBeamEvent\n");
		data = mem_Malloc(SmallAmoutOfDataMemType);
		if (data != NULL)
			memcpy(data, data_from_lvds + 4, data_len);
		sendMessageToProtocol(SetWideBeamEvent, data_len, 0, SmallAmoutOfDataMemType, data);
	}
	else if (type == 0xB3)//窄波束设置响应
	{
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is SetNarrowBeamEvent\n");
		data = mem_Malloc(SmallAmoutOfDataMemType);
		if (data != NULL)
			memcpy(data, data_from_lvds + 4, data_len);
		sendMessageToProtocol(SetNarrowBeamEvent, data_len, 0, SmallAmoutOfDataMemType, data);
	}
	else if (type == 0xB7)//链路参数设置响应
	{
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is SetLinkEvent\n");
		data = mem_Malloc(SmallAmoutOfDataMemType);
		if (data != NULL)
			memcpy(data, data_from_lvds + 4, data_len);
		sendMessageToProtocol(SetLinkEvent, data_len, 0, SmallAmoutOfDataMemType, data);
	}
	else if (type == 0xD1)//宽波束数据接收指示
	{
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is Phy2MacOnWideEvent\n");
		data = mem_Malloc(SmallAmoutOfDataMemType);
		if (data != NULL)
			memcpy(data, data_from_lvds + 4, data_len);
		sendMessageToProtocol(Phy2MacOnWideEvent, data_len, 0, SmallAmoutOfDataMemType, data);
	}	
	else  if (type == 0xD2)//窄波束模式数据接收指示
	{
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is Phy2MacOnNarrowEvent\n");
		data = mem_Malloc(DatapackageMemType);
		if (data != NULL)
			memcpy(data, data_from_lvds + 4, data_len);
		sendMessageToProtocol(Phy2MacOnNarrowEvent, data_len, 0, DatapackageMemType, data);
	}
	else if (type == 0x01)//控制终端发送应用层消息
	{
		desID = (INT8U)data_from_lvds[1];
		data_len = (INT32U)((INT8U)data_from_lvds[2] * 256 * 256 + (INT8U)data_from_lvds[3] * 256 + (INT8U)data_from_lvds[4]);
		appSendData(desID, data_len, &data_from_lvds[5]);
		PrintfLog(LOG_DEBUG, "Receive lvds data,type is apppSendData, desID %d data_len %d \n", desID, data_len);
	}
	else if (type == 0x00)//收到位置消息&速度&时间
	{
		memcpy(&x,&data_from_lvds[1],4);
		memcpy(&y,&data_from_lvds[5],4);
		memcpy(&z,&data_from_lvds[9],2);

		memcpy(&vx, &data_from_lvds[11], 2);
		memcpy(&vy, &data_from_lvds[13], 2);
		memcpy(&vz, &data_from_lvds[15], 2);

		memcpy(&time_info, &data_from_lvds[17], 3);

		updateNodeInfo(THIS_NODE, x, y, z, vx, vy, vz, 0);
		current_time = time_info;
	}
	else{
		PrintfLog(LOG_ERROR, "Receive lvds data,type is unknown.Data is %s.\n", data_from_lvds);
	}
}