#ifndef MLOG_H_
#define MLOG_H_

#include"includes.h"
#define LOG_EN 1u
#define LOG_LEVEL 1u
#define LOG_ERROR 3u
#define LOG_INFO 2u
#define LOG_DEBUG 1u

extern FILE* out;
void PrintfLog(int level,char * fromat, ...);
#endif