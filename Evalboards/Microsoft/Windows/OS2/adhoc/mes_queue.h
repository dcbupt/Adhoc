#ifndef MESQUEUE_H_
#define MESQUEUE_H_

#include "includes.h"

#define Protocol_Prio 21

typedef struct Protocol_Message{
	INT8U mesType;       //判断消息类型
	INT32U len;          //data域数据长度，如果data域内容是协议处理的窄波束数据包（对应的事件类型是App2NetEvent，Phy2MacOnNarrowEvent），长度是链路层头部+网络层头部+应用层数据
	                     //                如果data域内容是的是接收到的宽波束数据（对应的事件类型是Phy2MacOnWideEvent），长度是链路层规定的宽波束数据包长度
	INT8U desID;         //应用层发数据，目的结点ID（对应的事件类型是App2NetEvent）
	INT8U memType;       //data对应的内存区域的类型，方便后面释放时使用
	void *data;          //如果事件类型是App2NetEvent或者Phy2MacOnNarrowEvent，数据代表的是窄波束数据包，包含链路层头部，网络层头部和应用层数据
}Protocol_Message;

/*
 * @len: 从lvds收到数据包的数据部分长度
 * @data: 指向从lvds收到的数据包的数据部分
 * @desID: 发送APP2NET消息时标识目的节点
 */
void sendMessageToProtocol(INT8U mesType, INT32U len, INT8U desID, INT8U memType, void *data);

#endif