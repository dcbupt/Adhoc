#include <stdio.h>
#include "mac_help.h"
#include "packetBuffer.h"
#include "mes_queue.h"
#include "event_type.h"
#include "math.h"
#include "publicstorage.h"

extern NodeInfo getNodeInfo(int no);
extern packet_queue packet_buffer_queue;
extern INT8U blocked_queue_flag;

extern connect_info neighbor_node[NUM_OF_NODES];
move_info node_move[NUM_OF_NODES];
speed_info node_speed[NUM_OF_NODES];
INT32U node_time[NUM_OF_NODES];
INT32U current_time;
INT8U time_up[NUM_OF_NODES];
extern connect_info_t connect_matrix[NUM_OF_NODES][NUM_OF_NODES];
extern antenna_state node_antenna_state[4];
extern INT8U current_narrow_antenna;
extern BOOLEAN connect_matrix_changed;

extern INT8U test_hh;
extern INT32U THIS_NODE;

extern INT8U rts_flag;
extern INT8U cts_flag;
extern INT8U cts_antenna;

extern INT8U waitDATA_flag;
extern INT8U waitACK_flag;
extern INT8U resend_data_num;

extern INT8U end_send;

// 生成END消息
INT8U generate_end_msg()
{
  return END_TYPE;
}


// 生成CTS消息
INT8U generate_cts_msg()
{
  return CTS_TYPE;
}


// 生成ACK消息
INT8U generate_ack_normal_msg()
{
  return ACK_NORMAL_TYPE;
}


INT8U generate_ack_overflow_msg()
{
  return ACK_OVERFLOW_TYPE;
}


void generate_narrow_set(INT8U* command, INT8U len, INT8U antenna, INT8U to_node)
{
	command[0] = SET_NARROW_BEAM_TYPE;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = len;
	command[4] = (antenna << 4) | to_node | 0x00;
	command[5] = 0x00;
	command[6] = 0x00;
	command[7] = 0x00;
	command[8] = 0x00;
}



void get_move_info(move_info* move)
{	

	move->x = getNodeInfo(THIS_NODE).x;
	move->y = getNodeInfo(THIS_NODE).y;
	move->z = getNodeInfo(THIS_NODE).z;
}

void get_speed_info(speed_info* speed)
{
	speed->X_speed = getNodeInfo(THIS_NODE).x_speed;
	speed->Y_speed = getNodeInfo(THIS_NODE).y_speed;
	speed->Z_speed = getNodeInfo(THIS_NODE).z_speed;
}


void generate_wide_set(INT8U* command, INT8U len, INT8U antenna)
{
	command[0] = SET_WIDE_BEAM_TYPE;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = len;
	command[4] = antenna << 4 | 0x00;
}


void generate_link_set(INT8U* command, INT8U len, INT8U antenna, INT8U link_mode)
{
	int i;
	command[0] = SET_LINK_TYPE;
	command[1] = 0x00;
	command[2] = 0x00;
	command[3] = len;
	command[4] = (antenna << 4) | 0x00;
	for (i = 5; i < 17; i++){
		command[i] = 0x00;
	}
	command[17] = link_mode;
}

void printStringByte(INT8U* s, INT32U len)
{
  INT8U i, j;
  INT32U tmp;
  for (i = 0; i < len; ++i)
  {
    tmp = (INT32U)s[i];
    for (j = 0; j < 8; ++j)
    {
      printf("%d ", (tmp&128)>>7);
      tmp <<= 1;
    }
    printf("\n");
  }
}


void print_node_move()
{
	INT8U i;
	printf("===============node%d's move info=============\n", THIS_NODE);
	printf("node_id\tx\ty\tz\t\vx\tvy\tvz\n");
	for (i = 0; i < NUM_OF_NODES; ++i)
	{
		printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
			getNodeInfo(i).x, getNodeInfo(i).y, getNodeInfo(i).z);
	}
}


void print_neighbor_node()
{
	INT8U i;
	printf("===============node%d's neighbor nodes=============\n", THIS_NODE);
	printf("node_id\tconnect\tfrom_antenna\tto_antenna\n");
	for (i = 0; i < NUM_OF_NODES; ++i)
	{
		printf("%d\t%d\t%d\t\n", i, neighbor_node[i].connect, neighbor_node[i].from_antenna);
	}
}


void print_connect_matrix()
{
	INT8U i, j;
	printf("===============node%d's connect matrix=============\n", THIS_NODE);
	for (i = 0; i < NUM_OF_NODES; ++i)
	{
		for (j = 0; j < NUM_OF_NODES; ++j)
		{
			printf("%d\t", connect_matrix[i][j].connect_);
		}
		printf("\n");
	}
}


