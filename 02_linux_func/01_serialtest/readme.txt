这是一个 linux 串口编程的例程

目的： 检测EC20 4G模块是否开启 热插拔功能  
			如果没有开启  则发送命令 开启 热插拔功能
		
主要学习点：
	1、关于串口的函数主要是 setTTYconfig 函数
			1）这里主要做了设置波特率的操作 
			
	2、
		1）ec20 开启热插拔的命令：
			AT+QSIMDET=1,1
			
		2）查看是否开启热插拔：
			AT+QSIMDET?
			
			
注意点：
	ec20det是一个android的扩展模块  这里使用android.mk 将其编译成可执行文件
	lib是一个参考例程 他包含了一些基本功能的api 