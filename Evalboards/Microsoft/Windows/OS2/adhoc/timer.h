#ifndef TIMER_H
#define TIMER_H
#include "includes.h"

#define TASK_STK_SIZE    128
#define TIMER_NUMBER 14

#define WideBeam_Time 100
#define Neighbour_Time 500
#define Data_Time 200
#define Ack_Time 200


typedef struct Timer_Info{
	INT8U no;
	INT8U prior;
	INT16U time;
}TIMER_INFO;

void set_timer(INT16U time,INT8U no);
void cancle_timer(INT8U no);
void new_timer(void *pdata);
#endif