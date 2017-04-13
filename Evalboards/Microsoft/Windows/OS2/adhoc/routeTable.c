#include "routeTable.h"
#include <stddef.h>

/**
  * ��ʼ��·�ɱ�
  * @param table ��Ҫ��ʼ����·�ɱ�ָ��
  *
  */
void init_route_table(route_table_map *table) {
	clear_table(table);
	printf("init_route_table!\n");
}


/**
  * ·�ɱ�  ��Ӳ���
  * @param  table   ��Ҫ�����·�ɵ�·�ɱ�
  * @param  new_entry   �µ�·�ɱ���
  * @return ��ӳɹ�����0��ʧ�ܷ���1
  *
  */
int add_entry(route_table_map *table, rt_entry *new_entry){
	INT8U destID = new_entry->destID;
	rt_entry *entry;
	if (table->m_isExisted[destID] != 1) {
		table->m_size++;
		table->m_isExisted[destID] = 1;
	}

	entry = &table->m_table[destID];

	entry->destID = new_entry->destID;
	entry->hops = new_entry->hops;
	entry->LET = new_entry->LET;
	entry->nextID = new_entry->nextID;

	return 0;
}

/**
  * ɾ����Ӧ����
  *
  * @param table  ·�ɱ�
  * @param destID  ��Ҫɾ���ı����Ŀ��ڵ�ID
  *
  */

void remove_entry(route_table_map *table, INT8U destID){
	if (table->m_isExisted[destID] != 1) {
		return;
	}
	
	table->m_size--;
	table->m_isExisted[destID] = 0;
	
}

/**
  * ���·�ɱ�
  * @param  table ��Ҫ��յ�·�ɱ�Ӧ��
  *	
  */
void clear_table(route_table_map *table) {
	int i;
	table->m_size = 0;
	for (i = 0; i < MAX_ROUTE_TABLE_SIZE; ++i) {
		table->m_isExisted[i] = 0;
	}
}


/**
  * ����·�ɱ���
  *
  * @param  table    ·�ɱ�
  * @param  destID   Ŀ���ַID
  * @return ����ҵ��������ҵ���·�ɱ����ַ�����򣬷���NULL
  *
  */
rt_entry* find_entry(route_table_map *table, INT8U destID) {
	if (table->m_isExisted[destID] != 1) {
		return NULL;
	} 

	return &table->m_table[destID];
}


/**
  * ��ѯ����·��
  /// Imagine a routing table like this: [A,B] [B,C] [C,C]; being each pair of the
  /// form [dest addr,next-hop addr]. In this case, if this function is invoked with
  /// [A,B] then pair [C,C] is returned because C is the next hop that must be used
  /// to forward a data packet destined to A. That is, C is a neighbor of this node,
  /// but B isn't. This function finds the appropiate neighbor for forwarding a packet.
  ///
  *
  *	@param table  ·�ɱ�
  * @param destID  Ŀ��ڵ�ID
  * @return ����ҵ��������ҵ���·�ɱ����ַ�����򣬷���NULL
  * 
  */
rt_entry* find_send_entry(route_table_map *table, INT8U destID) {
	rt_entry* entry;
	if (table->m_isExisted[destID] != 1) {
		return NULL;
	}

	entry = &table->m_table[destID];
	while (table->m_isExisted[entry->destID] == 1 && entry->destID != entry->nextID)
	{
		entry = &table->m_table[entry->nextID];
	}

	if (table->m_isExisted[entry->destID] != 1) {
		return NULL;
	}

	return entry;
}

