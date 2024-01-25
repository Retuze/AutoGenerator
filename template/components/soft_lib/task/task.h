/*******************************************
# Author        : Retuze
# Date          : 2023-12-30 15:54:45
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-14 03:45:49
# Description   :
*********************************************/
#ifndef _TASK_H_
#define _TASK_H_

#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "partfrom.h"

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}

class TASK
{
private:
    struct task_list
    {
        void (*fun)(void);
        int rept;
        char *str;
        struct list_head list;
    };
    struct list_head entry;
public:
    TASK()
    {
        INIT_LIST_HEAD(&entry);
    }
    void creat(void (*fun)(void), const char *str = NULL, int rept = 1)
    {
        auto obj = (struct task_list *)malloc(sizeof(struct task_list));
        obj->fun = fun;
        obj->rept = rept;
        if (str == NULL)
        {
            obj->str = NULL;
        }
        else
        {
            obj->str = (char *)malloc(strlen(str) + 1);
            strcpy(obj->str, str);
        }
        list_add(&obj->list, &entry);
    }
    void remove(const char *str)
    {
        struct task_list *node = NULL;
        struct task_list *n = NULL;
        list_for_each_entry_safe(node, n, &entry, list)
        {
            if (strcmp(node->str, str) == 0)
            {
                list_del(&node->list);
                free(node->str);
                free(node);
            }
        }
    }
    void show(void)
    {
        struct task_list *node = NULL;
        list_for_each_entry(node, &entry, list)
        {
            ulog("任务名称：%s 任务剩余执行次数：%d \n", node->str, node->rept);
        }
    }

    void run(void)
    {
        struct task_list *node = NULL;
        struct task_list *n = NULL;
        list_for_each_entry_safe(node, n, &entry, list)
        {
            if(node->rept>0)
            {
                node->rept--;
                node->fun();
            }
            else if(node->rept<0)
            {
                node->fun();
            }
            else
            {
                list_del(&node->list);
                free(node->str);
                free(node);
            }
        }
    }
};
extern TASK task;
#endif

#endif // _TASK_H_