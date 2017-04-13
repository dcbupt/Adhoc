#include "packetBuffer.h"
#include <stddef.h>
#include <malloc.h>

//��ʼ�����ݰ��������
void init_packet_queue(packet_queue *queue, INT16U capacity, FP64 blocked_factor, FP64 clear_factor) {
	queue->head = queue->tail = NULL;
	queue->m_capacity = capacity;
	queue->m_blocked_factor = blocked_factor;
	queue->m_clear_factor = clear_factor;
	queue->m_size = 0;
}

//�����ݰ����
//��Ҫ�û����м����еĵ�ǰ״̬�Ƿ�������ӣ�Ĭ�����в���������ǳɹ���
void enqueue(packet_queue *queue, packet *data) {
	//����Ϊ��ʱ
	if (queue->head == NULL && queue->tail == NULL) {
		queue->head = queue->tail = data;
		queue->m_size++;
		return;
	}
	
	queue->tail->next = data;
	queue->tail = data;

	queue->m_size++;
	
}

//���ݰ�����
//���ǰ���м������Ƿ�Ϊ�գ�Ĭ�ϵ�ǰ���в�Ϊ��
packet* dequeue(packet_queue *queue) {
	packet *res = queue->head;
	if (queue->m_size == 1) {
		queue->head = queue->tail = NULL;
	} else {
		queue->head = res->next;
	}
	
	queue->m_size--;

	return res;
}

//�ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1����Ϊ�շ���0�����пգ����ܳ���
INT8U is_empty(packet_queue *queue) {
	if (queue->m_size == 0) {
		return EMPTY;
	} else {
		return NOT_EMPTY;
	}
}

//�ж϶����Ƿ�ﵽ���ޣ��Ƿ���1���񷵻�0
//�ﵽ���޺󣬶��в���������Ԫ����ӣ��������ö���������־λ
INT8U is_blocked(packet_queue *queue) {
	FP64 ratio = (FP64)queue->m_size / queue->m_capacity;
	if (ratio > queue->m_blocked_factor) {
		return BLOCKED;
	} else {
		return NOT_BLOCKED;
	}
}

//�ж϶����Ƿ������Ƿ���1���񷵻�0�����������������
INT8U is_full(packet_queue *queue) {
	if (queue->m_size == queue->m_capacity) {
		return FULL_QUEUE;
	} else {
		return NOT_FULL_QUEUE;
	}
}

//��ȡͷ���ڵ�
packet* read_head(packet_queue *queue) {
	if (is_empty(queue)) {
		return NULL;
	} else {
		return queue->head;
	}
}

//�ж����Blocked��־�ĺ���, 1��ʾ��Ҫ�����0��ʾ����Ҫ���
INT8U is_clear_blocked_flag(packet_queue *queue) {
	FP64 ratio = (FP64)queue->m_size / queue->m_capacity;
	if (ratio < queue->m_clear_factor) {
		return 1;
	} else {
		return 0;
	}
}

/** ���ض��е�ǰ��С
  * @param queue ����ָ��
  * @return ���ض��е�ǰ��С
  *
  */
INT16U queue_size(packet_queue *queue) {
	return queue->m_size;
}

