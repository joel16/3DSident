#ifndef _3DSIDENT_COMMON_H_
#define _3DSIDENT_COMMON_H_

#include <setjmp.h>

#define wait(msec) svcSleepThread(10000000 * (s64)msec)

jmp_buf exitJmp;

#endif