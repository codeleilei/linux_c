#ifndef __UART_API_H
#define __UART_API_H

#ifndef __UART_API_H_INC__
    #define __UART_API_H_EXT__ extern
#else
    #define __UART_API_H_EXT__
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TIMEOUT                     1  /* read operation timeout 1s = TIMEOUT/10 */
#define MIN_LEN                     128  /* the min len datas */
#define ARRAY_SIZE(a)               (sizeof(a) / sizeof((a)[0]))

struct SERIAL_BAUD_ST{
    unsigned int lable;
    unsigned int baudrate;
};

struct SERIAL_ST{
    unsigned int fd;
	unsigned int baud;
    unsigned int databits;
    unsigned int stopbits;
    char parity;
};

static struct SERIAL_BAUD_ST g_attr_baud[] = {
    {230400, B230400}, {115200, B115200},
    {57600, B57600}, {38400, B38400}, 
    {19200, B19200}, {9600, B9600},
    {4800, B4800},  {2400, B2400},
    {1800, B1800}, {1200, B1200},
};

/**************************************************************************************
** 函数名称: serial_init
** 功能描述: 初始化serial
** 输　入  :dev         串口設備    eg：/dev/ttymxc3                  
**         baud        波特率      eg: 115200             
**         databits    數據位      eg: 8         
**         stopbits    停止位      eg: 1          
**         parity      校驗位      eg: n
** 输　出  : SERIAL_ST指針, 错误返回 NULL
**************************************************************************************/
struct SERIAL_ST* serial_init(const char *dev, int baud, int databits, int stopbits, char parity);

/**************************************************************************************
** 函数名称: serial_init_with_baud
** 功能描述: 初始化serial並設置波特率
** 输　入  :dev         串口設備    eg：/dev/ttymxc3                  
**         baud        波特率      eg: 115200             
** 输　出  : SERIAL_ST指針, 错误返回 NULL
**************************************************************************************/
struct SERIAL_ST* serial_init_with_baud(const char *dev, int baud);

/**************************************************************************************
** 函数名称: serial_read
** 功能描述: 關閉serial
** 输　入  : s          serial指針  
            buf         讀取存放的指針    
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_read(struct SERIAL_ST* s, char *buf, int len);

/**************************************************************************************
** 函数名称: serial_write
** 功能描述: 關閉serial
** 输　入  : s          serial指針  
            buf         發送存放的指針  
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_write(struct SERIAL_ST* s, char *buf, int len);

/**************************************************************************************
** 函数名称: serial_close
** 功能描述: 關閉serial
** 输　入  : s          serial指針  
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_close(struct SERIAL_ST* s);

#ifdef __cplusplus
}
#endif

#endif