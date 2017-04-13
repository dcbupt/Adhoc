#ifndef MAC_LAYER_H
#define MAC_LAYER_H

//#include "types.h"
#include   <os_cpu.h>


void send_node_msg();


void send_buf_packet();

/*
 * packet  指向宽波束接收消息数据部分
 * memType 接收宽波束消息申请的内存类型
 */
void read_node_msg(INT8U* packet, INT8U memType);

/*
 * packet:指向窄波束接收消息的数据部分
 * pkt_len:窄波束接收消息数据部分长度
 */
void read_narrowBeam_msg(INT8U* packet, INT16U pkt_len);

/*
 * data:指向窄波束设置响应的数据部分
 * memType:接收窄波束设置响应申请的内存类型
 * data_len:数据部分长度
 */
void read_setNarrowBeamRsp(void* data, INT8U memType, INT32U data_len);

/*
 * data:指向链路参数设置响应的数据部分
 * memType:接收链路参数设置响应申请的内存类型
 * data_len:数据部分长度
 */
void read_setLinkRsp(void* data, INT8U memType, INT32U data_len);

/*
 * data:指向宽波束设置响应的数据部分
 * memType:接收宽波束设置响应申请的内存类型
 * data_len:数据部分长度
 */
void read_setWideBeamRsp(void* data, INT8U memType, INT32U data_len);


void set_node_msg_timer();


void quit_connect_matrix(INT8U neighbourID);

void dataTimeUp_Process();

void ackTimeUp_Process();
#endif
