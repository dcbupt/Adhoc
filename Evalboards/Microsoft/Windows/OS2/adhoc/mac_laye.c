#include <stdio.h>
#include <string.h>
//#include "types.h"
#include "includes.h"
#include "vardef.h"
#include "mac_laye.h"
#include "mac_help.h"

#include "packetBuffer.h"
#include "send_mes.h"
#include "timer.h"
#include "network.h"
#include "mem_man.h"
#include "log.h"
#include "publicstorage.h"


INT8U waitDATA_flag;

INT8U waitACK_flag;

INT8U resend_data_num;

INT8U rts_flag;

extern NodeInfo getNodeInfo(int no);
extern INT8U time_up[NUM_OF_NODES];
extern packet_queue       packet_buffer_queue;
extern INT8U              blocked_queue_flag;
extern connect_info_t     connect_matrix[NUM_OF_NODES][NUM_OF_NODES];

extern INT8U test_hh;
extern INT32U THIS_NODE;

connect_info       neighbor_node[NUM_OF_NODES];
extern move_info	node_move[NUM_OF_NODES];
extern speed_info	node_speed[NUM_OF_NODES];
extern INT32U	    node_time[NUM_OF_NODES];
antenna_state      node_antenna_state[4];
INT8U			   current_next_node;
INT8U              current_narrow_antenna;
BOOLEAN            connect_matrix_changed;

INT8U              cts_flag;
INT8U              cts_antenna;

INT8U              end_send;

/***************************************
 * Function:      set_node_msg_timer
 * Description:   设置本节点时隙
 *                标识位为1，调用send_node_msg函数
 * Input:
 * Output:
 **************************************/
void set_node_msg_timer()
{
  // 标识位为1， 调用send_node_msg函数
  set_timer(25+THIS_NODE*100, 1);
}

/***************************************
* Function:      quit_connect_matrix
* Description:   删除邻居数组中原来的邻居节点
				 删除连通矩阵中邻居节点
* Input:
* Output:
**************************************/

void quit_connect_matrix(INT8U neighbourID)
{
	int i;
	time_up[neighbourID] = 0;
	connect_matrix[THIS_NODE][neighbourID].connect_ = 0;
	connect_matrix[neighbourID][THIS_NODE].connect_ = 0;

	connect_matrix_changed = CONN_MATRIX_CHANGED;
	neighbor_node[neighbourID].connect = 0;
	neighbor_node[neighbourID].from_antenna = INITIALIZE_CONN_ANTENNA;
}


void send_node_msg()
{
	INT8U antenna_id;

	INT8U this_antenna;
	INT8U next_node;

	INT8U narrow_set[9];
	INT8U wide_set[Set_Wide_Beam_Length];

	INT8U node_msg[NODE_MSG_SIZE];

	packet* head_pkt;

	memset(node_msg, 0, sizeof(node_msg));

	print_connect_matrix();

	PrintfLog(LOG_DEBUG, "Time = %d\n", MARSTimeGet());


	if (rts_flag == 1)
	{
		rts_flag = 0;
		antenna_id = get_antenna_index(current_narrow_antenna);
		if (node_antenna_state[antenna_id].mode != NARROW_BEAM_SEND_MODE) {
			generate_wide_set(wide_set, 1, current_narrow_antenna);
			set_wide_beam(wide_set, Set_Wide_Beam_Length);
			PrintfLog(LOG_DEBUG, "not receive cts\n set wide beam\n");
		}
	}

		computeRouteTableWithoutLET();
	
	

	if (all_in_wide_beam())
	{
		PrintfLog(LOG_DEBUG, "all antenna in wide beam\n");
		if (!is_empty(&packet_buffer_queue))
		{
			PrintfLog(LOG_DEBUG, "has data to send\n");
			next_node = read_head(&packet_buffer_queue)->nextID;
			this_antenna = neighbor_node[next_node].from_antenna;
			current_next_node = next_node;

			if (connect_matrix[next_node][THIS_NODE].connect_ == 0){
				head_pkt = dequeue(&packet_buffer_queue);
				mem_Free(DatapackageMemType, head_pkt->data);
				mem_Free(BufferInfoMemType, head_pkt);
				PrintfLog(LOG_DEBUG, "destID is missing,dequeue pkt\n");
			}
			else{
				generate_node_msg(node_msg, 1, next_node);
				send_on_wide_beam(node_msg, NODE_MSG_SIZE, this_antenna, DIRECT_SEND);
				rts_flag = 1;
				PrintfLog(LOG_DEBUG, "^^^^^^send rts to ID:%d ^^^^^^\n", next_node);

				generate_narrow_set(narrow_set, 5, this_antenna, next_node);
				set_narrow_beam(narrow_set, 9);
				PrintfLog(LOG_DEBUG, "set narrow beam antenna:%d\n", this_antenna);

				current_narrow_antenna = this_antenna;

				antenna_id = get_antenna_index(current_narrow_antenna);
				node_antenna_state[antenna_id].beam = NARROW_BEAM;
				node_antenna_state[antenna_id].recv_or_send = SENDER;
				node_antenna_state[antenna_id].mode = NARROW_BEAM_SET_MODE;
			}
		}
		else
		{
			PrintfLog(LOG_DEBUG, " has no data to send\n send wide beam msg\n");
			generate_node_msg(node_msg, 0, 0);
			send_on_wide_beam(node_msg, NODE_MSG_SIZE, FORWARD_ANTENNA + BACK_ANTENNA + LEFT_ANTENNA + RIGHT_ANTENNA, DIRECT_SEND);
		}
	}
	else
	{
		PrintfLog(LOG_DEBUG, " has antenna in narrow beam\n");
		//do nothing
	}

	set_timer(WideBeam_Time, 1);
}





