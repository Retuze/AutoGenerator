/*******************************************
# Author        : Retuze
# Date          : 2024-01-13 15:21:36
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-13 16:38:58
# Description   :
*********************************************/
#include "prefs.h"
enum
{
    INVALID,
    VALID,
};
enum
{
    pf_OK,
    pf_NOT_ENOUGH_SPACE,
    pf_NOT_DIV_TWO,
    pf_UNKOW_ERROR,
};
// BKDR Hash Function
static uint32_t pf_BKDRHash(const char *str)
{
    uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
    uint32_t hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

// Erase Flash One Page
static void pf_eraseOnePage(uint32_t addr)
{
    ulog("正在擦除地址: %#8lX\n", addr);
    HAL_FLASH_Unlock();                                         // 解锁
    SET_BIT(FLASH->CR, FLASH_CR_PER);                           // 置位扇区擦除控制位
    WRITE_REG(FLASH->AR, addr);                                 // 对应扇区起始地址
    SET_BIT(FLASH->CR, FLASH_CR_STRT);                          // 开始扇区清除
    while ((FLASH->SR & (FLASH_SR_BSY | FLASH_SR_EOP)) != 0x20) // 等待擦除完毕
        ;
    FLASH->SR |= FLASH_SR_EOP; // 清除标志位
    HAL_FLASH_Lock();          // 重新上锁，擦除完成
    ulog("擦除成功\n");
}
static uint8_t pf_GC()
{
    ulog("正在进行空间回收\n");
    if (pf_getWriteCount() - pf_getItemCount())
    {
        ulog("正在读取所有有效数据\n");
        // read_all();
        uint16_t data[512] = {0};
        for (size_t i = 0; i < 512; i++)
        {
            data[i] = 0xffff;
        }
        uint32_t data_addr = 0;
        size_t current_addr = PREFS_ADDR_BEGIN;
        uint32_t hash = ((*(uint32_t *)(current_addr)));
        uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
        while (hash != 0xFFFFFFFF)
        {
            if (flag == VALID)
            {
                for (uint16_t i = 0; i < ((6 + len) / 2); i++)
                {
                    data[data_addr + i] = *(uint16_t *)(current_addr + i);
                }
                data_addr = data_addr + 6 + len;
            }
            current_addr = current_addr + 6 + len;
            if (current_addr > PREFS_ADDR_END)
            {
                // 超过了flash的大小
                break;
            }
            hash = ((*(uint32_t *)(current_addr)));
            flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
            len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
        }
        ulog("擦除存储空间\n");
        pf_eraseAll();
        ulog("正在写回数据\n");
        // write_all();
        HAL_FLASH_Unlock();
        for (size_t i = 0; i < data_addr / 2; i++)
        {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, PREFS_ADDR_BEGIN, data[i]);
        }
        HAL_FLASH_Lock();
        return pf_OK;
    }
    else
    {
        ulog("没有过期空间可供回收\n");
        return pf_NOT_ENOUGH_SPACE;
    }
}
static uint8_t pf_putItem(const char *key, void *val, uint32_t size)
{
    if (size % 2 != 0)
    {
        ulog("写入数据必须为2字节倍数\n");
        return pf_NOT_DIV_TWO;
    }
    if (pf_getFreeByte() < size)
    {
        // 剩余空间不足，尝试回收空间
        if (pf_GC())
        {
            // 回收空间完成
            if (pf_getFreeByte() < size)
            {
                // 回收后还是不足，拒绝写入
                ulog("剩余空间不足\n");
                return pf_NOT_ENOUGH_SPACE;
            }
        }
        else
        {
            // 回收空间失败，没有空间可以回收
            ulog("剩余空间不足\n");
            return pf_NOT_ENOUGH_SPACE;
        }
    }
    size_t current_addr = PREFS_ADDR_BEGIN;
    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        // 如果计算的hash值和flash读取的一致说明原来保存过该数据，则把原最新数据的标记改为过时标记
        if (hash == pf_BKDRHash(key) && flag == VALID)
        {
            HAL_FLASH_Unlock();
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, current_addr + 4, 0x0000);
            HAL_FLASH_Lock();
        }
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            return pf_UNKOW_ERROR;
        }
        hash = ((*(uint32_t *)(current_addr)));
        flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    HAL_FLASH_Unlock();
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, current_addr, pf_BKDRHash(key));
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, current_addr + 4, (1 << 15) + size);
    for (size_t i = 0; i < size / 2; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, current_addr + 6 + i * 2, *(uint16_t *)(val + i));
    }
    HAL_FLASH_Lock();
    ulog("写入成功\n");
    return pf_OK;
}

static uint8_t pf_getItem(const char *key, void **val)
{
    size_t current_addr = PREFS_ADDR_BEGIN;

    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        // 如果计算的hash值和flash读取的一致且flag标志为最新数据时读出
        if (hash == pf_BKDRHash(key) && flag == VALID)
        {
            *val = (uint16_t *)(current_addr + 6);
            return pf_OK;
        }
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            return pf_UNKOW_ERROR;
        }
        hash = ((*(uint32_t *)(current_addr)));
        flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return pf_UNKOW_ERROR;
}

