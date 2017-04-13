#include "network.h"
#include "list.h"
#include "mem_man.h"
#include "mac_help.h"
#include "log.h"
#include "packetBuffer.h"
#include "mac_help.h"
/************************************************************************/
/* 全局变量的定义                                                                     */
/************************************************************************/

route_table_map  rt_table;			//全局路由表

connect_info_t  connect_matrix[NUM_OF_NODES][NUM_OF_NODES];		//全局全连通矩阵

packet_queue packet_buffer_queue;		//全局的数据包缓存队列

INT8U blocked_queue_flag;			//队列阻塞标志符，1代表阻塞，0非阻塞

extern INT8U THIS_NODE;
//初始化缓存队列
//init_packet_queue(&packet_buffer_queue, 1000, 0.8);	放在主程序开始处	


#define DEBUG_NET 


INT8U getNodeAddr() {
	return THIS_NODE;
}

void send_to_app(void *point2packet, INT32U length) {
	PrintfLog(LOG_DEBUG,"send data to app\n");
	return;
}


/**
  *	根据全连通矩阵计算路由
  *
  */
void computeRouteTable() {

	list unvisited_nodes;			//未访问的节点列表

	INT8U cur_nodeID = getNodeAddr();		//获取当前节点ID的接口 TODO--由系统提供
	INT16S dist[NUM_OF_NODES];				//该节点到相应节点的路径损耗（跳数）
	INT16S prev_nodeID[NUM_OF_NODES];		//该节点到相应节点最短路径的前一跳节点ID
	FP64  min_entry_LET[NUM_OF_NODES];		//该节点到相应节点的路径当前最短LET值

	INT16S unvisited_size;		//未访问节点数
	INT16S min_dist_nodeID;	//未访问节点中dist值最小的节点ID
	INT16S alt_dist;			//新计算的路径损耗
	INT16S unvisited_nodeID;	//未访问节点ID

	FP64 min_let, let_of_nodes;	
	INT8U i;

	rt_entry new_entry;		
	INT8U prev_node_id;

	//清空路由
	clear_table(&rt_table);
	//初始化列表	
	init_list(&unvisited_nodes);	

	//初始化变量
	dist[cur_nodeID] = 0;
	prev_nodeID[cur_nodeID] = UNDEFINED;
	min_entry_LET[cur_nodeID] = DOUBLE_INFINITY;

	for (i = 0; i < NUM_OF_NODES; ++i) {
		if (i != cur_nodeID) {
			dist[i] = UNDEFINED;
			prev_nodeID[i] = UNDEFINED;
			min_entry_LET[i] = DOUBLE_INFINITY;
		}

		add_elem(&unvisited_nodes, i);
	}

	while (is_list_empty(&unvisited_nodes) != LIST_IS_EMPTY) {
		unvisited_size = list_size(&unvisited_nodes);
		min_dist_nodeID = get_idx_of_elem(&unvisited_nodes, 0);	//未访问节点中dist值最小的节点ID
		//选取dist最小的那个节点ID
		for (i = 1; i < unvisited_size; ++i) {
			unvisited_nodeID = get_idx_of_elem(&unvisited_nodes, i);
			if (dist[min_dist_nodeID] > dist[unvisited_nodeID]) {
				min_dist_nodeID = unvisited_nodeID;
			}
		}

		remove_elem(&unvisited_nodes, min_dist_nodeID);		//删除最小值的节点
		unvisited_size = list_size(&unvisited_nodes);

		//遍历min_dist_nodeID节点的所有邻居，更新邻居的dist和prev_nodeID
		for (i = 0; i < unvisited_size; ++i) {
			unvisited_nodeID = get_idx_of_elem(&unvisited_nodes, i);
			if (connect_matrix[min_dist_nodeID][unvisited_nodeID].connect_ == 1) {
				alt_dist = dist[min_dist_nodeID] + 1;
				let_of_nodes = connect_matrix[min_dist_nodeID][unvisited_nodeID].let_;
				if (alt_dist < dist[unvisited_nodeID]) {
					dist[unvisited_nodeID] = alt_dist;
					prev_nodeID[unvisited_nodeID] = min_dist_nodeID;
					//let_of_nodes = connect_matrix[min_dist_nodeID][unvisited_nodeID].let_;
					if (min_entry_LET[unvisited_nodeID] > let_of_nodes) {	//Q:min_entry_LET[unvisited_nodeID]=min(let[min_dist_nodeID],connect.let)
						min_entry_LET[unvisited_nodeID] = let_of_nodes;
					} 
				} else if (alt_dist == dist[unvisited_nodeID]) {	//代价相同时，比较LET
					min_let = min_entry_LET[min_dist_nodeID] > let_of_nodes ? let_of_nodes : min_entry_LET[min_dist_nodeID];
					if (min_entry_LET[unvisited_nodeID] < min_let) {		// > ?
						prev_nodeID[unvisited_nodeID] = min_dist_nodeID;
						min_entry_LET[unvisited_nodeID] = min_let;
					}
				}
			}	
		}	
	}  
	
	//根据DIJSTRA算法的结果，dist,prev_nodeID,min_entry_LET回溯链路下一跳，构建路由表
	for (i = 0; i < NUM_OF_NODES; ++i) {
		prev_node_id = i;
		if (prev_nodeID[prev_node_id] != UNDEFINED) {
			while (prev_nodeID[prev_node_id] != cur_nodeID) {
				prev_node_id = prev_nodeID[prev_node_id];
			}

			new_entry.destID = i;
			new_entry.hops = dist[i];
			new_entry.LET = min_entry_LET[i];
			new_entry.nextID = prev_node_id;

			add_entry(&rt_table, &new_entry);
		}
	}

}

