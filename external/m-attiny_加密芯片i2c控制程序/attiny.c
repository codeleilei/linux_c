/*******************************************************************************
 * 工程名称:VNet10新一代工程固件
 * 模块名称:APP/m-attiny
 * 文件名称:attiny.c
 * 创建日期:2014/12/01
 * 创建作者:系统工程嵌入式软件组 韩伟浩
 * 实现功能:对数据库操作的一些函数进行声明
 * 备注:由文件中接收函数既成irtk加密芯片有关接口函数源码,在原来基础上修改成VNet10对应的函数
*******************************************************************************/

/**********************************包含头文件************************************/
#include "attiny.h"
#define I2C_ATTINY_DEV  "/dev/i2c-2"

unsigned char returnsucceed[6] = {'H', 'D', 'C', 0x94, 0x00, 0xDB};
unsigned char returnfail[6] = {'H', 'D', 'C', 0x95, 0x00, 0xDA};

#if 0
/*******************************************************************************
 * 名称:Read_ATtiny
 * 功能:I2C读ATTINY45的EEPROM的程序
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度，注：add+len小于256，且len小于32/
 * 返回:读取成功，返回0，读取的数据保存在str里面
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Read_ATtiny(unsigned char add, unsigned char *str, unsigned char len)
{
	int fd, ret;
	unsigned char i, j, temp;

	/*1 创建数据结构并初始化 ----------------------------------------------------*/
	struct i2c_rdwr_ioctl_data readdata;
	readdata.nmsgs = 1; /*开始信号个数*/
	readdata.msgs = (struct i2c_msg *) malloc(readdata.nmsgs * sizeof(struct i2c_msg));
	if (!readdata.msgs) {
		perror("malloc readdata.msgs error\n");
		return -1;
	}

	(readdata.msgs[0]).len = 8; /*数据长度*/
	(readdata.msgs[0]).addr = 0x45; /*芯片I2C地址*/
	(readdata.msgs[0]).flags = 0; /*写标志*/
	(readdata.msgs[0]).buf = (unsigned char *) malloc((readdata.msgs[0]).len);
	if (!(readdata.msgs[0]).buf) {
		perror("malloc (readdata.msgs[0]).buf error\n");
		free(readdata.msgs);
		return -1;
	}
	/*2 确定要读取的地址和长度 --------------------------------------------------*/
	/*  数据内容＜H＞＜D＞＜C＞＜0x92＞＜L=02＞ <add> <len> <X> ------------------*/
	(readdata.msgs[0]).buf[0] = 'H';
	(readdata.msgs[0]).buf[1] = 'D';
	(readdata.msgs[0]).buf[2] = 'C';
	(readdata.msgs[0]).buf[3] = 0x92;
	(readdata.msgs[0]).buf[4] = 0x02;
	(readdata.msgs[0]).buf[5] = add;
	(readdata.msgs[0]).buf[6] = len;
	(readdata.msgs[0]).buf[7] = 'H' ^ 'D' ^ 'C' ^ 0x92 ^ 0x02 ^ add ^ len;

