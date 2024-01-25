/*******************************************
# Author        : Retuze
# Date          : 2023-11-01 01:40:22
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-09 01:39:54
# Description   :
*********************************************/
#ifndef _CMD_H_
#define _CMD_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <string.h>
#include <malloc.h>
#include <stdint.h>

#include "partfrom.h"


#ifdef __cplusplus
}
#endif

class CMD
{
private:
    class cmdList
    {
    public:
        cmdList *next;
        const char *cmd;
        const char *help;
        void (*fun)(uint8_t argc, char *argv[]);
        cmdList(const char *cmd, void (*fun)(uint8_t argc, char *argv[]))
        {

            this->cmd = cmd;
            this->fun = fun;
            this->help = NULL;
            next = nullptr;
        }
        cmdList(const char *cmd, const char *help, void (*fun)(uint8_t argc, char *argv[]))
        {
            this->cmd = cmd;
            this->fun = fun;
            this->help = help;
            next = nullptr;
        }
    };
    cmdList *tail = &root;


    cmdList root = cmdList("help", "列出所有支持的命令", [](uint8_t argc, char *argv[]) {});
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
    void addCmd(const char *str, void (*fun)(uint8_t argc, char *argv[]))
    {
        auto obj = new cmdList(str, fun);
        tail->next = obj;
        tail = obj;
    }
    void addCmd(const char *str, const char *help_str, void (*fun)(uint8_t argc, char *argv[]))
    {
        auto obj = new cmdList(str, help_str, fun);
        tail->next = obj;
        tail = obj;
    }
    void exec(const char *str)
    {
        auto temp = (char *)malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(temp, str);
        auto argv = (char **)malloc(sizeof(char *) * 3);
        auto argc = split_string(temp, " (),", &argv);
        auto head = &root;
        char down = 0;
        if (strcmp(argv[0], "help") == 0)
        {
            ulog("\n支持的命令：\n");
            while (head)
            {
                if (head->help == NULL)
                {
                    ulog(" %-10s\n", head->cmd);
                }
                else
                {
                    ulog(" %-10s%s\n", head->cmd, head->help);
                }
                head = head->next;
            }
            free(temp);
            free(argv);
            return;
        }
        while (head)
        {
            if (strcmp(argv[0], head->cmd) == 0)
            {
                head->fun(argc, argv);
                down = 1;
                break;
            }
            head = head->next;
        }
        if (down == 0)
        {
            ulog("unkonw cmd %s !!!\n", argv[0]);
        }
        free(temp);
        free(argv);
    }
};
#define CMD_ARG uint8_t argc, char *argv[]
#endif // _CMD_H_