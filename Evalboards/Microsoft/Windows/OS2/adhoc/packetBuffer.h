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


//数据包的封装结构
typedef struct packet {
	INT8U nextID;			//该数据包的下一条地址
	INT8U destID;
	INT8U *data;			//数据包的地址指针
	INT16U data_length;		//数据包的长度
	struct packet* next;	//指向下一节点的指针
} packet;


//链表结构的队列，出队后的元素空间由程序员负责释放
typedef struct packet_queue {
	packet* head;			//头结点
	packet* tail;			//尾节点
	INT16U m_size;			//队列当前大小
	INT16U m_capacity;		//队列总容量
	FP64 m_blocked_factor;	//队列阻塞的阀值因子，ratio=当前大小/队列总容量
	FP64 m_clear_factor;	//清除队列阻塞标志的阀值因子，ratio=当前大小/队列总容量
} packet_queue;

/** 初始化数据包缓存队列，队列使用前必须初始化操作
  * @param queue 队列指针
  * @param capacity 队列容量
  * @param blocked_factor 队列阻塞因子
  * @param clear_factor 清除队列阻塞标志的因子
  *
  */
void init_packet_queue(packet_queue *queue, INT16U capacity, FP64 blocked_factor, FP64 clear_factor);

/** 新数据包入队
  * @param queue 队列指针
  * @param data  数据包指针，指向动态内存块的指针
  *
  */
void enqueue(packet_queue *queue, packet *data);

/** 数据包出队
  * @param queue 队列指针
  * @return 返回队列头部指针，该指针需要调用程序负责释放空间
  *
  */
packet* dequeue(packet_queue *queue);

/** 读取头部节点
  * @param queue 读取头部节点，但是没有出队，仍在队列中
  * @return 返回队列中的头结点的指针，一般用于预读操作
  *
  */
packet* read_head(packet_queue *queue);

/** 判断清除Blocked标志的函数, 1表示需要清除，0表示不需要清除
  * @param queue 队列指针
  * @return ratio=当前大小/队列总容量，如果ratio小于packet_queue->m_clear_factor，则返回1，说明
            需要清空阻塞标志位；否则返回0，不需要；
  *
  */
INT8U is_clear_blocked_flag(packet_queue *queue);


/** 判断队列是否为空，为空返回1，不为空返回0
  * @param queue 队列指针
  * @return 如果队列为空，返回1；否则，返回0
  *
  */
INT8U is_empty(packet_queue *queue);

/** 判断队列是否达到容限，是返回1，否返回0
  * @param queue 队列指针
  * @return ratio=当前大小/队列总容量，如果ratio大于packet_queue->m_blocked_factor，则返回1，说明
            需要设置阻塞标志位；否则返回0，不需要；
  *
  */
INT8U is_blocked(packet_queue *queue);

/** 判断队列是否满，是返回1，否返回0
  * @param queue 队列指针
  * @return 如果队列满，返回1；否则，返回0
  *
  */
INT8U is_full(packet_queue *queue);

/** 返回队列当前大小
  * @param queue 队列指针
  * @return 返回队列当前大小
  *
  */
INT16U queue_size(packet_queue *queue);


#endif