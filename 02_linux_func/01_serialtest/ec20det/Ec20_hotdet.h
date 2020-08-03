#ifndef EC20
#define EC20

#include <stdlib.h> 
#include <stdio.h> 
#include <assert.h>
#include <string.h>

#include <termios.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <string.h> 
#include <pthread.h>

#include <utils/Log.h>

 #define MAXBUFF 1000
 
typedef struct EC20{
	struct termios old_cfg;
	struct termios new_cfg;
	
	speed_t speed;
	int fd;

	char *read_buff;
	char *write_buff;

	char *name;
	
	int ishot;
	int isDet;
	//boolean ishot;
	
}Ec;


static speed_t getBaudrate(int baurate);
static int openTTYPath(char *path);
static void closeTTYPath(int fd);
static int setTTYconfig(Ec *ec20);
static void ReadTTY(Ec *ec20);
static int writeTTY(Ec *ec20,const char *str);
static int serial_write(Ec *s, char *buf, int len);
static int serial_read(Ec *s, char *buf, int len);




#endif

