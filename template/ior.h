/*******************************************
# Author        : Retuze 
# Date          : 2023-11-09 19:44:20
# LastEditors   : Retuze 
# LastEditTime  : 2023-11-09 19:51:53
# Description   : 
*********************************************/
#ifndef _IOR_H_
#define _IOR_H_

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int _isatty(int fd);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char *ptr, int len);
int _fstat(int fd, struct stat *st);
int _getpid(void);
int _kill(int pid, int sig);
int _write(int fd, char *pBuffer, int size);

#endif // _IOR_H_