/***************************************
 * Function:      send_buf_packet
 * Description:   发送缓存数据包
 * Input:
 * Output:
 **************************************/
void send_buf_packet()
{
	packet* pkt_to_send;
	INT8U pkt_type;
	INT16U pkt_len;
	INT8U len1;
	INT8U len2;
	INT8U end_msg;
	INT8U wide_set[Set_Wide_Beam_Length];



	if (!is_empty(&packet_buffer_queue))
	{
		if ((read_head(&packet_buffer_queue))->nextID == current_next_node){
			pkt_to_send = read_head(&packet_buffer_queue);
			pkt_type = DATA_TYPE;
			pkt_len = pkt_to_send->data_length - 3;
			len1 = (pkt_len >> 8) & 0xFF;
			len2 = (pkt_len)& 0xFF;
			pkt_to_send->data[0] = pkt_type;
			pkt_to_send->data[1] = len1;
			pkt_to_send->data[2] = len2;

			send_on_narrow_beam(pkt_to_send->data, (INT32U)pkt_to_send->data_length + 1, current_narrow_antenna, DIRECT_SEND);
			PrintfLog(LOG_DEBUG, "send packet success\n");
			set_timer(Ack_Time, 21);
		}
		else{
			end_msg = generate_end_msg();
			send_on_narrow_beam(&end_msg, 1 + 1, current_narrow_antenna, DIRECT_SEND);
			PrintfLog(LOG_DEBUG, "pkt->next_node has changed\n\^^^^^send end msg\^^^^^\n");
			waitACK_flag = 0;

			generate_wide_set(wide_set, 1, current_narrow_antenna);
			set_wide_beam(wide_set, Set_Wide_Beam_Length);
			PrintfLog(LOG_DEBUG, "set wide beam,antenna:%d\n", current_narrow_antenna);
		}
	}
	else
	{
		end_msg = generate_end_msg();
		send_on_narrow_beam(&end_msg, 1 + 1, current_narrow_antenna, DIRECT_SEND);
		PrintfLog(LOG_DEBUG, "buffer queue is empty\n\^^^^^send end msg\^^^^^\n");
		waitACK_flag = 0;

		generate_wide_set(wide_set, 1, current_narrow_antenna);
		set_wide_beam(wide_set, Set_Wide_Beam_Length);
		PrintfLog(LOG_DEBUG, "set wide beam,antenna:%d\n", current_narrow_antenna);
	}
}





/***************************************
 * Function:      read_node_msg
 * Description:   解析宽波束模式的节点信息，
 *                宽波束下接收到节点信息应
 *                调用此函数。
 * Input: packet  数据包指针
 *        antenna 接收到信息的天线
 * Output:
 **************************************/
