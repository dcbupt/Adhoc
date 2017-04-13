#include "publicstorage.h"
#include "MarsOSAPI.h"
#include "log.h"
NodeInfo kNodesInfo[NUM_OF_NODES];

void updateNodeInfo(INT8U no,FP32 x,FP32 y,FP32 z,INT16U x_speed,INT16U y_speed,INT16U z_speed,INT16U time){
	if (no >= NUM_OF_NODES || no < 0){
		PrintfLog(LOG_ERROR,"File:publicstorage\nFunc:updateNodeInfo\nWrong no,the no is %d.",no);
		getchar();
		exit(1);
	}
	MARS_ENTER_CRITICAL();
	kNodesInfo[no].x = x;
	kNodesInfo[no].y = y;
	kNodesInfo[no].z = z;
	kNodesInfo[no].x_speed = x_speed;
	kNodesInfo[no].y_speed = y_speed;
	kNodesInfo[no].z_speed = z_speed;
	kNodesInfo[no].time = time;
	MARS_EXIT_CRITICAL();
}

NodeInfo getNodeInfo(int no){
	NodeInfo result = {0,0,0,0,0,0,0};
	if (no >= NUM_OF_NODES || no < 0){
		PrintfLog(LOG_ERROR, "File:publicstorage\nFunc:getNodeInfo\nWrong no,the no is %d.",no);
		getchar();
		exit(1);
	}
	MARS_ENTER_CRITICAL();
	result = kNodesInfo[no];
	MARS_EXIT_CRITICAL();
	return kNodesInfo[no];
}