// put开头的方法 返回0表示操作成功 返回非0值表示操作失败
uint8_t pf_putChar(const char *key, uint8_t value)
{
    uint16_t temp = value;
    uint16_t *val = &temp;
    return pf_putItem(key, val, 2);
}

uint8_t pf_putShort(const char *key, uint16_t value)
{
    uint16_t temp = value;
    uint16_t *val = &temp;
    return pf_putItem(key, val, 2);
}

uint8_t pf_putInt(const char *key, uint32_t value)
{
    uint32_t temp = value;
    uint32_t *val = &temp;
    return pf_putItem(key, val, 4);
}

uint8_t pf_putFloat(const char *key, float value)
{
    float temp = value;
    float *val = &temp;
    return pf_putItem(key, val, 4);
}

// 返回 0：找到，非0：没找到
uint8_t pf_isKey(const char *key)
{
    size_t current_addr = PREFS_ADDR_BEGIN;

    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        // 如果计算的hash值和flash读取的一致说明原来保存过该数据，则把原最新数据的标记改为过时标记
        if (hash == pf_BKDRHash(key) && flag == VALID)
        {
            return pf_OK;
        }
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            return pf_UNKOW_ERROR;
        }
        hash = ((*(uint32_t *)(current_addr)));
        flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return pf_UNKOW_ERROR;
}

uint8_t pf_getChar(const char *key)
{
    // 默认返回值为0
    uint8_t ret = 0;
    uint8_t *p_ret = &ret;
    pf_getItem(key, (void**)&p_ret);
    return *p_ret;
}

uint16_t pf_getShort(const char *key)
{
    // 默认返回值为0
    uint16_t ret = 0;
    uint16_t *p_ret = &ret;
    pf_getItem(key, (void**)&p_ret);
    return *p_ret;
}

uint32_t pf_getInt(const char *key)
{
    // 默认返回值为0
    uint32_t ret = 0;
    uint32_t *p_ret = &ret;
    pf_getItem(key, (void**)&p_ret);
    return *p_ret;
}

float pf_getFloat(const char *key)
{
    // 默认返回值为0
    float ret = 0;
    float *p_ret = &ret;
    pf_getItem(key, (void**)&p_ret);
    return *p_ret;
}

uint8_t pf_removeItem(const char *key)
{
    size_t current_addr = PREFS_ADDR_BEGIN;

    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        // 如果计算的hash值和flash读取的一致说明原来保存过该数据，则把原最新数据的标记改为过时标记
        if (hash == pf_BKDRHash(key) && flag == VALID)
        {
            HAL_FLASH_Unlock();
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, current_addr + 4, 0x0000);
            HAL_FLASH_Lock();
            return pf_OK;
        }
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            // 超过了flash的大小
            return pf_UNKOW_ERROR;
        }
        hash = ((*(uint32_t *)(current_addr)));
        flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return pf_UNKOW_ERROR;
}

size_t pf_getFreeByte()
{
    return pf_getAllByte() - pf_getUsedByte();
}

size_t pf_getAllByte()
{
    return PREFS_SIZE * 1024;
}

size_t pf_getUsedByte()
{
    size_t current_addr = PREFS_ADDR_BEGIN;

    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            current_addr = current_addr - 6 - len;
            // 超过了flash的大小
            break;
        }
        hash = ((*(uint32_t *)(current_addr)));
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return current_addr - PREFS_ADDR_BEGIN;
}
size_t pf_getItemCount()
{
    size_t count = 0;
    size_t current_addr = PREFS_ADDR_BEGIN;
    uint32_t hash = ((*(uint32_t *)(current_addr)));
    uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        if (flag == VALID)
        {
            count++;
        }
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            // 超过了flash的大小
            break;
        }
        hash = ((*(uint32_t *)(current_addr)));
        flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return count;
}
size_t pf_getWriteCount()
{
    size_t count = 0;
    size_t current_addr = PREFS_ADDR_BEGIN;
    uint32_t hash = ((*(uint32_t *)(current_addr)));
    // uint16_t flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
    uint16_t len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    while (hash != 0xFFFFFFFF)
    {
        count++;
        current_addr = current_addr + 6 + len;
        if (current_addr > PREFS_ADDR_END)
        {
            // 超过了flash的大小
            break;
        }
        hash = ((*(uint32_t *)(current_addr)));
        // flag = ((*(uint16_t *)(current_addr + 4))) >> 15;
        len = ((*(uint16_t *)(current_addr + 4))) & (0x7FFF);
    }
    return count;
}
void pf_eraseAll()
{
    for (size_t i = PREFS_ADDR_BEGIN; i < PREFS_ADDR_END; i = i + 1024)
    {
        pf_eraseOnePage(i);
    }
}