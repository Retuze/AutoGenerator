/*******************************************
# Author        : Retuze
# Date          : 2024-01-14 03:58:12
# LastEditors   : Retuze
# LastEditTime  : 2024-01-14 04:28:35
# Description   :
*********************************************/
#ifndef _SHELL_H_
#define _SHELL_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <string.h>
#include <malloc.h>
#include <stdint.h>

#include "list.h"
#include "partfrom.h"

#ifdef __cplusplus
}
#endif

class SHELL
{
private:
    struct cmd_list
    {
        const char *cmd;
        const char *help;
        void (*fun)(uint8_t argc, char *argv[]);
        struct list_head list;
    };
    struct list_head entry;
    uint8_t split_string(char *str, const char *delim, char ***pargv)
    {
        char *token = nullptr;
        uint8_t idx = 0;
        token = strtok(str, delim);
        while (token)
        {
            if (idx >= 3)
            {
                *pargv = (char **)realloc(*pargv, (idx + 1) * sizeof(char *));
            }
            (*pargv)[idx++] = token;
            token = strtok(nullptr, delim);
        }
        return idx;
    }

public:
    SHELL()
    {
        INIT_LIST_HEAD(&entry);
    }
    void add_cmd(const char *str, void (*fun)(uint8_t argc, char *argv[]))
    {
        auto obj = (struct cmd_list *)malloc(sizeof(struct cmd_list));
        obj->help = NULL;
        obj->fun = fun;
        obj->cmd = str;
        list_add(&obj->list, &entry);
    }
    void add_cmd(const char *str, const char *help_str, void (*fun)(uint8_t argc, char *argv[]))
    {
        auto obj = (struct cmd_list *)malloc(sizeof(struct cmd_list));
        obj->fun = fun;
        obj->cmd = str;
        obj->help = help_str;
        list_add(&obj->list, &entry);
    }

    /// @brief 该函数会改变str，如果不希望str改变请使用exec(const char *str)
    /// @param str 
    void exec(char *str)
    {
        auto argv = (char **)malloc(sizeof(char *) * 3);
        auto argc = split_string(str, " (),", &argv);
        if (strcmp(argv[0], "help") == 0)
        {
            ulog("\n支持的命令：\n");
            struct cmd_list *node = NULL;
            list_for_each_entry(node, &entry, list)
            {
                if (node->help == NULL)
                {
                    ulog(" %-12s\n", node->cmd);
                }
                else
                {
                    ulog(" %-12s%s\n", node->cmd, node->help);
                }
            }
            free(argv);
            return;
        }
        struct cmd_list *node = NULL;
        list_for_each_entry(node, &entry, list)
        {
            if (strcmp(argv[0], node->cmd) == 0)
            {
                node->fun(argc, argv);
                free(argv);
                return;
            }
        }
        ulog("unkonw cmd %s !!!\n", argv[0]);
        free(argv);
    }

    /// @brief 安全的命令解析函数
    /// @param str 
    void exec(const char *str)
    {
        auto temp = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(temp, str);
        exec(temp);
        free(temp);
    }
};

#define CMD_ARG uint8_t argc, char *argv[]

extern SHELL shell;

#endif // _SHELL_H_