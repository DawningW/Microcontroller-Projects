#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "system.h"

// 不要做梦了, 这块开发板上的单片机只支持INT_0和INT_1 =_=
enum Ints { INT_0, INT_1, INT_2, INT_3 };

struct InterruptItem
{
    byte trigger; // 触发方式, 0为低电平触发, 1为下降沿触发
    void (*callback)(enum Ints); // 触发回调函数
};

extern void interrupt_init(enum Ints i, struct InterruptItem item);

#endif
