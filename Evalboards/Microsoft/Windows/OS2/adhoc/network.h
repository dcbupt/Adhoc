#ifndef _NETWORK_LAYRE_H_
#define _NETWORK_LAYER_H_

#include "routeTable.h"
#include "vardef.h"
#include "packetBuffer.h"

#define NUM_OF_NODES 10

#define UNDEFINED   1000
#define DOUBLE_INFINITY  0x7fffffff

#define OFFSET_TO_NETLAYER  3		//TODO 根据具体的情况而定 

/*
route_table_map  rt_table;			//全局路由表

connect_info_t  connect_matrix[NUM_OF_NODES][NUM_OF_NODES];		//全局全连通矩阵

packet_queue packet_buffer_queue;		//全局的数据包缓存队列  TODO 需要初始化

INT8U blocked_queue_flag;			//缓存队列是否阻塞标志
*/


//网络层报文头部结构体
typedef struct Netlayer_Hdr {
	INT8U source_ID;			//源地址
	INT8U dest_ID;				//目标地址
	INT16U dataLength;			//数据段的长度
	INT8U  TTL;					//数据包的生存期，Time_To_Live（跳数计）
	INT8U  padding;				//保留位
} netlayer_hdr;

/**
  *	根据全连通矩阵计算路由
  *
  */
void computeRouteTable();

/**
  *	根据全连通矩阵计算路由（不考虑LET）
  *
  */
void computeRouteTableWithoutLET();

/**
  * 处理数据的发送和转发
  * @param destID: 数据发往的目标地址
  * @point2packet: 指向链路层头部
  * @length: 从链路层算起的长度
  */
void packetProcess(INT8U destID, void* point2packet, INT32U length);


/** 
  * 处理物理层上来的数据包
  * @param point2packet 数据包地址指针
  * @param length  数据包长度
  *
  */
void receiveFromMac(void *point2packet, INT32U length);




	#ifdef DEBUG_NET

	INT8U getNodeAddr();

	void send_to_app(void *point2packet, INT32U length);

	#endif


#endif