void read_node_msg(INT8U* packet, INT8U memType)
{
	INT8U from_node;

	INT8U rts;
	INT8U recv_node;
	INT8U recv_antenna;

	INT8U narrow_set[9];

	int k;
	int l = 0;
	int i, j;
	int m;

	INT8U antenna_index;

	from_node = (INT8U)((packet[14] >> 4) & 0x0f);
	PrintfLog(LOG_DEBUG, "receive %d\n", from_node);

	if (time_up[from_node] == 1)
	{
		cancle_timer(from_node + 10);
		time_up[from_node] = 0;
	}

	recv_antenna = (INT8U)((packet[0] >> 4) & 0x0f);
	neighbor_node[from_node].connect = 1;
	neighbor_node[from_node].from_antenna = recv_antenna;

	memcpy(&kNodesInfo[from_node].x, &packet[15], 4);
	memcpy(&kNodesInfo[from_node].y, &packet[19], 4);
	memcpy(&kNodesInfo[from_node].z, &packet[23], 2);

	memcpy(&kNodesInfo[from_node].x_speed, &packet[25], 2);
	memcpy(&kNodesInfo[from_node].y_speed, &packet[27], 2);
	memcpy(&kNodesInfo[from_node].z_speed, &packet[29], 2);

	memcpy(&node_time[from_node], &packet[31], 3);

	rts = ((packet[34]) >> 7) & (INT8U)0x01;

	if (rts == 1)
	{
		recv_node = (INT8U)(packet[14] & 0x0f);
		if (recv_node == THIS_NODE){
			PrintfLog(LOG_DEBUG, "receive rts\n");
			if (all_in_wide_beam())
			{
				PrintfLog(LOG_DEBUG, "set narrow beam,antenna: %d\n", recv_antenna);
				generate_narrow_set(narrow_set, 5, recv_antenna, from_node);
				set_narrow_beam(narrow_set, 9);
				antenna_index = get_antenna_index(recv_antenna);
				current_narrow_antenna = recv_antenna;
				node_antenna_state[antenna_index].beam = NARROW_BEAM;
				node_antenna_state[antenna_index].recv_or_send = RECEIVER;
				node_antenna_state[antenna_index].mode = NARROW_BEAM_RECV_SET_MODE;

				cts_flag = 1;
				cts_antenna = recv_antenna;
			}
			else
			{
				PrintfLog(LOG_DEBUG, "has antenna in narrow beam\n");
			}
		}
		else{
			PrintfLog(LOG_DEBUG, "RTS NOT FOR THIS NODE\n");
		}
	}
	else{
		PrintfLog(LOG_DEBUG, "not rts msg\n");

		if (connect_matrix[1][0].connect_ == 1){
			connect_matrix[1][0].connect_ = connect_matrix[1][0].connect_ && (packet[34] >> 4 & 0x01);
			connect_matrix[0][1].connect_ = connect_matrix[0][1].connect_ && (packet[34] >> 4 & 0x01);
		}
		else{
			connect_matrix[1][0].connect_ = connect_matrix[1][0].connect_ || (packet[34] >> 4 & 0x01);
			connect_matrix[0][1].connect_ = connect_matrix[0][1].connect_ || (packet[34] >> 4 & 0x01);
		}
		if (connect_matrix[2][0].connect_ == 1){
			connect_matrix[2][0].connect_ = connect_matrix[2][0].connect_ && (packet[34] >> 3 & 0x01);
			connect_matrix[0][2].connect_ = connect_matrix[0][2].connect_ && (packet[34] >> 3 & 0x01);
		}
		else{
			connect_matrix[2][0].connect_ = connect_matrix[2][0].connect_ || (packet[34] >> 3 & 0x01);
			connect_matrix[0][2].connect_ = connect_matrix[0][2].connect_ || (packet[34] >> 3 & 0x01);
		}
		if (connect_matrix[2][1].connect_ == 1){
			connect_matrix[2][1].connect_ = connect_matrix[2][1].connect_ && (packet[34] >> 2 & 0x01);
			connect_matrix[1][2].connect_ = connect_matrix[1][2].connect_ && (packet[34] >> 2 & 0x01);
		}
		else{
			connect_matrix[2][1].connect_ = connect_matrix[2][1].connect_ || (packet[34] >> 2 & 0x01);
			connect_matrix[1][2].connect_ = connect_matrix[1][2].connect_ || (packet[34] >> 2 & 0x01);
		}
		if (connect_matrix[3][0].connect_ == 1){
			connect_matrix[3][0].connect_ = connect_matrix[3][0].connect_ && (packet[34] >> 1 & 0x01);
			connect_matrix[0][3].connect_ = connect_matrix[0][3].connect_ && (packet[34] >> 1 & 0x01);
		}
		else{
			connect_matrix[3][0].connect_ = connect_matrix[3][0].connect_ || (packet[34] >> 1 & 0x01);
			connect_matrix[0][3].connect_ = connect_matrix[0][3].connect_ || (packet[34] >> 1 & 0x01);
		}
		if (connect_matrix[3][1].connect_ == 1){
			connect_matrix[3][1].connect_ = connect_matrix[3][1].connect_ && (packet[34] & 0x01);
			connect_matrix[1][3].connect_ = connect_matrix[1][3].connect_ && (packet[34] & 0x01);
		}
		else{
			connect_matrix[3][1].connect_ = connect_matrix[3][1].connect_ || (packet[34] & 0x01);
			connect_matrix[1][3].connect_ = connect_matrix[1][3].connect_ || (packet[34] & 0x01);
		}
		if (connect_matrix[3][2].connect_ == 1){
			connect_matrix[3][2].connect_ = connect_matrix[3][2].connect_ && (packet[35] >> 7 & 0x01);
			connect_matrix[2][3].connect_ = connect_matrix[2][3].connect_ && (packet[35] >> 7 & 0x01);
		}
		else{
			connect_matrix[3][2].connect_ = connect_matrix[3][2].connect_ || (packet[35] >> 7 & 0x01);
			connect_matrix[2][3].connect_ = connect_matrix[2][3].connect_ || (packet[35] >> 7 & 0x01);
		}
		for (i = 4; i < NUM_OF_NODES; i++){
			for (j = 0; j < i; j++){
				k = i - 1;
				while (k != 0){
					l += k;
					k--;
				}
				l = l + j + 4;
				if (connect_matrix[i][j].connect_ == 1){
					if (l % 8 != 0){
						connect_matrix[i][j].connect_ = connect_matrix[i][j].connect_ && (packet[34 + l / 8] >> (8 - l % 8) & 0x01);
						connect_matrix[j][i].connect_ = connect_matrix[j][i].connect_ && (packet[34 + l / 8] >> (8 - l % 8) & 0x01);
						l = 0;
					}
					else{
						connect_matrix[i][j].connect_ = connect_matrix[i][j].connect_ && (packet[33 + l / 8] & 0x01);
						connect_matrix[j][i].connect_ = connect_matrix[j][i].connect_ && (packet[33 + l / 8] & 0x01);
						l = 0;
					}
				}
				else{
					if (l % 8 != 0){
						connect_matrix[i][j].connect_ = connect_matrix[i][j].connect_ || (packet[34 + l / 8] >> (8 - l % 8) & 0x01);
						connect_matrix[j][i].connect_ = connect_matrix[j][i].connect_ || (packet[34 + l / 8] >> (8 - l % 8) & 0x01);
						l = 0;
					}
					else{
						connect_matrix[i][j].connect_ = connect_matrix[i][j].connect_ || (packet[33 + l / 8] & 0x01);
						connect_matrix[j][i].connect_ = connect_matrix[j][i].connect_ || (packet[33 + l / 8] & 0x01);
						l = 0;
					}
				}
			}
		}

		//update neighbour on connect_matrix
		for (m = 0; m < NUM_OF_NODES; m++){
			connect_matrix[THIS_NODE][m].connect_ = neighbor_node[m].connect;
			connect_matrix[m][THIS_NODE].connect_ = neighbor_node[m].connect;
		}
	}

	PrintfLog(LOG_DEBUG, "connect matrix finished\n");
	set_timer(Neighbour_Time, from_node + 10);
	time_up[from_node] = 1;

	mem_Free(memType, packet);
}



