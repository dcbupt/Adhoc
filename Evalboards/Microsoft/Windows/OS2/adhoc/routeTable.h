#ifndef _H_ROUTE_TABLE_H
#define _H_ROUTE_TABLE_H

#include "includes.h"

/************************************************************************/
/* 路由表相关操作声明                                                                     */
/************************************************************************/

#define MAX_ROUTE_TABLE_SIZE  256

/**
 **  路由表项
 **/
typedef struct rt_entry {
	INT8U  destID;					//目标IP地址
	INT8U  nextID;					//下一跳IP地址
	INT8U  hops;					//总跳数
	FP64  LET;					//链路LET值
} rt_entry;


/**
  * 路由表结构体，根据目的IP地址的值作为KEY来建立到对应路由表项的映射 
  */
typedef struct route_table_map {
	rt_entry m_table[MAX_ROUTE_TABLE_SIZE];		//路由表项的存储表,下表代表DestID, 目标地址IP
	INT8U m_isExisted[MAX_ROUTE_TABLE_SIZE];	//对应表项是否存在标志,1-->存在，0不存在
	INT8U m_size;								//当前路由表的大小
} route_table_map;

/**
  * 初始化路由表
  * @param table 需要初始化的路由表指针
  *
  */
void init_route_table(route_table_map *table);

/**
  * 路由表  添加操作
  * @param  table   需要添加新路由的路由表
  * @param  new_entry   新的路由表项
  * @return 添加成功返回0，失败返回1
  *
  */

int add_entry(route_table_map *table, rt_entry *new_entry);

/**
  * 删除对应表项
  *
  * @param table  路由表
  * @param destID  需要删除的表项的目标节点ID
  *
  */

void remove_entry(route_table_map *table, INT8U destID);

/**
  * 清空路由表
  * @param  table 需要清空的路由表应用
  *	
  */
void clear_table(route_table_map *table);


/**
  * 查找路由表项
  *
  * @param  table    路由表
  * @param  destID   目标地址ID
  * @return 如果找到，返回找到的路由表项地址；否则，返回NULL
  *
  */
rt_entry* find_entry(route_table_map *table, INT8U destID);


/**
  * 查询发送路由,避免环路路由
  *
  *	@param table  路由表
  * @param destID  目标节点ID
  * @return 如果找到，返回找到的路由表项地址；否则，返回NULL
  * 
  */
rt_entry* find_send_entry(route_table_map *table, INT8U destID);


#endif