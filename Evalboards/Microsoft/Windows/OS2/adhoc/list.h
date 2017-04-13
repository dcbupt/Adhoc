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

/** ��ʼ���б����С����
  * param mlist  ��Ҫ��ʼ�����б�
  *
  */
void init_list(list *mlist);

/** �б����Ԫ�أ���Ԫ��ֵ�������
  * @param mlist �б�ָ��
  * @param elem	 �����Ԫ��ֵ��һ���ǽڵ��IP��ַ
  *
  */
void add_elem(list *mlist, INT8U elem);

/** �б�ɾ��Ԫ�أ�����Ԫ��ֵɾ��ָ��Ԫ��
  * @param mlist �б�ָ��
  * @param elem  ɾ��Ԫ�ص�ֵ��һ���ǽڵ�IP��ַ
  *
  */
void remove_elem(list *mlist, INT8U elem);

/** ����Ԫ���±���Ԫ��ֵ
  * @param mlist �б�ָ��
  * @param index ��ѯ��Ԫ���ڱ��е��±��ַ
  *
  */
INT8U get_idx_of_elem(list *mlist, INT8U index);

/** �ж��б��Ƿ�Ϊ�գ���--����1�����շ���0
  * @param mlist �б�ָ��
  *
  */
INT8U is_list_empty(list *mlist);

/** �����б�ĵ�ǰ��С
  * @param mlist �б�ָ��
  *
  */
INT8U list_size(list *mlist);


#endif