void read_narrowBeam_msg(INT8U* packet, INT16U pkt_len)
{
	INT8U pkt_type;
	INT8U ack_msg;
	INT8U end_msg;
	INT8U wide_set[5];
	INT8U* data;
	data = packet + 14;
	//memcpy(data, packet + 14, pkt_len - 14);


	pkt_type = data[0] & 0xFF;
	switch (pkt_type)
	{
	case DATA_TYPE:
		if (waitDATA_flag == 1){
			cancle_timer(20);
			PrintfLog(LOG_DEBUG, "cancle waitDataTimer\nreceive data: %s\n", data);
			receiveFromMac((void*)data, pkt_len - 14);
			mem_Free(DatapackageMemType, packet);
			if (blocked_queue_flag == BLOCKED)
			{
				ack_msg = generate_ack_overflow_msg();
				send_on_narrow_beam(&ack_msg, 1 + 1, current_narrow_antenna, DIRECT_SEND);
				PrintfLog(LOG_DEBUG, "send ack overflow msg\n");

				generate_wide_set(wide_set, 1, current_narrow_antenna);
				set_wide_beam(wide_set, Set_Wide_Beam_Length);
				PrintfLog(LOG_DEBUG, "set wide beam,antenna:%d\n", current_narrow_antenna);

				waitDATA_flag = 0;
			}
			else
			{
				ack_msg = generate_ack_normal_msg();
				send_on_narrow_beam(&ack_msg, 1 + 1, current_narrow_antenna, DIRECT_SEND);
				PrintfLog(LOG_DEBUG, "send ack normal msg\n");
				set_timer(Data_Time, 20);
			}
		}
		else{
			PrintfLog(LOG_DEBUG, "recv data but waitDATA timeUp\n");
			mem_Free(DatapackageMemType, packet);
		}
		break;

	case END_TYPE:
		mem_Free(DatapackageMemType, packet);
		if (waitDATA_flag == 1){
			PrintfLog(LOG_DEBUG, "cancle waitDataTimer\n");
			cancle_timer(20);

			PrintfLog(LOG_DEBUG, "\^^^^^receive end msg\^^^^^\n");

			waitDATA_flag = 0;

			generate_wide_set(wide_set, 1, current_narrow_antenna);
			set_wide_beam(wide_set, Set_Wide_Beam_Length);
			PrintfLog(LOG_DEBUG, "set wide beam,antenna:%d\n", current_narrow_antenna);
		}

		break;

	case ACK_NORMAL_TYPE:
		if (waitACK_flag == 1){
			cancle_timer(21);
			PrintfLog(LOG_DEBUG, "cancle ACK Timer\nreceive ack normal msg\n");
			mem_Free(DatapackageMemType, read_head(&packet_buffer_queue)->data);
			mem_Free(BufferInfoMemType, dequeue(&packet_buffer_queue));
			send_buf_packet();
		}
		else{
			PrintfLog(LOG_DEBUG, "waitACK TimeUp\n");
		}
		mem_Free(DatapackageMemType, packet);
		break;

	case ACK_OVERFLOW_TYPE:
		if (waitACK_flag == 1){
			cancle_timer(21);
			waitACK_flag = 0;
			PrintfLog(LOG_DEBUG, "Cancle wait ACK timer\n");

			PrintfLog(LOG_DEBUG, "receive ack overflow msg\n");
			mem_Free(DatapackageMemType, read_head(&packet_buffer_queue)->data);
			mem_Free(BufferInfoMemType, dequeue(&packet_buffer_queue));
			generate_wide_set(wide_set, 1, current_narrow_antenna);
			set_wide_beam(wide_set, Set_Wide_Beam_Length);
			PrintfLog(LOG_DEBUG, "set wide beam,antenna:%d\n", current_narrow_antenna);
		}
		else{
			PrintfLog(LOG_DEBUG, "waitACK TimeUp\n");
		}
		mem_Free(DatapackageMemType, packet);
		break;

	case CTS_TYPE:
		rts_flag = 0;
		mem_Free(DatapackageMemType, packet);
		PrintfLog(LOG_DEBUG, "\^^^^^receive cts\^^^^^\n");
		node_antenna_state[current_narrow_antenna].mode = NARROW_BEAM_SEND_MODE;
		send_buf_packet();
		waitACK_flag = 1;
		break;

	default:
		printf("unknown msg type!!\n");
		break;
	}
}


