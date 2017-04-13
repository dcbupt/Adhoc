#ifndef _NETWORK_LAYRE_H_
#define _NETWORK_LAYER_H_

#include "routeTable.h"
#include "vardef.h"
#include "packetBuffer.h"

#define NUM_OF_NODES 10

#define UNDEFINED   1000
#define DOUBLE_INFINITY  0x7fffffff

#define OFFSET_TO_NETLAYER  3		//TODO ���ݾ����������� 

/*
route_table_map  rt_table;			//ȫ��·�ɱ�

connect_info_t  connect_matrix[NUM_OF_NODES][NUM_OF_NODES];		//ȫ��ȫ��ͨ����

packet_queue packet_buffer_queue;		//ȫ�ֵ����ݰ��������  TODO ��Ҫ��ʼ��

INT8U blocked_queue_flag;			//��������Ƿ�������־
*/


//����㱨��ͷ���ṹ��
typedef struct Netlayer_Hdr {
	INT8U source_ID;			//Դ��ַ
	INT8U dest_ID;				//Ŀ���ַ
	INT16U dataLength;			//���ݶεĳ���
	INT8U  TTL;					//���ݰ��������ڣ�Time_To_Live�������ƣ�
	INT8U  padding;				//����λ
} netlayer_hdr;

/**
  *	����ȫ��ͨ�������·��
  *
  */
void computeRouteTable();

/**
  *	����ȫ��ͨ�������·�ɣ�������LET��
  *
  */
void computeRouteTableWithoutLET();

/**
  * �������ݵķ��ͺ�ת��
  * @param destID: ���ݷ�����Ŀ���ַ
  * @point2packet: ָ����·��ͷ��
  * @length: ����·������ĳ���
  */
void packetProcess(INT8U destID, void* point2packet, INT32U length);


/** 
  * ������������������ݰ�
  * @param point2packet ���ݰ���ַָ��
  * @param length  ���ݰ�����
  *
  */
void receiveFromMac(void *point2packet, INT32U length);




	#ifdef DEBUG_NET

	INT8U getNodeAddr();

	void send_to_app(void *point2packet, INT32U length);

	#endif


#endif