//不考虑LET时的路由表计算
void computeRouteTableWithoutLET() {

	list unvisited_nodes;			//未访问的节点列表

	INT8U cur_nodeID = getNodeAddr();		//获取当前节点ID的接口 TODO--由系统提供
	INT16S dist[NUM_OF_NODES];				//该节点到相应节点的路径损耗（跳数）
	INT16S prev_nodeID[NUM_OF_NODES];		//该节点到相应节点最短路径的前一跳节点ID
	//FP64  min_entry_LET[NUM_OF_NODES];		//该节点到相应节点的路径当前最短LET值

	INT16S unvisited_size;		//未访问节点数
	INT16S min_dist_nodeID;	//未访问节点中dist值最小的节点ID
	INT16S alt_dist;			//新计算的路径损耗
	INT16S unvisited_nodeID;	//未访问节点ID

	//FP64 min_let, let_of_nodes;	
	INT16U i;

	rt_entry new_entry;
	INT8U prev_node_id;

	INT8U pre_nextID;
	INT8U tmp_nextID;

	//清空路由
	clear_table(&rt_table);
	//初始化列表	
	init_list(&unvisited_nodes);

	//初始化变量
	dist[cur_nodeID] = 0;
	prev_nodeID[cur_nodeID] = UNDEFINED;
	//min_entry_LET[cur_nodeID] = DOUBLE_INFINITY;

	for (i = 0; i < NUM_OF_NODES; ++i) {
		if (i != cur_nodeID) {
			dist[i] = UNDEFINED;
			prev_nodeID[i] = UNDEFINED;
			//min_entry_LET[i] = DOUBLE_INFINITY;
		}

		add_elem(&unvisited_nodes, i);
	}

	//最短路径算法：DIJSTRA算法
	while (is_list_empty(&unvisited_nodes) != LIST_IS_EMPTY) {
		unvisited_size = list_size(&unvisited_nodes);
		min_dist_nodeID = get_idx_of_elem(&unvisited_nodes, 0);	//未访问节点中dist值最小的节点ID

		//选取dist最小的那个节点ID
		for (i = 1; i < unvisited_size; ++i) {
			unvisited_nodeID = get_idx_of_elem(&unvisited_nodes, i);
			if (dist[min_dist_nodeID] > dist[unvisited_nodeID]) {
				min_dist_nodeID = unvisited_nodeID;
			}
		}

		remove_elem(&unvisited_nodes, min_dist_nodeID);		//删除最小值的节点
		unvisited_size = list_size(&unvisited_nodes);

		//PrintfLog(LOG_DEBUG, "finish 1\n");

		//遍历min_dist_nodeID节点的所有邻居，更新邻居的dist和prev_nodeID
		for (i = 0; i < unvisited_size; ++i) {
			unvisited_nodeID = get_idx_of_elem(&unvisited_nodes, i);
			if (connect_matrix[min_dist_nodeID][unvisited_nodeID].connect_ == 1) {
				alt_dist = dist[min_dist_nodeID] + 1;
				if (alt_dist < dist[unvisited_nodeID]) {
					dist[unvisited_nodeID] = alt_dist;
					prev_nodeID[unvisited_nodeID] = min_dist_nodeID;
				}
				else if (alt_dist == dist[unvisited_nodeID]){
					pre_nextID = unvisited_nodeID;
					while (prev_nodeID[pre_nextID] != cur_nodeID){
						pre_nextID = prev_nodeID[pre_nextID];
					}
					tmp_nextID = min_dist_nodeID;
					while (prev_nodeID[tmp_nextID] != cur_nodeID){
						tmp_nextID = prev_nodeID[tmp_nextID];
					}
					if (get_distance(tmp_nextID) < get_distance(pre_nextID)){
						prev_nodeID[unvisited_nodeID] = min_dist_nodeID;
					}
				}
				else{

				}
			}
		}
		//PrintfLog(LOG_DEBUG, "finish 2\n");
	}

	//根据DIJSTRA算法的结果，dist,prev_nodeID,min_entry_LET回溯链路下一跳，构建路由表
	for (i = 0; i < NUM_OF_NODES; ++i) {
		prev_node_id = i;
		if (prev_nodeID[prev_node_id] != UNDEFINED) {			//必有路由
			while (prev_nodeID[prev_node_id] != cur_nodeID) {
				prev_node_id = prev_nodeID[prev_node_id];
			}

			new_entry.destID = i;
			new_entry.hops = dist[i];
			new_entry.nextID = prev_node_id;

			add_entry(&rt_table, &new_entry);
		}
	}
}


