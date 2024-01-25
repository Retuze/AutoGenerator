/*******************************************
# Author        : Retuze 
# Date          : 2024-01-08 00:28:26
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 03:58:05
# Description   : 
*********************************************/
#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include "partfrom.h"

// 日志输出的数据，包括日志等级，日志主体，输出日志的文件名，行数，函数名
struct log_data
{
    va_list ap;
    const char *fmt;
    const char *file;
    const char *func;
    int line;
    int level;
};

// 虚拟的日志输出设备，有独立的日志等级（默认为全部输出），启用/禁用选项（默认启用），自旋锁（可选），输出数据结构体，输出方式
struct log_dev
{
    int level;
    bool quiet;
    void (*lock)(bool);
    struct log_data ld;
    void (*log_out)(struct log_data *);
};

enum
{
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

void log_set_level(struct log_dev *self, int level);
void log_set_quiet(struct log_dev *self, bool enable);
void log_set_out(struct log_dev *self, void (*log_out)(struct log_data *));
void log_set_lock(struct log_dev *self, void (*lock)(bool));
void log_log(struct log_dev *self, int level, const char *file, int line, const char *func, const char *fmt, ...);

#if (_WIN32 || WIN64)
#define GET_FILE_NAME(x) strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x
#else
#define GET_FILE_NAME(x) strrchr(x, '/') ? strrchr(x, '/') + 1 : x
#endif

extern struct log_dev uart_log;

#define log_trace(...) log_log(&uart_log, LOG_TRACE, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_debug(...) log_log(&uart_log, LOG_DEBUG, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_info(...) log_log(&uart_log, LOG_INFO, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_warn(...) log_log(&uart_log, LOG_WARN, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_log(&uart_log, LOG_ERROR, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)
#define log_fatal(...) log_log(&uart_log, LOG_FATAL, GET_FILE_NAME(__FILE__), __LINE__, __func__, __VA_ARGS__)


#ifdef __cplusplus
}
#endif

#endif // _LOG_H_