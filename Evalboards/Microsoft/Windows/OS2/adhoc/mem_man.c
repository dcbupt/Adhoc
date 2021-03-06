#include "mem_man.h"

/*包含自己实现的MarsOSAPI,仅PC上仿真需要，移植时不需要*/
#include "MarsOSAPI.h"
/******************************************************/

#include "log.h"

INT8U Datapackage[DatapackageCount][DatapackageLen];
INT8U BufferInfo[BufferInfoCount][BufferInfoLen];
INT8U SmallAmoutOfData[SmallAmountOfDataCount][SmallAmountOfDataLen];

MARS_MEM* DatapackageMem, *BufferInfoMem, *SmallAmoutOfDataMem;

//初始化两个内存分区，一个是用来存储数据包，另一个用来存储链路层和网络之间的packet
void init_Mem(){
	init_DatapackageMem();
	init_BufferInfoMem();
	init_SmallAmountOfDataMem();
}
//初始化用来存储数据包的内存区域，主要是完成内存控制块DatapackageMem的初始化
void init_DatapackageMem(){
	INT8U err;
	DatapackageMem = MARSMemCreate(Datapackage, DatapackageCount, DatapackageLen, &err);
	if (err == MARS_ERR_NONE){
		PrintfLog(LOG_DEBUG, "Init datapcakagemem succeed!\n");
	}
	else{
		PrintfLog(LOG_ERROR, "Init datapcakagemem failed!\n");
	}
}

//初始化用来存储链路层和网络之间的packet的内存区域，主要是完成内存控制块BufferInfoMem的初始化
void init_BufferInfoMem(){
	INT8U err;
	BufferInfoMem = MARSMemCreate(BufferInfo, BufferInfoCount, BufferInfoLen, &err);
	if (err == MARS_ERR_NONE){
		PrintfLog(LOG_DEBUG, "Init bufferinfomem succeed!\n");
	}
	else{
		PrintfLog(LOG_ERROR, "Init bufferinfomem failed!\n");

	}
}
//初始化用来存储少量数据的内存区域，例如从lvds收到的宽波束数据，主要完成内存控制块SmallAmoutOfDataMem的初始化
void init_SmallAmountOfDataMem(){
	INT8U err;
	SmallAmoutOfDataMem = MARSMemCreate(SmallAmoutOfData, SmallAmountOfDataCount, SmallAmountOfDataLen, &err);
	if (err == MARS_ERR_NONE){
		PrintfLog(LOG_DEBUG, "Init SmallAmountOfData succeed!\n");
	}
	else{
		PrintfLog(LOG_ERROR, "Init SmallAmountOfData failed!\n");
	}
}
//完成对3块内存区域内存的动态申请
void* mem_Malloc(INT8U type){
	INT8U err = 10;
	char *mem_info[4] = {"","DatapackageMem","BufferInfoMem","SmallAmoutOfDataMem"};
	void *pblk = NULL;

	switch(type){
		case DatapackageMemType:
			pblk = MARSMemGet(DatapackageMem, &err);
			break;
		case BufferInfoMemType:
			pblk = MARSMemGet(BufferInfoMem, &err);
			break;
		case SmallAmoutOfDataMemType:
			pblk = MARSMemGet(SmallAmoutOfDataMem,&err);
			break;
		default:
			PrintfLog(LOG_ERROR, "Malloc unknown type\n");
			break;
	}
	if(err==MARS_ERR_NONE){
		//PrintfLog(LOG_DEBUG, "Malloc %s succeed\n", mem_info[type]);
	}
	else{
		//PrintfLog(LOG_ERROR, "Malloc %s failed!The wrong number is %d!\n", mem_info[type], err);
	}
	return pblk;
}
//完成对3块内存区域内存的释放
void mem_Free(INT8U type, void *pblk){
	INT8U err = 10;
	char *mem_info[4] = {"", "DatapackageMem", "BufferInfoMem","SmallAmoutOfDataMem"};
	if (pblk == NULL)
		return;

	switch(type){
		case DatapackageMemType:
			err=MARSMemPut(DatapackageMem,pblk);
			break;
		case BufferInfoMemType:
			err=MARSMemPut(BufferInfoMem,pblk);
			break;
		case SmallAmoutOfDataMemType:
			err = MARSMemPut(SmallAmoutOfDataMem,pblk);
			break;
		default:
			PrintfLog(LOG_ERROR, "Free unknown type\n");
			break;
	}
	if(err==MARS_ERR_NONE){
		//PrintfLog(LOG_DEBUG, "Free %s succeed\n", mem_info[type]);
	}
	else{
		//PrintfLog(LOG_ERROR, "Free %s succeed", "Free %s failed!The wrong number is %d!\n", mem_info[type], err);
	}
	return;
}