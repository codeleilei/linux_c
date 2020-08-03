#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <linux/serial.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>

#include "uart_api.h"

static int serial_set_baud_nonstandard(int fd, unsigned int baud);
static int serial_set_baud(int fd, unsigned int baudrate);
static int serial_set_attr(struct SERIAL_ST *serial_st);
static int serial_set(struct SERIAL_ST *serial_st);

extern int errno;

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
struct SERIAL_ST* serial_init(const char *dev, int baud, int databits, int stopbits, char parity)
{
    int fd;
    int ret;
    struct SERIAL_ST *serial_st;

    if (dev == NULL || dev[0] == 0) return NULL;
    if (baud < 300 || baud > 115200) return NULL;
    
    serial_st = malloc(sizeof(struct SERIAL_ST));
    if (serial_st == NULL) goto err;

    memset(serial_st, 0, sizeof(struct SERIAL_ST));

    fd = open(dev, O_RDWR);
    if (fd < 0) goto err;

    serial_st->fd = fd;
    serial_st->baud = baud;
    serial_st->databits = databits;
    serial_st->stopbits = stopbits;
    serial_st->parity = parity;

    ret = serial_set(serial_st);
    if (ret == -1) 
    {
        printf("serial init failure %s/r/n",strerror(errno));
        goto clserr;
    }

    return serial_st;

clserr:
    close(fd);
err:
    free(serial_st);
    return NULL;
}

/**************************************************************************************
** 函数名称: serial_init_with_baud
** 功能描述: 初始化serial並設置波特率
** 输　入  :dev         串口設備    eg：/dev/ttymxc3                  
**         baud        波特率      eg: 115200             
** 输　出  : SERIAL_ST指針, 错误返回 NULL
**************************************************************************************/
struct SERIAL_ST* serial_init_with_baud(const char *dev, int baud)
{
    return serial_init(dev, baud, 8, 1, 'n');
}

/**************************************************************************************
** 函数名称: serial_read
** 功能描述: 關閉serial
** 输　入  : s          serial指針  
            buf         讀取存放的指針    
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_read(struct SERIAL_ST* s, char *buf, int len)
{
    int fd;
    int ret;

    if (s == NULL || s->fd < 0) return -1;
    fd = s->fd;

    ret = read(fd, buf, len); 
    if (ret <= 0) return -1;
    
    return ret;
}

/**************************************************************************************
** 函数名称: serial_write
** 功能描述: 關閉serial
** 输　入  : s          serial指針  
            buf         發送存放的指針  
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_write(struct SERIAL_ST* s, char *buf, int len)
{
    int fd;
    int ret;

    if (s == NULL || s->fd < 0) return -1;
    fd = s->fd;

    ret = write(fd, buf, len);
    if (ret <= 0) return -1;

    return ret;
}

/**************************************************************************************
** 函数名称: serial_close
** 功能描述: 關閉serial
** 输　入  : s      serial指針  
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int serial_close(struct SERIAL_ST* s)
{
    int fd;

    if (s == NULL || s->fd < 0) return -1;
    fd = s->fd;
    free(s);
    return close(fd);
}

static int serial_set_baud_nonstandard(int fd, unsigned int baud)
{
    int ret;
    // struct termios option;
    struct serial_struct ss;

    ret = ioctl(fd,TIOCGSERIAL,&ss);
    if (ret != 0)
    {
        printf("BAUD: error to get the serial_struct info:%s/n",strerror(errno));
        return -1;
    }

    ss.flags = ASYNC_SPD_CUST;
    ss.custom_divisor = ss.baud_base / baud;
    
    ret = ioctl(fd,TIOCSSERIAL,&ss);
    if(ret != 0){
        printf("BAUD: error to set serial_struct:%s/n",strerror(errno));
        return -1;
    }
    return serial_set_baud(fd, B38400);
}

static int serial_set_baud(int fd, unsigned int baudrate)
{
    int ret; 
    struct termios option;

    /* get old serial attribute */
    memset(&option, 0, sizeof(option));

    ret =  tcflush(fd, TCIOFLUSH);
    if (ret != 0) return -1;
    
    ret = tcgetattr(fd, &option);
    if (ret != 0) return -1;

    ret = cfsetispeed(&option, baudrate); 
    if (ret != 0) return -1;

    ret = cfsetospeed(&option, baudrate); 
    if (ret != 0) return -1;

    ret = tcsetattr(fd, TCSANOW, &option);
    if (ret != 0) return -1;

    ret =  tcflush(fd, TCIOFLUSH);
    if (ret != 0) return -1;
    
    return ret;
}

static int serial_set_attr(struct SERIAL_ST *serial_st)
{ 
    int fd, ret;
	struct termios option;	

    fd = serial_st->fd;
    memset(&option, 0, sizeof(option));
    
    ret = tcgetattr(fd, &option);
    if (ret != 0) return -1;

    option.c_iflag = CLOCAL | CREAD;
    
    /* set datas size */
    option.c_cflag &= ~CSIZE; 
    option.c_iflag = 0;

    switch (serial_st->databits)
    {   
        case 7:		
            option.c_cflag |= CS7; 
            break;
            
        case 8:     
            option.c_cflag |= CS8;
            break;  
            
        default:    
            perror("invalid argument, unsupport datas size.\n");
            return -1;  
    }

    /* set parity */    
    switch (serial_st->parity) 
    {   
        case 'n':
        case 'N':    
            option.c_cflag &= ~PARENB;   
            option.c_iflag &= ~INPCK;      
            break;  
            
        case 'o':   
        case 'O':    
            option.c_cflag |= (PARODD | PARENB);  
            option.c_iflag |= INPCK;            
            break;  
            
        case 'e':  
        case 'E':   
            option.c_cflag |= PARENB;       
            option.c_cflag &= ~PARODD;   
            option.c_iflag |= INPCK;     
            break;
            
        case 's': 
        case 'S':  
            option.c_cflag &= ~PARENB;
            option.c_cflag &= ~CSTOPB;
            break;  
            
        default:   
            perror("invalid argument, unsupport parity type.\n");
            return -1;  
    }  
    
    /* set stop bits  */
    switch (serial_st->stopbits)
    {   
        case 1:    
            option.c_cflag &= ~CSTOPB;  
            break;  
            
        case 2:    
            option.c_cflag |= CSTOPB;  
            break;
            
        default:    
            perror("invalid argument, unsupport stop bits.\n");
            return -1; 
    } 
    
    option.c_oflag = 0;
    option.c_lflag = 0;  
    option.c_cc[VTIME] = TIMEOUT;    
    option.c_cc[VMIN] = MIN_LEN; 

    ret =  tcflush(fd, TCIFLUSH);
    if (ret != 0) return -1;

    ret = tcsetattr(fd, TCSANOW, &option);
    if (ret != 0) return -1;

    return ret;  
}

static int serial_set(struct SERIAL_ST *serial_st)
{
    int i, ret;
    int baudrate = 0;

    for (i = 0; i < ARRAY_SIZE(g_attr_baud); i++) 
	{ 
        if (serial_st->baud == g_attr_baud[i].lable) 
        { 
            baudrate = g_attr_baud[i].baudrate;
            break;
        }
    }

    if (baudrate == 0) {
        serial_set_baud_nonstandard(serial_st->fd, serial_st->baud);
    } else {
        serial_set_baud(serial_st->fd, baudrate);
    }
    ret = serial_set_attr(serial_st);
    if (ret != 0) return -1;

    return ret;
}