void generate_node_msg(INT8U* packet, INT8U rts, INT8U recv_node)
{
  INT8U tmp;
  INT8U i, j;
  INT8U k = 0;
  move_info move;
  speed_info speed;
  INT8U compress_connect_matrix[45];

  for (i = 1; i < NUM_OF_NODES; ++i)
  {
	  for (j = 0; j < i; ++j)
	  {
		  compress_connect_matrix[k++] = connect_matrix[i][j].connect_;
	  }
  }

  packet[0] = (THIS_NODE << 4) | (recv_node);

  get_move_info(&move);
  memcpy(&packet[1], &move.x, 4);
  memcpy(&packet[5], &move.y, 4);
  memcpy(&packet[9], &move.z, 2);

  get_speed_info(&speed);
  memcpy(&packet[11], &speed.X_speed, 2);
  memcpy(&packet[13], &speed.Y_speed, 2);
  memcpy(&packet[15], &speed.Z_speed, 2);

  memcpy(&packet[17], &current_time, 3);

  //send compress connect_matrix

  //todo:update matrix label with LET

  tmp = ((INT8U)(rts << 7));
	  for (i = 0; i < 5; i++){
		if (compress_connect_matrix[i] == 1){
			tmp |= ((INT8U)(1 << (4 - i))); 	  
		  }
	  }
	  packet[20] = tmp;
	  tmp = 0;
	  for (; i < 45; i++){
		  if (compress_connect_matrix[i] == 1){
			  if ((i % 8) > 4){
				  tmp |= ((INT8U)(1 << (12 - i % 8)));
			  }
			  else{
				  tmp |= ((INT8U)(1 << (4 - i % 8)));
			  }
		  }
		  if (i % 8 == 4){
			  packet[20 + i / 8] = tmp;
			  tmp = 0;
		  }
	  }
  }
	  
  



BOOLEAN all_in_wide_beam()
{
  return (node_antenna_state[0].beam == WIDE_BEAM)
      && (node_antenna_state[1].beam == WIDE_BEAM)
      && (node_antenna_state[2].beam == WIDE_BEAM)
      && (node_antenna_state[3].beam == WIDE_BEAM);
}

void variable_initialize()
{
  INT8U i, j;
  for (i = 0; i < NUM_OF_NODES; ++i)
  {
    neighbor_node[i].connect = 0;
    neighbor_node[i].from_antenna = INITIALIZE_CONN_ANTENNA;
  }

  rts_flag = 0;
  cts_flag = 0;
  cts_antenna = 10;
 
  end_send = 0;

  waitDATA_flag = 0;
  waitACK_flag = 0;
  resend_data_num = 3;
 
  for (i = 0; i < NUM_OF_NODES; ++i)
  {
	  kNodesInfo[i].x = 0;
	  kNodesInfo[i].y = 0;
	  kNodesInfo[i].z = 0;

	  kNodesInfo[i].x_speed = 0;
	  kNodesInfo[i].y_speed = 0;
	  kNodesInfo[i].z_speed = 0;

	node_time[i] = 0;
  }

  for (i = 0; i < NUM_OF_NODES; ++i)
  {
    for (j = 0; j < NUM_OF_NODES; ++j)
    {
      connect_matrix[i][j].connect_ = 0;
      connect_matrix[i][j].let_ = 0.0;
    }
  }

  for (i = 0; i < NUM_OF_NODES; i++)
  {
	  time_up[i] = 0;
  }

  for (i = 0; i < 4; ++i)
  {
    node_antenna_state[i].beam = WIDE_BEAM;
    node_antenna_state[i].recv_or_send = RS_DEFAULT;
    node_antenna_state[i].mode = MODE_DEFAULT;
  }

  current_narrow_antenna = CUR_ANTENNA_DEFAULT;

  connect_matrix_changed = CONN_MATRIX_UNCHANGED;
}
//带LVDS头
//send_to_lvds(INT8U* header,INT32U header_len,INT8U* data,INT32U data_len)
void set_wide_beam(INT8U* command, INT32U command_len){
    send_to_lvds(command,command_len,NULL,0);

}
void set_narrow_beam(INT8U* command, INT32U command_len){
    send_to_lvds(command,command_len,NULL,0);
}
void set_link(INT8U* command, INT32U command_len){
	send_to_lvds(command, command_len, NULL, 0);
}
//不带LVDS头 消息类型0x18
void send_on_wide_beam(INT8U* node_msg, INT8U msg_len, INT8U from_antenna, INT8U send_mode){
  INT8U com[5];
  com[0] = WIDE_BEAM_MSG_SEND_TYPE;
  com[1] = 0x00;
  com[2] = 0x00;
  com[3] = msg_len + 1;
  com[4] = 0x00 | (from_antenna << 4) | (send_mode << 3);
  send_to_lvds(com, 5, node_msg, msg_len);
}
//不带LVDS头 消息类型0x20
void send_on_narrow_beam(INT8U* pkt, INT32U pkt_len, INT8U from_antenna, INT8U send_mode){
  INT8U com[5];
  com[0] = NARROW_BEAM_MSG_SEND_TYPE;
  com[1] = (pkt_len >> 16) & 0xff;
  com[2] = (pkt_len >> 8) & 0xff;
  com[3] = pkt_len & 0xff;
  com[4] = (from_antenna << 4 | 0x00) | ((send_mode) << 3) | 0x00;
  send_to_lvds(com, 5, pkt, pkt_len - 1);
}


