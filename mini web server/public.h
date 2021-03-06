#ifndef Public_H
#define Public_H
/***********************************************
* 本文件包含Mini-Webserver的基础函数
* 作者：王志军
* 日期：2017.4.1
************************************************/

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1
#define false 0

#define RIO_BUFSIZE 4096
typedef struct sockaddr SA;

/*健壮I/O缓冲区结构*/
typedef struct{
    int rio_fd;
    size_t rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;


/************************************************
* 函数功能：socket函数、bind函数与listen函数的
            包装函数,创建服务器的监听描述符。
* 输入参数：port -- 服务器端口
* 输出参数：无
*   返回值：listenFd -- 成功则返回监听描述符
                  -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.1
* 修改备注：无
************************************************/
int openListenFd(int port);

/************************************************
* openListenFd函数的包装函数，自动进行错误处理。
************************************************/
int OpenListenFd(int port);


/************************************************
* 函数功能：accept函数的包装函数，自动进行错误处理。
* 输入参数：listenFd -- 监听描述符
          *clientLen -- 套接字地址结构大小
* 输出参数：*clientAddr -- 客户端套接字地址
*   返回值：connectFd -- 成功则返回已连接描述符
                   -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.1
* 修改备注：无
************************************************/
int Accept(int listenFd, struct sockaddr *clientAddr, int *clientLen);


/************************************************
* 函数功能：错误处理
* 输入参数：*msg -- 错误信息
* 输出参数：无
*   返回值：无
*   创建人：王志军
* 创建时间：2017.4.1
* 修改备注：无
************************************************/
void error(char *msg);


/************************************************
* 函数功能：open函数的包装函数，打开文件。
************************************************/
int Open(char *filename, int flags, mode_t mode);



/************************************************
* 函数功能：close函数包装函数，关闭描述符
************************************************/
void Close(int connectFd);


/************************************************
* 函数功能：初始化健壮I/O的读缓冲区，并将其与指定
            描述符绑定。
* 输入参数： fd -- 描述符
* 输出参数：*rp -- 初始化之后的健壮I/O缓冲区结构
*   返回值：无
*   创建人：王志军
* 创建时间：2017.4.2
* 修改备注：无
************************************************/
void initReadBuf(rio_t *rp, int fd);

/************************************************
* initReadBuf函数的包装函数，自动进行错误处理。
************************************************/
void InitReadBuf(rio_t *rp, int fd);

/************************************************
* 函数功能：健壮的带缓冲区的读函数
* 输入参数：*rp -- 已与描述符绑定的健壮I/O缓冲区
              n -- 读出字节数
* 输出参数：*buf -- 读出的数据
*   返回值：nbytes -- 成功则返回实际读出的字节数
            0 -- EOF
           -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.2
* 修改备注：无
************************************************/
size_t rioRead(rio_t *rp, char *buf, size_t n);

/************************************************
* rioRead函数的包装函数，自动进行错误处理。
************************************************/
size_t RioRead(rio_t *rp, char *buf, size_t n);


/************************************************
* 函数功能：以rioRead函数为基础从缓冲区读取n字节。
* 输入参数：*rp -- 已与描述符绑定的健壮I/O缓冲区
              n -- 读出字节数
* 输出参数：*buf -- 读出的数据
*   返回值：nbytes -- 成功则返回实际读出的字节数
            -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.3
* 修改备注：无
************************************************/
size_t rioReadN(rio_t *rp, char *buf, size_t n);

/************************************************
* rioRead函数的包装函数，自动进行错误处理。
************************************************/
size_t RioReadN(rio_t *rp, char *buf, size_t n);


/************************************************
* 函数功能：以rioRead函数为基础从缓冲区读取一行。
* 输入参数：*rp -- 已与描述符绑定的健壮I/O缓冲区
         maxLen -- 行最大长度
* 输出参数：*buf -- 读取的行
*   返回值：nbytes -- 成功则返回读出的字节数
           -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.2
* 修改备注：无
************************************************/
size_t rioReadLine(rio_t *rp, char *buf, size_t maxLen);

/************************************************
* rioReadLine函数的包装函数，自动进行错误处理。
************************************************/
size_t RioReadLine(rio_t *rp, char *buf, size_t maxLen);


/************************************************
* 函数功能：健壮的写函数,自动进行不足值处理。
* 输入参数：*rp -- 已与描述符绑定的健壮I/O缓冲区
           *buf -- 待写数据
              n -- 写入字节数
* 输出参数：无
*   返回值：nbytes -- 成功则返回实际写入的字节数
            -1 -- 失败
*   创建人：王志军
* 创建时间：2017.4.3
* 修改备注：无
************************************************/
size_t rioWriteN(int fd, const char *buf, size_t n);

/************************************************
* rioWriteN函数的包装函数，自动进行错误处理。
************************************************/
size_t RioWriteN(int fd, const char *buf, size_t n);

/************************************************
* 函数功能：mmap函数包装函数，将指定文件的指定区
            域映射到虚拟地址空间并进行错误处理。
* 输入参数：*start -- 期望分配的起始地址
            length -- 欲映射文件内容size
              port -- 新映射存储器区域访问权限
             flags -- 描述被映射对象类型
                fd -- 欲映射文件描述符
            offset -- 欲映射文件内部偏移
* 输出参数：无
*   返回值：*ptr -- 成功则返回指向映射区域的指针
*   创建人：王志军
* 创建时间：2017.4.3
* 修改备注：无
************************************************/
void *Mmap(void *start, size_t length, int port, int flags, int fd, off_t offset);


/************************************************
* 函数功能：munmap函数包装函数，与mmap对应，销毁
            指定虚拟存储器区域并进行错误处理。
* 输入参数：*start -- 欲销毁的起始地址
            length -- 欲销毁存储器区域size
* 输出参数：无
*   返回值：无
*   创建人：王志军
* 创建时间：2017.4.3
* 修改备注：无
************************************************/
void Munmap(void *start, size_t length);


/************************************************
* 函数功能：fork函数包装函数，自动进行错误处理。
************************************************/
int Fork();


/************************************************
* 函数功能：execve函数包装函数，自动进行错误处理。
************************************************/
void Execve(const char *filename, const char *argv[], const char *envp[]);


/************************************************
* 函数功能：dup2函数包装函数，自动进行错误处理。
************************************************/
void Dup2(int oldFd, int newFd);


/************************************************
* 函数功能：wait函数的包装函数，自动进行错误处理。
************************************************/
void Wait(int *status);

#endif /*Public_H*/
