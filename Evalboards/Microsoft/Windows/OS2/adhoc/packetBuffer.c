#include "packetBuffer.h"
#include <stddef.h>
#include <malloc.h>

//初始化数据包缓存队列
void init_packet_queue(packet_queue *queue, INT16U capacity, FP64 blocked_factor, FP64 clear_factor) {
	queue->head = queue->tail = NULL;
	queue->m_capacity = capacity;
	queue->m_blocked_factor = blocked_factor;
	queue->m_clear_factor = clear_factor;
	queue->m_size = 0;
}

//新数据包入队
//需要用户自行检测队列的当前状态是否允许入队，默认所有插入操作都是成功的
void enqueue(packet_queue *queue, packet *data) {
	//队列为空时
	if (queue->head == NULL && queue->tail == NULL) {
		queue->head = queue->tail = data;
		queue->m_size++;
		return;
	}
	
	queue->tail->next = data;
	queue->tail = data;

	queue->m_size++;
	
}

//数据包出队
//入队前自行检测队列是否为空，默认当前队列不为空
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

//判断队列是否为空，为空返回1，不为空返回0；队列空，不能出队
INT8U is_empty(packet_queue *queue) {
	if (queue->m_size == 0) {
		return EMPTY;
	} else {
		return NOT_EMPTY;
	}
}

//判断队列是否达到容限，是返回1，否返回0
//达到容限后，队列不允许再有元素入队，用于设置队列阻塞标志位
INT8U is_blocked(packet_queue *queue) {
	FP64 ratio = (FP64)queue->m_size / queue->m_capacity;
	if (ratio > queue->m_blocked_factor) {
		return BLOCKED;
	} else {
		return NOT_BLOCKED;
	}
}

//判断队列是否满，是返回1，否返回0；队列满，不能入队
INT8U is_full(packet_queue *queue) {
	if (queue->m_size == queue->m_capacity) {
		return FULL_QUEUE;
	} else {
		return NOT_FULL_QUEUE;
	}
}

//读取头部节点
packet* read_head(packet_queue *queue) {
	if (is_empty(queue)) {
		return NULL;
	} else {
		return queue->head;
	}
}

//判断清除Blocked标志的函数, 1表示需要清除，0表示不需要清除
INT8U is_clear_blocked_flag(packet_queue *queue) {
	FP64 ratio = (FP64)queue->m_size / queue->m_capacity;
	if (ratio < queue->m_clear_factor) {
		return 1;
	} else {
		return 0;
	}
}

/** 返回队列当前大小
  * @param queue 队列指针
  * @return 返回队列当前大小
  *
  */
INT16U queue_size(packet_queue *queue) {
	return queue->m_size;
}

