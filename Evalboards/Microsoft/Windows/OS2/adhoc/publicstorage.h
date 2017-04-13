#ifndef PUBLICSTORAGE_H_
#define PUBLICSTORAGE_H_

#include "includes.h"
#include "vardef.h"

typedef struct nodeInfo{
	FP32 x;
	FP32 y;
	FP32 z;
	INT16U x_speed;
	INT16U y_speed;
	INT16U z_speed;
	INT16U time;
}NodeInfo;

extern NodeInfo kNodesInfo[NUM_OF_NODES];

void updateNodeInfo(INT8U no, FP32 x, FP32 y, FP32 z, INT16U x_speed, INT16U y_speed, INT16U z_speed, INT16U time);
NodeInfo getNodeInfo(int no);

#endif