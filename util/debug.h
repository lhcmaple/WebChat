#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef DEBUG
#include <stdio.h>
#define DBP(format,...) printf(format,##__VA_ARGS__)
#else
#define DBP(format,...)
#endif

#endif