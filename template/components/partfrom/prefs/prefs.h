/*******************************************
# Author        : Retuze
# Date          : 2024-01-11 21:03:56
# LastEditors   : Retuze
# LastEditTime  : 2024-01-13 15:07:19
# Description   :
*********************************************/
#ifndef _PREFS_H_
#define _PREFS_H_

#include "partfrom.h"
#include "stdint.h"
#include "main.h"

// PREFS 使用的flash大小，单位KB
#define PREFS_SIZE 1
// PREFS flash起始地址
#define PREFS_ADDR_BEGIN (0x800C000 - 1024)
#define PREFS_ADDR_END (PREFS_ADDR_BEGIN + PREFS_SIZE * 1024)

// 数据保存在flash中的样子
// struct data
// {
//     uint32_t hash;//保存key对应的hash值
//     uint16_t flag_len;//保存该数据是否有效的标志位，以及数据的长度
//     uint16_t *val;//保存的数据本体
// };

#ifdef __cplusplus
extern "C"
{
#endif

void pf_eraseAll();
uint8_t pf_putChar(const char *key, uint8_t value);
uint8_t pf_putShort(const char *key, uint16_t value);
uint8_t pf_putInt(const char *key, uint32_t value);
uint8_t pf_putFloat(const char *key, float value);
uint8_t pf_isKey(const char *key);
uint8_t pf_getChar(const char *key);
uint16_t pf_getShort(const char *key);
uint32_t pf_getInt(const char *key);
float pf_getFloat(const char *key);
uint8_t pf_removeItem(const char *key);
size_t pf_getFreeByte();
size_t pf_getAllByte();
size_t pf_getUsedByte();
size_t pf_getItemCount();
size_t pf_getWriteCount();
void pf_eraseAll();

#ifdef __cplusplus
}

#endif

#endif // _PREFS_H_