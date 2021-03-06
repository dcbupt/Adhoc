#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#define App2NetEvent 1                 //事件类型为应用层向网络层发送数据包
#define Phy2MacOnWideEvent 2           //事件类型为从物理层收到宽波束数据包
#define Phy2MacOnNarrowEvent 3         //事件类型为从物理层收到窄波束数据包
#define SetWideBeamEvent 4             //事件类型为宽波束设置命令响应
#define SetNarrowBeamEvent 5           //事件类型为窄波束设置命令响应
#define InitTimerEvent 6               //事件类型为初始定时器到时
#define TimeUpEvent 7				   //邻居节点定时器到时
#define SetLinkEvent 8				   //事件类型为链路参数设置相应
#define DataTimeUpEvent 9			   //事件类型为等待DATA超时响应
#define AckTimeUpEvent 10			   //事件类型为等待ACK超时响应

#endif