/*******************************************
# Author        : Retuze
# Date          : 2023-12-30 15:54:45
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-11 03:58:49
# Description   :
*********************************************/
#ifndef _TASK_H_
#define _TASK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "string.h"

#include "partfrom.h"

#ifdef __cplusplus
}

class TASK
{
private:
    class taskList
    {
    public:
        taskList(int num, void (*fun)(void), int rept, const char *str = nullptr)
        {
            this->num = num;
            this->fun = fun;
            this->next = nullptr;
            this->rept = rept;
            if(str==nullptr)
            {
                this->str=nullptr;
            }
            else
            {
                this->str=(char *)malloc(strlen(str)+1);
                strcpy(this->str,str);
            }
        }
        int num;
        void (*fun)(void);
        int rept;
        char *str;
        taskList *next;
    };
    taskList head_data = taskList(0, nullptr, 0);

public:
    int creat(void (*fun)(void), int rept, const char *str = nullptr)
    {
        taskList *head = &head_data;
        int num = 1;
        while (head->next)
        {
            num++;
            head = head->next;
        }
        auto obj = new taskList(num, fun, rept, str);
        head->next = obj;
        return num;
    }
    void stop(int num)
    {
        taskList *head = &head_data;
        int i = 0;
        while (head->next && num != i)
        {
            i++;
            head = head->next;
        }
        head->rept = 0;
    }
    // #TODO 每创建一个任务都占用内存，记得要释放掉;
    // #TODO 应该是检测到执行完成自动释放;
    // #TODO 删除太麻烦了，下次再说;
    void remove(int num)
    {
    }
    void show(void)
    {
        taskList *head = &head_data;
        while (head->next != nullptr)
        {
            head = head->next;
            ulog("任务序号：%d 任务剩余执行次数：%d 任务说明：%s\n", head->num, head->rept, head->str);
        }
    }
    void run(void)
    {
        taskList *head = &head_data;
        while (head->next)
        {
            head = head->next;
            if (head->fun && head->rept != 0)
            {
                if (head->rept > 0)
                {
                    head->rept--;
                    head->fun();
                }
                else
                {
                    head->fun();
                }
            }
        }
    }
};

#endif

#endif // _TASK_H_