void packetProcess(INT8U destID, void* point2packet, INT32U length) {
	INT8U *ptr_netlayer = (INT8U*)point2packet + OFFSET_TO_NETLAYER;
	INT16U data_length = length - OFFSET_TO_NETLAYER - sizeof(netlayer_hdr);
	netlayer_hdr *ptr_netlayer_hdr = (netlayer_hdr*)ptr_netlayer;
	packet *send_packet;
	rt_entry *route = find_send_entry(&rt_table, destID);
	if (!route){
		PrintfLog(LOG_DEBUG, "Not found route!\n");
		mem_Free(DatapackageMemType, point2packet);
		return;
	}

	ptr_netlayer_hdr->dest_ID = destID;
	ptr_netlayer_hdr->source_ID = getNodeAddr();		//获取当前节点ID的接口 TODO--由系统提供
	ptr_netlayer_hdr->dataLength = data_length;
	ptr_netlayer_hdr->TTL = 32;

	send_packet = (packet*)mem_Malloc(BufferInfoMemType);
	send_packet->nextID = route->nextID;
	send_packet->destID = route->destID;
	send_packet->next = NULL;
	send_packet->data = point2packet;
	send_packet->data_length = length;


	if (is_full(&packet_buffer_queue) != FULL_QUEUE){
		enqueue(&packet_buffer_queue, send_packet);
		PrintfLog(LOG_DEBUG, "enqueue\n");
	}
	else{
		PrintfLog(LOG_DEBUG, "buffer queue is full\n");
		mem_Free(BufferInfoMemType, send_packet);
		mem_Free(DatapackageMemType, point2packet);
	}
}



/** 
  * 处理物理层上来的数据包
  * @param point2packet 数据包地址指针
  * @param length  数据包长度
  *
  */
void receiveFromMac(void *point2packet, INT32U length) {
	INT8U *ptr_netlayer = (INT8U*)point2packet + OFFSET_TO_NETLAYER;
	netlayer_hdr *ptr_netlayer_hdr = (netlayer_hdr*)ptr_netlayer;
	INT8U destID = ptr_netlayer_hdr->dest_ID;
	INT8U TTL = ptr_netlayer_hdr->TTL;
	rt_entry *route;
	packet *send_packet;
	INT8U*  data;

	if (destID == getNodeAddr()) {
		send_to_app(point2packet, length);
	}
	else {
		if (TTL == 0) {
			PrintfLog(LOG_DEBUG, "TTL <= 0\n");
			//mem_Free(DatapackageMemType, point2packet);
			return;
		}
		else {
			ptr_netlayer_hdr->TTL--;
			route = find_send_entry(&rt_table, destID);
			if (!route) {
				PrintfLog(LOG_DEBUG, "not found route\n");
				return;
			}

			data = mem_Malloc(DatapackageMemType);
			memcpy(data, point2packet, length);
			send_packet = (packet*)mem_Malloc(BufferInfoMemType);
			send_packet->nextID = route->nextID;
			send_packet->destID = destID;
			send_packet->next = NULL;
			send_packet->data = data;
			send_packet->data_length = length;

			if (is_full(&packet_buffer_queue) != FULL_QUEUE) {
				enqueue(&packet_buffer_queue, send_packet);
				PrintfLog(LOG_DEBUG, "enqueue\n");
			}
			else {
				PrintfLog(LOG_DEBUG, "buffer queue is full\n");
				mem_Free(BufferInfoMemType, send_packet);
				mem_Free(DatapackageMemType, data);
			}

		}
	}

}