/*for(i=0; i<(readdata.msgs[0]).len; i++){
		printf("%x  ",(readdata.msgs[0]).buf[i]);
}*/

	//3 打开文件,写入数据
	fd = open("/dev/i2c-2", O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open /dev/i2c-2 error\n");
	}
	int err = flock(fd, LOCK_EX);          //Exclusive lock
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &readdata); /*数据发送*/
	if (ret < 0) {
		perror("I2C send command to tiny error\n");
	}

	//4 释放内存,写入结束
	free((readdata.msgs[0]).buf); /*释放内存*/
	flock(fd, LOCK_UN);                   //LOCK_UN
	close(fd);


	usleep(200000); /*发送完读取命令后，准备读取数据*/


	(readdata.msgs[0]).len = len + 6;
	(readdata.msgs[0]).addr = 0x45;
	(readdata.msgs[0]).flags = 1; /*读标志*/

	(readdata.msgs[0]).buf = (unsigned char*) malloc((readdata.msgs[0]).len);

	if (!(readdata.msgs[0]).buf) {
		perror("malloc (readdata.msgs[0]).buf error\n");
		free(readdata.msgs);
		return -1;
	}

	fd = open("/dev/i2c-2", O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open /dev/i2c-2 error\n");
	}
	flock(fd, LOCK_EX);
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &readdata);
	if (ret < 0) {
		perror("I2C read tiny data error\n");
	}
	flock(fd, LOCK_UN);
	close(fd);

	/*printf("\nread read data");
	for(i=0; i<((readdata.msgs[0]).len); i++){
			printf("%x  ",(readdata.msgs[0]).buf[i]);
	}*/
	if (('H' == (readdata.msgs[0]).buf[0]) && /*数据处理，一个个验证＜H＞＜D＞＜C＞＜0x92＞＜L=len＞<DATA><X>*/
			('D' == (readdata.msgs[0]).buf[1]) &&
			('C' == (readdata.msgs[0]).buf[2]) &&
			(0x92 == (readdata.msgs[0]).buf[3]) &&
			(len == (readdata.msgs[0]).buf[4])) {

		temp = 0x00; /*异或校验*/
		j = (readdata.msgs[0]).len - 1;
		for (i = 0; i < j; i++) {
			temp ^= (readdata.msgs[0]).buf[i];
		}


		if (temp == (readdata.msgs[0]).buf[i]) { /*数据返回*/
			for (i = 0; i < len; i++) {
				str[i] = (readdata.msgs[0]).buf[i + 5];
				//printf("str[%d] = %c ",i,str[i]);

			}
			//str[i] = '\0';				/*这里有点问题啊？？？？？？*/

			/*printf("\nread tiny=");
			for(i=0; i<(readdata.msgs[0]).len; i++){
					printf("%x  ",(readdata.msgs[0]).buf[i]);
			}
			printf("\n");*/
		} else {
			printf("the check is error\n ");
			ret = -1;
		}
	} else {
		printf("readtiny is error\n ");
		for (i = 0; i < (readdata.msgs[0]).len; i++) {
			printf("%x ", (readdata.msgs[0]).buf[i]);
		}
		printf("\n");
		ret = -1;
	}

	free((readdata.msgs[0]).buf); /*释放内存*/
	free(readdata.msgs);
	return ret;
}

/***************************************************************
 *函数名：int writetiny(	unsigned char add,	/地址，0x00～0xff/
 *			unsigned char *str,	/读取数据存放地址/
 *			unsigned char len)	/要读取数据的长度，注：add+len小于256，且len小于32/
 *函数描述：	I2C写ATTINY45的EEPROM的程序
 *返回参数：	写成功，返回0，写失败，返回-1
 ************************************************************/
int writetiny(unsigned char add, unsigned char *str, unsigned char len)
{
	int fd, ret;
	struct i2c_rdwr_ioctl_data writedata;
	unsigned char i, j, temp;

	writedata.nmsgs = 1; /*开始信号个数*/
	writedata.msgs = (struct i2c_msg*) malloc(writedata.nmsgs * sizeof (struct i2c_msg));
	if (!writedata.msgs) {
		perror("malloc writedata.msgs error\n");
		return -1;
	}

	(writedata.msgs[0]).len = len + 8; /*数据长度*/
	(writedata.msgs[0]).addr = 0x45; /*地址*/
	(writedata.msgs[0]).flags = 0; /*写*/
	(writedata.msgs[0]).buf = (unsigned char*) malloc((writedata.msgs[0]).len);
	if (!(writedata.msgs[0]).buf) {
		perror("malloc (writedata.msgs[0]).buf error\n");
		free(writedata.msgs);
		return -1;
	}
	(writedata.msgs[0]).buf[0] = 'H'; /*看门狗喂狗指令：＜H＞＜D＞＜C＞＜0x9?＞＜0x00＞＜X＞*/
	(writedata.msgs[0]).buf[1] = 'D';
	(writedata.msgs[0]).buf[2] = 'C';
	(writedata.msgs[0]).buf[3] = 0x93;
	(writedata.msgs[0]).buf[4] = len + 2;
	(writedata.msgs[0]).buf[5] = add;
	(writedata.msgs[0]).buf[6] = len;
	for (i = 0; i < len; i++) {
		(writedata.msgs[0]).buf[i + 7] = str[i];
	}
	//strcat( (writedata.msgs[0]).buf, str);

	temp = 0x00; /*异或校验*/
	j = (writedata.msgs[0]).len - 1;
	for (i = 0; i < j; i++) {
		temp ^= (writedata.msgs[0]).buf[i];

	}
	(writedata.msgs[0]).buf[ (writedata.msgs[0]).len - 1 ] = temp;

	/*printf("\nwritetiny send data = ");
	for(i=0; i<((writedata.msgs[0]).len); i++){
			printf("%x  ",(writedata.msgs[0]).buf[i]);
	}*/

	fd = open("/dev/i2c-2", O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open /dev/i2c-2 error\n");
	}
	int err = flock(fd, LOCK_EX);
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &writedata); /*数据发送*/
	if (ret < 0) {
		perror("writetiny send command  i2c_ioctl error\n");
	}
	flock(fd, LOCK_UN);
	close(fd);

	usleep(200000); /*发送完写命令后，准备读取数据*/
	(writedata.msgs[0]).len = 6; /*数据长度*/
	(writedata.msgs[0]).flags = 1; /*读操作结果，成功或失败*/


	fd = open("/dev/i2c-2", O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open /dev/i2c-2 error");
	}
	flock(fd, LOCK_EX);
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &writedata);
	if (ret < 0) {
		perror(" writetiny  return i2c_ioctl error\n");
	}
	flock(fd, LOCK_UN);
	close(fd);

	if (0 == strncmp((writedata.msgs[0]).buf, returnsucceed, 6)) {
		//printf("writetiny return succeed\n");
		ret = 0;
	} else if (0 == strncmp((writedata.msgs[0]).buf, returnfail, 6)) {
		ret = -1;
		printf("writetiny return fail\n");
	} else {
		printf("writetiny return error\n the return data =\n");
		for (i = 0; i < (writedata.msgs[0]).len; i++) {
			printf("%x ", (writedata.msgs[0]).buf[i]);
		}
		ret = -1;
	}

	free((writedata.msgs[0]).buf); /*释放内存*/
	free(writedata.msgs);
	return ret;
}

