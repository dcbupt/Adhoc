#ifndef MAC_LAYER_H
#define MAC_LAYER_H

//#include "types.h"
#include   <os_cpu.h>


void send_node_msg();


void send_buf_packet();

/*
 * packet  ָ�����������Ϣ���ݲ���
 * memType ���տ�����Ϣ������ڴ�����
 */
void read_node_msg(INT8U* packet, INT8U memType);

/*
 * packet:ָ��խ����������Ϣ�����ݲ���
 * pkt_len:խ����������Ϣ���ݲ��ֳ���
 */
void read_narrowBeam_msg(INT8U* packet, INT16U pkt_len);

/*
 * data:ָ��խ����������Ӧ�����ݲ���
 * memType:����խ����������Ӧ������ڴ�����
 * data_len:���ݲ��ֳ���
 */
void read_setNarrowBeamRsp(void* data, INT8U memType, INT32U data_len);

/*
 * data:ָ����·����������Ӧ�����ݲ���
 * memType:������·����������Ӧ������ڴ�����
 * data_len:���ݲ��ֳ���
 */
void read_setLinkRsp(void* data, INT8U memType, INT32U data_len);

/*
 * data:ָ�����������Ӧ�����ݲ���
 * memType:���տ���������Ӧ������ڴ�����
 * data_len:���ݲ��ֳ���
 */
void read_setWideBeamRsp(void* data, INT8U memType, INT32U data_len);


void set_node_msg_timer();


void quit_connect_matrix(INT8U neighbourID);

void dataTimeUp_Process();

void ackTimeUp_Process();
#endif
