#ifndef __DIO_API_H
#define __DIO_API_H

#ifndef __DIO_API_H_INC__
    #define __DIO_API_H_EXT__ extern
#else
    #define __DIO_API_H_EXT__
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DEV				                            "/dev/qiyang_dio"

#define	IOCTL_DIO_SET_IRQ                           _IOW('W', 0x04, int)	

#define DIO_PIN_MASK					            0x00FF

#define DIO_TRIGGER_MASK	                        0xFF00
#define DIO_TRIGGER_NONE	                        0x0100
#define DIO_TRIGGER_RISING	                        0x0200
#define DIO_TRIGGER_FALLING	                        0x0400
#define DIO_TRIGGER_BOTH	                        (DIO_TRIGGER_RISING | DIO_TRIGGER_FALLING)
#define DIO_CMD(pin, irqType)                       (pin + irqType)

struct IOTYPE{
	int pin;
	int state;
    int dio_trigger;
};

typedef void(*FUNC_IO)(struct IOTYPE io);

struct IOFUNC{
	int pin;
    int dio_trigger;
    FUNC_IO func;
};

struct DIORUN{
    int fd;
    pthread_t io_thread;
    struct IOFUNC ioFunc[3];
};


/**************************************************************************************
** 函数名称: io_init
** 功能描述: 初始化IO
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int io_init();

/**************************************************************************************
** 函数名称: io_setup
** 功能描述: 設置DIO中斷類型
** 输　入  : pinNum      pin号                      
**          irqType     設置觸發方式                   
**                          DIO_TRIGGER_NONE        0x00000000
**                          DIO_TRIGGER_RISING      0x00000001      //上升沿触发中断 
**                          DIO_TRIGGER_FALLING     0x00000002      //下升沿触发中断
**                          DIO_TRIGGER_BOTH        0x00000008      //邊沿触发中断
**          func        回調函數
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int io_setup(int *pinNum, int irqType, FUNC_IO callFunc);

/**************************************************************************************
** 函数名称: io_start
** 功能描述: 啓動觸發
** 输　入  :                       
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int io_start();

/**************************************************************************************
** 函数名称: io_exit
** 功能描述: 關閉IO
** 输　出  : 0, 错误返回 -1
**************************************************************************************/
int io_exit();

#ifdef __cplusplus
}
#endif

#endif