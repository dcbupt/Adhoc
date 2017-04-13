#include "init_node.h"
#include "log.h"

INT32U THIS_NODE;

void init_Node(){
	if ((out = fopen("log.txt", "w")) == NULL){
		PrintfLog(LOG_ERROR, "cant open log.txt\n");
		return;
	}
	
	FILE* fp;
	fp = fopen("init_node.config","r");
	if (fp == NULL){
		PrintfLog(LOG_ERROR,"Open init_node.config error!\n");
		return;
	}
	fscanf(fp, "%d", &THIS_NODE);
	PrintfLog(LOG_DEBUG, "\^^^this node = %d\^^^\n", THIS_NODE);
	fclose(fp);
}