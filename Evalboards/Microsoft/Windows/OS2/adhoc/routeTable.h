#ifndef _H_ROUTE_TABLE_H
#define _H_ROUTE_TABLE_H

#include "includes.h"

/************************************************************************/
/* ·�ɱ���ز�������                                                                     */
/************************************************************************/

#define MAX_ROUTE_TABLE_SIZE  256

/**
 **  ·�ɱ���
 **/
typedef struct rt_entry {
	INT8U  destID;					//Ŀ��IP��ַ
	INT8U  nextID;					//��һ��IP��ַ
	INT8U  hops;					//������
	FP64  LET;					//��·LETֵ
} rt_entry;


/**
  * ·�ɱ�ṹ�壬����Ŀ��IP��ַ��ֵ��ΪKEY����������Ӧ·�ɱ����ӳ�� 
  */
typedef struct route_table_map {
	rt_entry m_table[MAX_ROUTE_TABLE_SIZE];		//·�ɱ���Ĵ洢��,�±����DestID, Ŀ���ַIP
	INT8U m_isExisted[MAX_ROUTE_TABLE_SIZE];	//��Ӧ�����Ƿ���ڱ�־,1-->���ڣ�0������
	INT8U m_size;								//��ǰ·�ɱ�Ĵ�С
} route_table_map;

/**
  * ��ʼ��·�ɱ�
  * @param table ��Ҫ��ʼ����·�ɱ�ָ��
  *
  */
void init_route_table(route_table_map *table);

/**
  * ·�ɱ�  ��Ӳ���
  * @param  table   ��Ҫ�����·�ɵ�·�ɱ�
  * @param  new_entry   �µ�·�ɱ���
  * @return ��ӳɹ�����0��ʧ�ܷ���1
  *
  */

int add_entry(route_table_map *table, rt_entry *new_entry);

/**
  * ɾ����Ӧ����
  *
  * @param table  ·�ɱ�
  * @param destID  ��Ҫɾ���ı����Ŀ��ڵ�ID
  *
  */

void remove_entry(route_table_map *table, INT8U destID);

/**
  * ���·�ɱ�
  * @param  table ��Ҫ��յ�·�ɱ�Ӧ��
  *	
  */
void clear_table(route_table_map *table);


/**
  * ����·�ɱ���
  *
  * @param  table    ·�ɱ�
  * @param  destID   Ŀ���ַID
  * @return ����ҵ��������ҵ���·�ɱ����ַ�����򣬷���NULL
  *
  */
rt_entry* find_entry(route_table_map *table, INT8U destID);


/**
  * ��ѯ����·��,���⻷··��
  *
  *	@param table  ·�ɱ�
  * @param destID  Ŀ��ڵ�ID
  * @return ����ҵ��������ҵ���·�ɱ����ַ�����򣬷���NULL
  * 
  */
rt_entry* find_send_entry(route_table_map *table, INT8U destID);


#endif