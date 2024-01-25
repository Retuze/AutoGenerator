/*******************************************
# Author        : Retuze
# Date          : 2024-01-07 20:18:18
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 03:57:52
# Description   :
*********************************************/
#include "log.h"

static const char *level_strings[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

#ifndef DISABLE_COLOR

static const char *level_colors[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"};

#endif

// 设置日志输出等级
void log_set_level(struct log_dev *self, int level)
{
    self->level = level;
}

// 设置是否启用日志
void log_set_quiet(struct log_dev *self, bool enable)
{
    self->quiet = enable;
}

// 设置日志输出方式
void log_set_out(struct log_dev *self, void (*log_out)(struct log_data *))
{
    self->log_out = log_out;
}

// 设置日志输出自旋锁
void log_set_lock(struct log_dev *self, void (*lock)(bool))
{
    self->lock = lock;
}

// 默认的输出方式
static void log_out(struct log_data *ld)
{
#ifndef DISABLE_COLOR
    ulog(
        "%s%-5s\x1b[0m \x1b[90m%s:%d [%s]:\x1b[0m ",
        level_colors[ld->level], level_strings[ld->level],
        ld->file, ld->line, ld->func);
#else
    ulog(
        "%-5s %s:%d [%s]: ",
        level_strings[ld->level], ld->file, ld->line, ld.func);
#endif
    vprintf(ld->fmt, ld->ap);
    ulog("\n");
}

void log_log(struct log_dev *self, int level, const char *file, int line, const char *func, const char *fmt, ...)
{
    self->ld.fmt = fmt;
    self->ld.file = file;
    self->ld.func = func;
    self->ld.line = line;
    self->ld.level = level;

    if (self->lock)
    {
        self->lock(true);
    }

    if (!self->quiet && level >= self->level)
    {
        va_start(self->ld.ap, fmt);
        self->log_out ? self->log_out(&(self->ld)) : log_out(&(self->ld));
        va_end(self->ld.ap);
    }

    if (self->lock)
    {
        self->lock(false);
    }
}

struct log_dev uart_log;