void read_setNarrowBeamRsp(void* data, INT8U memType, INT32U data_len)
{
	INT8U* packet;
	INT8U set_antenna;
	INT8U result;
	INT8U command[18];
	int antenna_id;
	packet = data;
	if (((packet[0] >> 4) & 0x0f)&(packet[0] & 0x0f) == 0){
		mem_Free(memType, data);
		PrintfLog(LOG_DEBUG, "receive set narrow beam fail rsp\n");
	}
	else{
		set_antenna = (packet[0] >> 4) & 0x0f;
		antenna_id = get_antenna_index(set_antenna);
		printf("set antenna :%d\n", set_antenna);
		printf("antenna id:%d\n", antenna_id);
		if (node_antenna_state[antenna_id].mode == NARROW_BEAM_SET_MODE){
			node_antenna_state[antenna_id].mode = NARROW_BEAM_SET_LINK;
			generate_link_set(command, 14, set_antenna, 0);
			set_link(command, 18);
		}
		else if (node_antenna_state[antenna_id].mode == NARROW_BEAM_RECV_SET_MODE){
			node_antenna_state[antenna_id].mode = NARROW_BEAM_SET_LINK;
			generate_link_set(command, 14, set_antenna, 1);
			set_link(command, 18);
		}
		else{

		}

		mem_Free(memType, data);

		PrintfLog(LOG_DEBUG, "receive set narrow beam success rsp\n");
	}
}

