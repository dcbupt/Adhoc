#ifndef MEMMAN_H
#define MEMMAN_H
#include "includes.h"

//存储数据包的内存区域，数据包包含两种，一种是窄波束数据包（包含链路层头部，网络层头部，应用层数据），一种是宽波束数据包（即宽波束组网信息帧）
#define DatapackageCount 20
#define DatapackageLen 100+1048576

#define BufferInfoCount 70
#define BufferInfoLen   20

#define SmallAmountOfDataCount 20
#define SmallAmountOfDataLen   50

#define DatapackageMemType 1
#define BufferInfoMemType 2
#define SmallAmoutOfDataMemType 3

void init_Mem();
void init_DatapackageMem();
void init_BufferInfoMem();
void init_SmallAmountOfDataMem();

void* mem_Malloc(INT8U type);
void mem_Free(INT8U type,void *pblk);

#endif