#include "Ec20_hotdet.h"


static speed_t getBaudrate(int baurate){
	//default baurate as 115200
	return B115200;
}


static int openTTYPath(const char *path){
	int fd;
	fd=open(path,O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
	if(fd <0 ){
		perror("open ttypath failed!");
		return -1;
	}
	return fd;
}


static void closeTTYPath(int fd){
	close(fd);
}



static int setTTYconfig(Ec *ec20){
	
	/*1、get baurate*/
	ec20->speed=getBaudrate(115200);
	
	/*2、get old cfg*/
	if(tcgetattr(ec20->fd,&ec20->old_cfg)!=0){
		perror("tcgetattr");
		return -1;
	}

	/*3、init new_cfg*/
	cfmakeraw(&ec20->new_cfg);

	/*4、set baurate*/
	cfsetispeed(&ec20->new_cfg,ec20->speed);
	cfsetospeed(&ec20->new_cfg,ec20->speed);

	/*5、enable new_cfg*/
	if (tcsetattr(ec20->fd, TCSANOW, &ec20->new_cfg)) { 
		perror("tcsetattr");
		return -1;
	}
	return 0;
}


static int serial_read(Ec *s, char *buf, int len)
{
    int fd;
    int ret;

    if (s == NULL || s->fd < 0) return -1;
    fd = s->fd;

    ret = read(fd, buf, len); 
    if (ret <= 0){ 
		//perror("read");
		//printf("read fail ! ret: %d /n",ret);
		//return -1;
    }
    return ret;
}


static int serial_write(Ec *s, char *buf, int len)
{
    int fd;
    int ret;

    if (s == NULL || s->fd < 0) return -1;
    fd = s->fd;

    ret = write(fd, buf, len);
    if (ret <= 0) return -1;

    return ret;
}

/*
*@brife: jugg if str contain string:QSIMDET
*/
int IsDet(char *str){
	char *key="QSIMDET"; 
	if(strstr(str,key))
		return 1;
	return 0;

}

int IshHotEable(char *str){

	if((*(str+12)=='1')&&(*(str+14)=='1'))
		return 1;
	else 
		return 0;
	/*
	int lenght;
	lenght =strlen(str);
	for(int i=0;i<lenght-1;i++)
		printf(" %d, str is :%c",i,*(str+i));
	*/


}


void *readFormTTY(void *arg){
	Ec *ec;
	int ret;
	ec=(Ec*)(arg);
	ec->read_buff=(char *)malloc(MAXBUFF);
	printf("ec->name: %s\n",ec->name);
	while(1){
		memset(ec->read_buff,0,MAXBUFF);
		
    	ret = serial_read(ec, ec->read_buff, MAXBUFF); 
	
		if (ret <= 0) {
			//perror("serial_read");
		}
		else if(ret!=0){
			printf("receive %d datas.\n",ret) ;
			*(ec->read_buff+ret+1)='\0';
			printf("data is :%s \n", ec->read_buff);
			
			if(IsDet(ec->read_buff)){
				ec->isDet=1;
				
				if(IshHotEable(ec->read_buff)){
					ec->ishot=1;
				}else{
					printf("cl--not support hot!\n");
					
					writeTTY(ec,"AT+QSIMDET=1,1 ");
					sleep(1);
					writeTTY(ec,"AT+QSIMSTAT=1  ");
					sleep(1);
					writeTTY(ec,"AT+CFUN=1,1 ");
				}
				break;
			}
			
			writeTTY(ec,"AT+QSIMDET? ");
			//sleep(1);
		}

	}
	return 0;
}




static void ReadTTY(Ec *ec20){
	pthread_t readpid;
	int read;
	
	read = pthread_create(&readpid,NULL,readFormTTY,ec20);
	pthread_join(readpid,NULL);
	
}




static int writeTTY(Ec *ec20,const char *str){
	int buff_size,str_size;
	int ret;

	str_size = strlen(str);
	buff_size = str_size+2;
	
	ec20->write_buff=(char *)malloc(buff_size);
	if(!ec20->write_buff){
		return -1;
	}
	
	memset(ec20->write_buff,0,buff_size);

	strcpy(ec20->write_buff,str);
	*(ec20->write_buff+str_size-1+1) = '\r';
	*(ec20->write_buff+str_size-1+2) = '\n';

	printf("cmd %s str_size: %d buff_size:%d \n",ec20->write_buff ,str_size,buff_size);
	ret=serial_write(ec20,ec20->write_buff,buff_size);
	if(ret < 0){
		perror("serial_write");
		free(ec20->write_buff);
		return -1;
	}

	free(ec20->write_buff);
	return 0;
}


int main(){
	const char *tty_path="/dev/ttyUSB2";
	int ret;
	Ec *ec20; 
	
	ec20= (Ec *)malloc(sizeof(Ec));
	ec20->name="chenl";
	
	while((access(tty_path,F_OK|R_OK|W_OK)));   

	/*1、tty config*/
	ec20->fd = openTTYPath(tty_path);
	
	ret = setTTYconfig(ec20);
	if(ret < 0 ){
		printf("set ttycfg failed!\n");
		closeTTYPath(ec20->fd);
		free(ec20);
	}
	printf("config success\n");
	
	/*2、send cmd to ec20*/
	writeTTY(ec20,"AT+QSIMDET?");
	ReadTTY(ec20);
	while(!ec20->isDet);
}