void read_setLinkRsp(void* data, INT8U memType, INT32U data_len)
{
	INT8U* packet;
	INT8U set_antenna;
	INT8U result;
	INT8U cts_msg;
	INT8U antenna_id;
	packet = data;
	if (((packet[0] >> 4) & 0x0f)&(packet[0] & 0x0f) == 0){
		mem_Free(memType, data);
		PrintfLog(LOG_DEBUG, "receive set link fail rsp\n");
		}
		else{
			set_antenna = (packet[0] >> 4) & 0x0f;
			antenna_id = get_antenna_index(set_antenna);
			printf("set antenna :%d\n", set_antenna);
			printf("antenna id:%d\n", antenna_id);
			if (node_antenna_state[antenna_id].mode == NARROW_BEAM_SET_LINK){
				if (cts_flag == 1){
					node_antenna_state[antenna_id].mode = NARROW_BEAM_RECV_MODE;
					cts_msg = generate_cts_msg();
					send_on_narrow_beam(&cts_msg, 1 + 1, cts_antenna, DIRECT_SEND);
					set_timer(Data_Time, 20);
					waitDATA_flag = 1;
					cts_flag = 0;
					cts_antenna = INITIALIZE_CONN_ANTENNA;
					PrintfLog(LOG_DEBUG, "recv set link success rsp\nsend cts\n");
				}
				else{
					node_antenna_state[antenna_id].mode = NARROW_BEAM_SEND_BEFORE_MODE;
					PrintfLog(LOG_DEBUG, "recv set link success rsp\n");
				}
				mem_Free(memType, data);
		}
	}
}

void read_setWideBeamRsp(void* data, INT8U memType, INT32U data_len)
{
	INT8U* packet;
	INT8U set_antenna;
	INT8U result;
	int antenna_id;
	packet = data;
	//memcpy(packet, data, data_len);

	if (((packet[0] >> 4) & 0x0f)&(packet[0] & 0x0f) == 0){
		mem_Free(memType, data);

		PrintfLog(LOG_DEBUG, "receive set wide beam fail rsp\n");
	}
	else{
		set_antenna = (packet[0] >> 4) & 0x0f;
		printf("set antenna :%x\n", set_antenna);
		antenna_id = get_antenna_index(set_antenna);
		printf("antenna: %d\n", antenna_id);
		node_antenna_state[antenna_id].beam = WIDE_BEAM;
		node_antenna_state[antenna_id].recv_or_send = RS_DEFAULT;
		node_antenna_state[antenna_id].mode = MODE_DEFAULT;
		current_narrow_antenna = CUR_ANTENNA_DEFAULT;

		mem_Free(memType, data);

		PrintfLog(LOG_DEBUG, "receive set wide beam success rsp\n");
	}
}

void dataTimeUp_Process()
{
	INT8U wide_set[Set_Wide_Beam_Length];
	generate_wide_set(wide_set, 1, current_narrow_antenna);
	set_wide_beam(wide_set, Set_Wide_Beam_Length);
	waitDATA_flag = 0;
}

void ackTimeUp_Process()
{
	INT8U wide_set[Set_Wide_Beam_Length];
	if (connect_matrix[THIS_NODE][current_next_node].connect_ == 0)
	{
		generate_wide_set(wide_set, 1, current_narrow_antenna);
		set_wide_beam(wide_set, Set_Wide_Beam_Length);
		resend_data_num = 3;
		waitACK_flag = 0;
	}
	else
	{
		if (resend_data_num > 0){
			resend_data_num--;
			send_buf_packet();
		}
		else{
			mem_Free(DatapackageMemType, read_head(&packet_buffer_queue)->data);
			mem_Free(BufferInfoMemType, dequeue(&packet_buffer_queue));

			generate_wide_set(wide_set, 1, current_narrow_antenna);
			set_wide_beam(wide_set, Set_Wide_Beam_Length);
			resend_data_num = 3;
			waitACK_flag = 0;
		}
	}
}
