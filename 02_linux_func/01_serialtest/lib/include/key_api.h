#include <stdio.h>
#include <pthread.h>
#include <linux/input.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define ALARM_LENGTH 	20
#define OFF_VALUE 	0 
#define ON_VALUE 	1 

#define KEY_FIFO_NAME  	"/tmp/key_fifo" 
#define KEY_FIFO_EXITSTR 	"exit" 
#define KEY_FIFO_EXITSIZE 	5 
typedef int(*FUNC_P)();

struct key_node{
	unsigned int key_time;  //按键按下所以时长
	int reboot_time;
	int recover_time;
	int fd;
	int reboot_flag;   //是否开启重启功能标识
	int recover_flag;  //是否开启恢复功能标识
	int key_pipe_fd;
	FUNC_P reboot_func;  //重启处理函数指针
	FUNC_P recover_func;  //恢复处理函数指针
};

typedef struct input_event INPUT_EVENT;


/**************************************************************************************
** 函数名称:		keyInit
** 功能描述:		开启按键监听线程
** 输　入  : 		fd			打开的按键节点的文件描述符符                      
** 输　出  : 		无
**************************************************************************************/
void keyInit(int fd);

/**************************************************************************************
** 函数名称:		keyExit
** 功能描述:		关闭按键监听线程
** 输　出  : 		0, 错误返回 -1
**************************************************************************************/
int keyExit();

/**************************************************************************************
** 函数名称:		keyRebootSetup
** 功能描述:		设置重启时长，并触发重启处理函数
** 输　入  : 		s				触发重启的时长值,单位s  
**				reboot_func		重启的处理函数指针
** 输　出  : 		无
**************************************************************************************/
void keyRebootSetup(int s,FUNC_P reboot_func);


/**************************************************************************************
** 函数名称:		keyRecoverSetup
** 功能描述:		设置软件恢复时长，并触发软件恢复处理函数
** 输　入  : 		s				触发软件恢复的时长值,单位s  
**				recover_func	软件恢复的处理函数指针
** 输　出  : 		无
**************************************************************************************/
void keyRecoverSetup(int s,FUNC_P recover_func);


