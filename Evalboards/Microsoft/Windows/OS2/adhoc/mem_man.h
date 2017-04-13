#ifndef MEMMAN_H
#define MEMMAN_H
#include "includes.h"

//�洢���ݰ����ڴ��������ݰ��������֣�һ����խ�������ݰ���������·��ͷ���������ͷ����Ӧ�ò����ݣ���һ���ǿ������ݰ���������������Ϣ֡��
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