#ifndef VARIABLE_DEFINE_H
#define VARIABLE_DEFINE_H

#include "includes.h"

// 节点数目
#define NUM_OF_NODES  10

// 当前节点ID
//#define THIS_NODE     3


// **********记录天线状态的参数定义**********
// 天线处于宽波束或者窄波束
#define WIDE_BEAM     0
#define NARROW_BEAM   1

// 处于窄波束下的接收方或者发送方
#define RS_DEFAULT    0
#define RECEIVER      1
#define SENDER        2

#define MODE_DEFAULT                    0
#define NARROW_BEAM_SET_MODE            1
#define NARROW_BEAM_SEND_BEFORE_MODE    2
#define NARROW_BEAM_SEND_MODE           3
#define NARROW_BEAM_RECV_SET_MODE       4
#define NARROW_BEAM_RECV_MODE           5
#define NARROW_BEAM_SET_LINK			6


// **********记录当前哪个天线处于窄波束**********
#define FRONT_ANTENNA           0
#define LEFT_ANTENNA            1
#define RIGHT_ANTENNA           2
#define BACK_ANTENNA            3
#define CUR_ANTENNA_DEFAULT     5


// **********记录连通矩阵是否有变化**********
#define CONN_MATRIX_CHANGED     1
#define CONN_MATRIX_UNCHANGED   0


// **********用于初始化邻居列表的天线参数**********
#define INITIALIZE_CONN_ANTENNA 10

/*









typedef struct Connect_Info_T{
INT8U connect_;   //两节点是否连通，连通为1，不连通其他值0
FP64  let_;				//两节点间的LET值
} connect_info_t;

*/
typedef struct Connect_Info{
	INT8U connect;
	INT8U from_antenna;
} connect_info;

typedef struct Move_Info{
	FP32 x;
	FP32 y;
	INT16U z;
} move_info;

typedef struct Speed_Info{
	INT16U X_speed;
	INT16U Y_speed;
	INT16U Z_speed;
} speed_info;

typedef struct Antenna_State{
	INT8U beam;
	INT8U recv_or_send;
	INT8U mode;
} antenna_state;


typedef struct Connect_Info_T{
	INT8U connect_;
	INT8U  let_;
} connect_info_t;
#endif

