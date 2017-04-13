#include "routeTable.h"
#include <stddef.h>

/**
  * 初始化路由表
  * @param table 需要初始化的路由表指针
  *
  */
void init_route_table(route_table_map *table) {
	clear_table(table);
	printf("init_route_table!\n");
}


/**
  * 路由表  添加操作
  * @param  table   需要添加新路由的路由表
  * @param  new_entry   新的路由表项
  * @return 添加成功返回0，失败返回1
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
  * 删除对应表项
  *
  * @param table  路由表
  * @param destID  需要删除的表项的目标节点ID
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
  * 清空路由表
  * @param  table 需要清空的路由表应用
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
  * 查找路由表项
  *
  * @param  table    路由表
  * @param  destID   目标地址ID
  * @return 如果找到，返回找到的路由表项地址；否则，返回NULL
  *
  */
rt_entry* find_entry(route_table_map *table, INT8U destID) {
	if (table->m_isExisted[destID] != 1) {
		return NULL;
	} 

	return &table->m_table[destID];
}


/**
  * 查询发送路由
  /// Imagine a routing table like this: [A,B] [B,C] [C,C]; being each pair of the
  /// form [dest addr,next-hop addr]. In this case, if this function is invoked with
  /// [A,B] then pair [C,C] is returned because C is the next hop that must be used
  /// to forward a data packet destined to A. That is, C is a neighbor of this node,
  /// but B isn't. This function finds the appropiate neighbor for forwarding a packet.
  ///
  *
  *	@param table  路由表
  * @param destID  目标节点ID
  * @return 如果找到，返回找到的路由表项地址；否则，返回NULL
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