void set_all_antenna_wide()
{
	INT8U wide_set[Set_Wide_Beam_Length];
	INT8U i;
	INT8U antenna;
	for (i = 0; i < 4; ++i)
	{
		antenna = i;
		generate_wide_set(wide_set, 1, antenna);
		set_wide_beam(wide_set, Set_Wide_Beam_Length);
	}
}

int get_antenna_index(INT8U antenna_id)
{
	switch (antenna_id)
	{
	case 0x01:
		return 0;
		break;
	case 0x02:
		return 1;
		break;
	case 0x04:
		return 2;
		break;
	case 0x08:
		return 3;
		break;
	default:
		return -1;
		break;
	}
}

/***********
Input:待预测节点ID
Output:预测节点的经度
***********/
FP32 pred_lon(INT8U nodeID)
{
	FP32 result;
	FP32 dist = getNodeInfo(nodeID).z_speed * (node_time[THIS_NODE] - node_time[nodeID]) / 1000;
	result = (dist * 180) / (pi * R_EARTH * cos(getNodeInfo(nodeID).y));
	return result + getNodeInfo(nodeID).x;
}

/***********
Input:待预测节点ID
Output:预测节点的纬度
***********/
FP32 pred_lat(INT8U nodeID)
{
	FP32 result;
	FP32 dist = getNodeInfo(nodeID).x_speed * (node_time[THIS_NODE] - node_time[nodeID]) / 1000;
	result = (dist * 180) / (pi * R_EARTH);
	return result + getNodeInfo(nodeID).y;
}

/***********
Input:待预测节点ID
Output:预测节点的高度
***********/
INT16U pred_height(INT8U nodeID)
{
	INT16U result = getNodeInfo(nodeID).y_speed * (node_time[THIS_NODE] - node_time[nodeID]) / 1000;
	return  getNodeInfo(nodeID).z + result;
}


/***********
Input:待预测节点ID
Output:预测节点的方位角
***********/
FP32 get_FW_angle(INT8U nodeID)
{
	FP32 delta_lon = getNodeInfo(THIS_NODE).x - pred_lon(nodeID);
	FP32 delta_lat = getNodeInfo(THIS_NODE).y - pred_lat(nodeID);

	FP32 delta_east = (R_EARTH * cos(getNodeInfo(THIS_NODE).y) * pi * delta_lon) / 180;
	FP32 delta_north = (R_EARTH * pi * delta_lat) / 180;
	INT16U delta_height = getNodeInfo(THIS_NODE).z - pred_height(nodeID);

	return atan2f(delta_east, delta_north);
}

/***********
Input:待预测节点ID
Output:预测节点的离轴角
***********/
FP32 get_LZ_angle(INT8U nodeID)
{
	FP32 delta_lon = getNodeInfo(THIS_NODE).x - pred_lon(nodeID);
	FP32 delta_lat = getNodeInfo(THIS_NODE).y - pred_lat(nodeID);

	FP32 delta_east = (R_EARTH * cos(getNodeInfo(THIS_NODE).y) * pi * delta_lon) / 180;
	FP32 delta_north = (R_EARTH * pi * delta_lat) / 180;
	INT16U delta_height = getNodeInfo(THIS_NODE).z - pred_height(nodeID);

	return atan2f(sqrt(delta_east * delta_east + delta_north * delta_north), delta_height);
}

FP32 get_distance(INT8U nodeID)
{
	FP32 this_lon = getNodeInfo(THIS_NODE).x;
	FP32 this_lat = getNodeInfo(THIS_NODE).y;
	FP32 dest_lon = getNodeInfo(nodeID).x;
	FP32 dest_lat = getNodeInfo(nodeID).y;

	FP32 delta_lon = fabs(this_lon - dest_lon);
	FP32 delta_lat = fabs(this_lat - dest_lat);
	FP32 result = pow(sin(delta_lat / 2), 2) + cos(this_lat) * cos(dest_lat) * pow(sin(delta_lon / 2), 2);
	return result;
}



