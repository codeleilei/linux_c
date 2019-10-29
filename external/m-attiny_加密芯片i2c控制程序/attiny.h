/*******************************************************************************
 * 工程名称:VNet10新一代工程固件
 * 模块名称:APP/m-attiny
 * 文件名称:attiny.h
 * 创建日期:2014/12/01
 * 创建作者:系统工程嵌入式软件组 韩伟浩
 * 实现功能:对数据库操作的一些函数进行声明
 * 备注:
*******************************************************************************/
#ifndef _VNET_ATTINY_
#define _VNET_ATTINY_

/**********************************包含头文件************************************/
#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
//#include "../configure.h"


/*********************************结构类型定义***********************************/
//芯片中存储元素信息
typedef enum {
	production_date = 0,
	machine_type = 1,
	machine_sn,
	registration_code,
	batt_type
} ATtinyEnum_t;

/*********************************函数声明**************************************/
int Send_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len, char action);
int Receive_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len, char action);
int Read_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len);
int Write_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len);

/*最终函数*/
int Read_VNet_AT(ATtinyEnum_t type, unsigned char *data);
int Write_VNet_AT(ATtinyEnum_t type, unsigned char *data);

#endif
/**********************************文件结束**************************************/
