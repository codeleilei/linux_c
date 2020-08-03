#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 


#define EC20_PID_FILE "/tmp/ec20_pid_file"

#define AUTO		0
#define GSM_ONLY	1
#define WCDMA_ONLY	2
#define LTE_ONLY	3
#define TD_SCDMA_ONLY	4
#define UMTS_ONLY	5
#define CDMA_ONLY	6
#define HDR_ONLY	7
#define CDMA_AND_HDR_ONLY	8

/**************************************************************************************
** 函数名称:		ec20_open
** 功能描述:		开启ec20模块的ppp拨号
** 输　入  : 		无                    
** 输　出  : 		无
**************************************************************************************/
void ec20_open();

/**************************************************************************************
** 函数名称:		ec20_close
** 功能描述:		断开ec20模块的ppp拨号
** 输　入  : 		无                    
** 输　出  : 		无
**************************************************************************************/
void ec20_close();

//int ec20_netType_set(char *);

/**************************************************************************************
** 函数名称:		ec20_netType_get
** 功能描述:		执行AT命令: 	AT+COPS?
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_netType_get(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_networkStandard_get
** 功能描述:		执行AT命令: 	AT+QCFG="nwscanmode"
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_networkStandard_get(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_networkStandard_set
** 功能描述:		执行AT命令: 	AT+QCFG="nwscanmode",$(networkStandard)
** 输　入  : 		networkStandard	指定要设置的网络制式编码,编码含义可以参照上面的宏定义  
** 输　出  : 		0
**************************************************************************************/
int ec20_networkStandard_set(int networkStandard);

/**************************************************************************************
** 函数名称:		ec20_APN_get
** 功能描述:		执行AT命令: 	AT+CGDCONT?
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_APN_get(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_APN_set
** 功能描述:		执行AT命令: 	AT+CGDCONT=1,"IP",$(apn),,0,0
** 输　入  : 		apn				指定要设置的apn字符串指针  
** 输　出  : 		0
**************************************************************************************/
int ec20_APN_set(char* apn);

/**************************************************************************************
** 函数名称:		ec20_AT_ATI
** 功能描述:		执行AT命令: ATI
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_ATI(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_SIM
** 功能描述:		执行AT命令: AT+CCID
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_SIM(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_csq
** 功能描述:		执行AT命令: AT+CSQ
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_csq(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_stop_mode
** 功能描述:		执行AT命令: AT+CFUN=0
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_stop_mode(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_restart_mode
** 功能描述:		执行AT命令: AT+CFUN=1,1
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_restart_mode(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_airplane_mode
** 功能描述:		执行AT命令: AT+CFUN=4
** 输　入  : 		ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_airplane_mode(char *ret_buf,int ret_buf_size);

/**************************************************************************************
** 函数名称:		ec20_AT_command
** 功能描述:		执行AT命令: $(command)
** 输　入  : 		command			需要执行的AT命令字符串指针
**				ret_buf			用于接收AT命令返回信息的字符数组指针  
**				ret_buf_size	想要读取的字符个数
** 输　出  : 		读取到的字符个数
**************************************************************************************/
int ec20_AT_command(char * command,char *ret_buf,int ret_buf_size);




