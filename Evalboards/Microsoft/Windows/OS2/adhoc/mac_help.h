#ifndef MAC_HELP_H
#define MAC_HELP_H

//#include "types.h"
#include "includes.h"
#include "vardef.h"


// 节点消息长度
#define NODE_MSG_SIZE 26

// 宽窄波束设置命令长度
#define Set_Wide_Beam_Length 5


//定义天线ID
#define FORWARD_ANTENNA 1
#define RIGHT_ANTENNA 2
#define LEFT_ANTENNA 4
#define BACK_ANTENNA 8

//定义宽波束发送模式
#define DIRECT_SEND 0
#define INRP_SEND 1


//定义宽波束消息发送命令消息类型
#define WIDE_BEAM_MSG_SEND_TYPE 0xA5

//定义窄波束消息发送命令消息类型
#define NARROW_BEAM_MSG_SEND_TYPE 0xA6


#define DATA_TYPE             0x3
#define END_TYPE              0x4
#define ACK_NORMAL_TYPE       0x84
#define ACK_OVERFLOW_TYPE     0x86
#define CTS_TYPE              0x88



#define SET_WIDE_BEAM_TYPE    0xA2
#define SET_NARROW_BEAM_TYPE   0xA3
#define SET_LINK_TYPE	0xA7

#define pi 3.1415926
#define R_EARTH 6371

//天线ID到数组序号的映射
int get_antenna_index(INT8U antenna_id);

// 生成END消息
INT8U generate_end_msg();

// 生成CTS消息
INT8U generate_cts_msg();

// 生成ACK消息
INT8U generate_ack_normal_msg();

// 生成ACK缓存满消息
INT8U generate_ack_overflow_msg();

/* 生成LVDS宽波束设置命令
 * command:指向设置命令的指针
 * len:数据部分长度
 * antenna：设置天线
 */
void generate_wide_set(INT8U* command, INT8U len, INT8U antenna);

/*
 * @command：指向设置命令帧头
 * @len：数据部分的长度
 */
void generate_narrow_set(INT8U* command, INT8U len, INT8U antenna, INT8U to_node);

//link_mode:0表示单向连续发射，1表示单向连续接收
void generate_link_set(INT8U* command, INT8U len, INT8U antenna, INT8U link_mode);

// 得到节点的运动状态
void get_move_info(move_info* move);

// 得到节点的速度状态
void get_speed_info(speed_info* speed);

// 辅助函数，按字节打印数据包
void printStringByte(INT8U* s, INT32U len);

// 生成需要发送的节点信息
void generate_node_msg(INT8U* packet, INT8U rts, INT8U recv_node);

// 判断是否所有天线都处于宽波束状态
// 是返回1，否返回0
BOOLEAN all_in_wide_beam();

// 变量初始化
void variable_initailize();

void set_wide_beam(INT8U* command, INT32U command_len);
void set_narrow_beam(INT8U* command, INT32U command_len);
void set_link(INT8U* command, INT32U command_len);
void send_on_wide_beam(INT8U* node_msg, INT8U msg_len, INT8U from_antenna, INT8U send_mode);
void send_on_narrow_beam(INT8U* pkt, INT32U pkt_len, INT8U from_antenna, INT8U send_mode);


void print_node_move();
void print_neighbor_node();
void print_connect_matrix();

FP32 pred_lon(INT8U nodeID);
FP32 pred_lat(INT8U nodeID);
INT16U pred_height(INT8U nodeID);
FP32 get_FW_angle(INT8U nodeID);
FP32 get_LZ_angle(INT8U nodeID);
FP32 get_distance(INT8U nodeID);
#endif