#endif

/*******************************************************************************
 * 名称:Send_ATtiny_Data
 * 功能:从ATtiny45 I2C设备写入数据
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度,0表示读取地址,其它写入数据长度
 *                        注：add+len小于256，且len小于32/
 *      char action 'R'-读数据,　'W'-写数据
 * 返回:读取成功，返回1
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Send_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len, char action)
{
	int fd, ret = 1;
	unsigned char i, j, temp;

	/*1 创建数据结构并初始化 ----------------------------------------------------*/
	struct i2c_rdwr_ioctl_data writedata;
	writedata.nmsgs = 1;                   //开始信号个数
	writedata.msgs = (struct i2c_msg *) malloc(writedata.nmsgs * sizeof(struct i2c_msg));
	if (!writedata.msgs) {
		perror("malloc writedata.msgs error\n");
		return -1;
	}

	(writedata.msgs[0]).len = len + 8; /*数据长度*/
	(writedata.msgs[0]).addr = 0x45; /*芯片I2C地址*/
	(writedata.msgs[0]).flags = 0; /*写标志*/
	(writedata.msgs[0]).buf = (unsigned char *) malloc((writedata.msgs[0]).len);
	if (!(writedata.msgs[0]).buf) {
		perror("malloc (writedata.msgs[0]).buf error\n");
		free(writedata.msgs);
		return -1;
	}
	/*2 确定要读取的地址和长度 --------------------------------------------------*/
	/*  通讯协议  <H> <D> <C> <0x92> <L=0x02> <add> <len> <X>------------------*/
	/*  读地址数据<H> <D> <C> <0x92> <L=0x02> <add> <len> <X>------------------*/
	/*  写数据内容<H> <D> <C> <0x93> <L=0x02 + len > <add> <len> <data> <X>----*/
	(writedata.msgs[0]).buf[0] = 'H';
	(writedata.msgs[0]).buf[1] = 'D';
	(writedata.msgs[0]).buf[2] = 'C';
	(writedata.msgs[0]).buf[4] = len + 0x02;
	(writedata.msgs[0]).buf[5] = add;
	(writedata.msgs[0]).buf[6] = len;

	if (action == 'R' || action == 'r') {
		(writedata.msgs[0]).len = 8; /*数据长度*/
		(writedata.msgs[0]).buf[3] = 0x92;
		(writedata.msgs[0]).buf[4] = 0x02;
		(writedata.msgs[0]).buf[7] = 'H' ^ 'D' ^ 'C' ^ 0x92 ^ 0x02 ^ add ^ len;
	} else if (action == 'W' || action == 'w') {
		(writedata.msgs[0]).buf[3] = 0x93;
		for (i = 0; i < len; i++) {
//			(writedata.msgs[0]).buf[i + 7] = str[i];
			(writedata.msgs[0]).buf[i + 7] = str[i];
		}
		temp = 0x00; /*异或校验*/
//		j = (writedata.msgs[0]).len - 1;
		j = (writedata.msgs[0]).len - 1;
		for (i = 0; i < j; i++) {
//			temp ^= (writedata.msgs[0]).buf[i];
			temp ^= (writedata.msgs[0]).buf[i];
		}
//		(writedata.msgs[0]).buf[ (writedata.msgs[0]).len - 1 ] = temp;
		(writedata.msgs[0]).buf[ (writedata.msgs[0]).len - 1 ] = temp;
	}
