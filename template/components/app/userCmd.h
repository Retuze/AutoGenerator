/*******************************************
# Author        : Retuze 
# Date          : 2024-01-14 04:36:51
# LastEditors   : Retuze 
# LastEditTime  : 2024-01-14 04:38:27
# Description   : 
*********************************************/
#ifndef _USERCMD_H_
#define _USERCMD_H_

#include "shell.h"
#include "task.h"
#include "prefs.h"
#include "MFRC522.h"


#ifdef __cplusplus
extern "C" {
#endif

static inline void RegisterCommand(void)
{
    // shell.add_cmd("pf_init","擦除prefs存储空间",[](CMD_ARG)
    // {
    //     task.creat([](){pf_eraseAll();});
    // });

    // shell.add_cmd("putChar","向prefs存储空间修改/添加一个char参数",[](CMD_ARG)
    // {
    //     if(argc==3)
    //     {
    //         pf_putChar(argv[1],atoi(argv[2]));
    //     }
    // });

    // shell.add_cmd("getChar","从prefs存储空间读取一个char参数",[](CMD_ARG)
    // {
    //     if(argc==2)
    //     {
    //         if(pf_isKey(argv[1])!=0)
    //         {
    //             ulog("该参数不存在\n");
    //         }
    //         else
    //         {
    //             ulog("%s = %d\n",argv[1],pf_getChar(argv[1]));
    //         }
    //     }
    // });

    // shell.add_cmd("getItem","查询prefs空间参数数量",[](CMD_ARG)
    // {
    //     ulog("prefs空间参数数量:%d\n",pf_getItemCount());
    // });

    // shell.add_cmd("getWrite","查询prefs空间写入参数次数",[](CMD_ARG)
    // {
    //     ulog("prefs空间写入参数次数:%d\n",pf_getWriteCount());
    // });

    // shell.add_cmd("getFree","查询prefs空间剩余字节数",[](CMD_ARG)
    // {
    //     ulog("prefs空间剩余字节数:%d\n",pf_getFreeByte());
    // });

    // shell.add_cmd("getUsed","查询prefs空间已使用字节数",[](CMD_ARG)
    // {
    //     ulog("prefs空间已使用字节数:%d\n",pf_getUsedByte());
    // });

    // shell.add_cmd("getAll","查询prefs空间总字节数",[](CMD_ARG)
    // {
    //     ulog("prefs空间总字节数:%d\n",pf_getAllByte());
    // });

    // shell.add_cmd("removeItem","删除prefs空间元素",[](CMD_ARG)
    // {
    //     if(argc==2)
    //     {
    //         ulog("删除prefs空间元素:%s\n",argv[1]);
    //         pf_removeItem(argv[1]);
    //     }
    // });

    shell.add_cmd("reboot","reboot [bootloader/recovery/app] 重启MCU",[](CMD_ARG){
        HAL_NVIC_SystemReset();
    });
extern MFRC522 mfrc522;
    shell.add_cmd("rfid_read","rfid_read 读取卡片信息",[](CMD_ARG){
        task.creat([](){
            mfrc522.read_card();
        },"rfid_read",-1);
    });

    shell.add_cmd("rfid_init","rfid_init [学号] [姓名] 激活卡片并录入信息",[](CMD_ARG){
        task.creat([](){},"rfid_init",-1);
    });

    shell.add_cmd("rfid_pay","rfid_pay [money] 付款",[](CMD_ARG){
        task.creat([](){},"rfid_pay",-1);
    });

    shell.add_cmd("rfid_recharge","rfid_recharge [money] 充值",[](CMD_ARG){
        task.creat([](){},"rfid_recharge",-1);
    });

    shell.add_cmd("task_show","task_show 显示正在执行的任务",[](CMD_ARG){
        task.show();
    });

    shell.add_cmd("task_remove","task_remove [task_name] 移除任务",[](CMD_ARG){
        task.remove(argv[1]);
    });
}

#ifdef __cplusplus
}
#endif

#endif // _USERCMD_H_

