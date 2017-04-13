#ifndef _H_PACKET_BUFFER_H_
#define _H_PACKET_BUFFER_H_

//#include "types.h"
#include "includes.h"

#define MAX_BUFFER_SIZE 1000


#define FULL_QUEUE		1
#define NOT_FULL_QUEUE  0

#define EMPTY		1
#define NOT_EMPTY   0

#define BLOCKED     1
#define NOT_BLOCKED 0


//���ݰ��ķ�װ�ṹ
typedef struct packet {
	INT8U nextID;			//�����ݰ�����һ����ַ
	INT8U destID;
	INT8U *data;			//���ݰ��ĵ�ַָ��
	INT16U data_length;		//���ݰ��ĳ���
	struct packet* next;	//ָ����һ�ڵ��ָ��
} packet;


//����ṹ�Ķ��У����Ӻ��Ԫ�ؿռ��ɳ���Ա�����ͷ�
typedef struct packet_queue {
	packet* head;			//ͷ���
	packet* tail;			//β�ڵ�
	INT16U m_size;			//���е�ǰ��С
	INT16U m_capacity;		//����������
	FP64 m_blocked_factor;	//���������ķ�ֵ���ӣ�ratio=��ǰ��С/����������
	FP64 m_clear_factor;	//�������������־�ķ�ֵ���ӣ�ratio=��ǰ��С/����������
} packet_queue;

/** ��ʼ�����ݰ�������У�����ʹ��ǰ�����ʼ������
  * @param queue ����ָ��
  * @param capacity ��������
  * @param blocked_factor ������������
  * @param clear_factor �������������־������
  *
  */
void init_packet_queue(packet_queue *queue, INT16U capacity, FP64 blocked_factor, FP64 clear_factor);

/** �����ݰ����
  * @param queue ����ָ��
  * @param data  ���ݰ�ָ�룬ָ��̬�ڴ���ָ��
  *
  */
void enqueue(packet_queue *queue, packet *data);

/** ���ݰ�����
  * @param queue ����ָ��
  * @return ���ض���ͷ��ָ�룬��ָ����Ҫ���ó������ͷſռ�
  *
  */
packet* dequeue(packet_queue *queue);

/** ��ȡͷ���ڵ�
  * @param queue ��ȡͷ���ڵ㣬����û�г��ӣ����ڶ�����
  * @return ���ض����е�ͷ����ָ�룬һ������Ԥ������
  *
  */
packet* read_head(packet_queue *queue);

/** �ж����Blocked��־�ĺ���, 1��ʾ��Ҫ�����0��ʾ����Ҫ���
  * @param queue ����ָ��
  * @return ratio=��ǰ��С/���������������ratioС��packet_queue->m_clear_factor���򷵻�1��˵��
            ��Ҫ���������־λ�����򷵻�0������Ҫ��
  *
  */
INT8U is_clear_blocked_flag(packet_queue *queue);


/** �ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1����Ϊ�շ���0
  * @param queue ����ָ��
  * @return �������Ϊ�գ�����1�����򣬷���0
  *
  */
INT8U is_empty(packet_queue *queue);

/** �ж϶����Ƿ�ﵽ���ޣ��Ƿ���1���񷵻�0
  * @param queue ����ָ��
  * @return ratio=��ǰ��С/���������������ratio����packet_queue->m_blocked_factor���򷵻�1��˵��
            ��Ҫ����������־λ�����򷵻�0������Ҫ��
  *
  */
INT8U is_blocked(packet_queue *queue);

/** �ж϶����Ƿ������Ƿ���1���񷵻�0
  * @param queue ����ָ��
  * @return ���������������1�����򣬷���0
  *
  */
INT8U is_full(packet_queue *queue);

/** ���ض��е�ǰ��С
  * @param queue ����ָ��
  * @return ���ض��е�ǰ��С
  *
  */
INT16U queue_size(packet_queue *queue);


#endif