/*
for(i=0; i<(writedata.msgs[0]).len; i++){
		printf("%x  ",(writedata.msgs[0]).buf[i]);
}
*/
	//3 打开文件,写入数据
	fd = open(I2C_ATTINY_DEV, O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open I2C_attiny error\n");
	}
	int err = flock(fd, LOCK_EX);          //Exclusive lock
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &writedata); /*数据发送*/
	if (ret < 0) {
		perror("I2C send command to tiny error\n");
	}

	//4 释放内存,写入结束
	flock(fd, LOCK_UN);                   //LOCK_UN

	free((writedata.msgs[0]).buf); /*释放内存*/
	free(writedata.msgs);
//	free((writedata.msgs[0]).buf); /*释放内存*/
//	free(writedata.msgs);
	close(fd);
	return ret;

}

/*******************************************************************************
 * 名称:Receive_ATtiny_Data
 * 功能:从ATtiny45 I2C设备读回数据
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度,0表示只对写状态返回的结果分析,其它读回数据长度
 *                        注：add+len小于256，且len小于32/
 *      char action 'R'-读数据,　'W'-写数据
 * 返回:读取成功，返回1，读取的数据保存在str里面
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Receive_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len, char action)
{
	int fd, ret = 1;
	unsigned char i, j, temp;

	/*1 创建数据结构并初始化 ----------------------------------------------------*/
	struct i2c_rdwr_ioctl_data readdata;

	readdata.nmsgs = 1;                   //开始信号个数
	readdata.msgs = (struct i2c_msg *) malloc(readdata.nmsgs * sizeof(struct i2c_msg));
	if (!readdata.msgs) {
		perror("malloc readdata.msgs error\n");
		return -1;
	}

	(readdata.msgs[0]).len = len + 6;
	if (action == 'w' || action == 'w') {
		(readdata.msgs[0]).len = 6;
	} else if (action == 'R' || action == 'r') {
		(readdata.msgs[0]).len = len + 6;
	}
	(readdata.msgs[0]).addr = 0x45;
	(readdata.msgs[0]).flags = 1; /*读标志*/
	(readdata.msgs[0]).buf = (unsigned char*) malloc((readdata.msgs[0]).len);
	if (!(readdata.msgs[0]).buf) {
		perror("malloc (readdata.msgs[0]).buf error\n");
		free(readdata.msgs);
		return -1;
	}


	fd = open(I2C_ATTINY_DEV, O_RDWR); /*打开I2C*/
	if (fd < 0) {
		perror("open I2C_attiny error\n");
	}
	flock(fd, LOCK_EX);
	ioctl(fd, I2C_TIMEOUT, 1); /*超时时间*/
	ioctl(fd, I2C_RETRIES, 1); /*重复次数*/

	ret = ioctl(fd, I2C_RDWR, (unsigned long) &readdata);
	if (ret < 0) {
		perror("I2C read tiny data error\n");
	}
	flock(fd, LOCK_UN);
	close(fd);

	/*printf("\nread read data");
	for(i=0; i<((readdata.msgs[0]).len); i++){
			printf("%x  ",(readdata.msgs[0]).buf[i]);
	}*/
	if (action == 'w' || action == 'w') {
//		if (0 == strncmp((writedata.msgs[0]).buf, returnsucceed, 6)) {
		if (0 == strncmp((readdata.msgs[0]).buf, returnsucceed, 6)) {
			printf("writetiny return succeed\n");
			ret = 0;
//		} else if (0 == strncmp((writedata.msgs[0]).buf, returnfail, 6)) {
		} else if (0 == strncmp((readdata.msgs[0]).buf, returnfail, 6)) {
			ret = -1;
			printf("writetiny return fail\n");
		} else {
			printf("writetiny return error\n the return data =\n");
//			for (i = 0; i < (writedata.msgs[0]).len; i++) {
			for (i = 0; i < (readdata.msgs[0]).len; i++) {
//				printf("%x ", (writedata.msgs[0]).buf[i]);
				printf("%x ", (readdata.msgs[0]).buf[i]);
			}
			ret = -1;
		}
	} else if (action == 'R' || action == 'r') {
		if (('H' == (readdata.msgs[0]).buf[0]) && /*数据处理，一个个验证＜H＞＜D＞＜C＞＜0x92＞＜L=len＞<DATA><X>*/
				('D' == (readdata.msgs[0]).buf[1]) &&
				('C' == (readdata.msgs[0]).buf[2]) &&
				(0x92 == (readdata.msgs[0]).buf[3]) &&
				(len == (readdata.msgs[0]).buf[4])) {

			temp = 0x00; /*异或校验*/
			j = (readdata.msgs[0]).len - 1;
			for (i = 0; i < j; i++) {
				temp ^= (readdata.msgs[0]).buf[i];
			}


			if (temp == (readdata.msgs[0]).buf[i]) { /*数据返回*/
				for (i = 0; i < len; i++) {
					str[i] = (readdata.msgs[0]).buf[i + 5];
					//printf("str[%d] = %c ",i,str[i]);

				}
				//str[i] = '\0';				/*这里有点问题啊？？？？？？*/

				/*printf("\nread tiny=");
				for(i=0; i<(readdata.msgs[0]).len; i++){
						printf("%x  ",(readdata.msgs[0]).buf[i]);
				}
				printf("\n");*/
			} else {
				printf("the check is error\n ");
				ret = -1;
			}
		} else {
			printf("readtiny is error\n ");
			for (i = 0; i < (readdata.msgs[0]).len; i++) {
				printf("%x ", (readdata.msgs[0]).buf[i]);
			}
			printf("\n");
			ret = -1;
		}
	}

	free((readdata.msgs[0]).buf); /*释放内存*/
	free(readdata.msgs);
	return ret;
}

