#ifndef _H_LIST_H_
#define _H_LIST_H_

//#include "types.h"
#include "includes.h"
#include <stddef.h>

#define MAX_LIST_SIZE 256

#define LIST_OP_FAILED  1
#define LIST_OP_SUCCESS 0

#define LIST_IS_EMPTY   1
#define LIST_NOT_EMPTY  0

typedef struct list {
	INT8U m_list[MAX_LIST_SIZE];
	INT8U m_size;
} list;

/** 初始化列表，表大小置零
  * param mlist  需要初始化的列表
  *
  */
void init_list(list *mlist);

/** 列表添加元素，将元素值插入表中
  * @param mlist 列表指针
  * @param elem	 插入的元素值，一般是节点的IP地址
  *
  */
void add_elem(list *mlist, INT8U elem);

/** 列表删除元素，根据元素值删除指定元素
  * @param mlist 列表指针
  * @param elem  删除元素的值，一般是节点IP地址
  *
  */
void remove_elem(list *mlist, INT8U elem);

/** 根据元素下标获得元素值
  * @param mlist 列表指针
  * @param index 查询的元素在表中的下表地址
  *
  */
INT8U get_idx_of_elem(list *mlist, INT8U index);

/** 判断列表是否为空，空--返回1，不空返回0
  * @param mlist 列表指针
  *
  */
INT8U is_list_empty(list *mlist);

/** 返回列表的当前大小
  * @param mlist 列表指针
  *
  */
INT8U list_size(list *mlist);


#endif