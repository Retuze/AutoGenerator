#ifndef _PARTFROM_H_
#define _PARTFROM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tim.h"
#include "usart.h"

#include "printf.h"

#define ulog(...) PRINTF(__VA_ARGS__)

// #define UPRINTF

#ifdef UPRINTF

#include "u_printf.h"
#define PRINTF u_printf
#define VPRINTF u_vprintf
#define VSPRINTF u_vsprintf

#else
#include <stdio.h>
#define PRINTF printf
#define VPRINTF vprintf
#define VSPRINTF vsprintf

#endif

#ifdef __cplusplus
}

#endif

#endif // _PARTFROM_H_