/*******************************************************************************
 * 名称:Read_ATtiny_Data
 * 功能:
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度,0表示只对写状态返回的结果分析,其它读回数据长度
 *                        注：add+len小于256，且len小于32/
 * 返回:读取成功，返回1，读取的数据保存在str里面
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Read_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len)
{
	if (Send_ATtiny_Data(add, str, len, 'R') < 0) {
		printf("%s:Send ATtiny Data error!\r\n", __func__);
		return -1;
	}
	usleep(200000);
	if (Receive_ATtiny_Data(add, str, len, 'R') < 0) {
		printf("%s:Recieve ATtiny Data error!\r\n", __func__);
		return -1;
	}
	return 1;
}

/*******************************************************************************
 * 名称:Write_ATtiny_Data
 * 功能:
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度,0表示只对写状态返回的结果分析,其它读回数据长度
 *                        注：add+len小于256，且len小于32/
 * 返回:读取成功，返回0
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Write_ATtiny_Data(unsigned char add, unsigned char *str, unsigned char len)
{
//	printf("%s:write buff %s\n", __func__, str);
	if (Send_ATtiny_Data(add, str, len, 'W') < 0) {
		printf("in file %s:func %s\r\n", __FILE__, __func__);
		printf("Read ATtiny Data error!\r\n");
		return -1;
	}
	usleep(200000);
	return Receive_ATtiny_Data(add, str, len, 'W');
}


/*******************************************************************************
 * 名称:Read_VNet_AT
 * 功能:
 * 形参: unsigned char add /地址，0x00～0xff/
 *      unsigned char *str /读取数据存放地址/
 *      unsigned char len /要读取数据的长度,0表示只对写状态返回的结果分析,其它读回数据长度
 *                        注：add+len小于256，且len小于32/
 * 返回:读取成功，返回0
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Read_VNet_AT(ATtinyEnum_t type, unsigned char *data)
{
	int res;
	char *buf_p;
	buf_p = data;

	unsigned char addr;
	unsigned char len = 16;
	switch (type) {
		case production_date: addr = 0; break;
		case machine_type: addr = 16; break;
		case machine_sn: addr = 32; break;
		case registration_code: addr = 48; len = 30; break;
		case batt_type: addr = 78; len = 2; break;
		default: {
			printf("Write to FRAM: NOT SUCH INFO!!\n");
			return -1;
			break;
		}
	}
	return Read_ATtiny_Data(addr, buf_p, len);
}

/*******************************************************************************
 * 名称:Write_VNet_AT
 * 功能:
 * 形参:
 * 返回:读取成功，返回0，读取的数据保存在str里面
 *     (读取失败，返回-1:这里做得还不够完善，还有一点难题没有解决）
 * 说明:无
*******************************************************************************/
int Write_VNet_AT(ATtinyEnum_t type, unsigned char *data)
{
	int res;
	unsigned char addr;
	unsigned char len = 16;
	switch (type) {
		case production_date: addr = 0; break;
		case machine_type: addr = 16; break;
		case machine_sn: addr = 32; break;
		case registration_code: addr = 48; len = 30; break;
		case batt_type: addr = 78; len = 2; break;
		default: {
			printf("Write to FRAM: NOT SUCH INFO!!\n");
			return -1;
			break;
		}
	}
	return Write_ATtiny_Data(addr, data, len);
}

/**********************************文件结束**************************************/

