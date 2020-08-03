#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>  
#include <unistd.h> 
#include <sys/ioctl.h> 
#include <fcntl.h>
#include <unistd.h>


#define PHY_SPEED_10_stat		0x2
#define PHY_SPEED_100_stat		0x3

#define PHY_CONFBY_HW            0     ///< Configured PHY operation mode by HW pin
#define PHY_CONFBY_SW            1     ///< Configured PHY operation mode by SW register   
#define PHY_MODE_MANUAL          0     ///< Configured PHY operation mode with user setting.
#define PHY_MODE_AUTONEGO        1     ///< Configured PHY operation mode with auto-negotiation
#define PHY_SPEED_10             0     ///< Link Speed 10
#define PHY_SPEED_100            1     ///< Link Speed 100
#define PHY_DUPLEX_HALF          0     ///< Link Half-Duplex
#define PHY_DUPLEX_FULL          1     ///< Link Full-Duplex
#define PHY_LINK_OFF             0     ///< Link Off
#define PHY_LINK_ON              1     ///< Link On
#define PHY_POWER_NORM           0     ///< PHY power normal mode
#define PHY_POWER_DOWN           1     ///< PHY power down mode 

#define W5500_VERSIONR		0X04

//#define IOW_MAGIC 'W'
#define W5500_RESET_PHY 		1 	
#define W5500_SET_PHYCONF 		8	
#define W5500_GET_PHYCONF 		3	
#define W5500_GET_PHYSTATUS 	4	
#define W5500_GET_ANYREG_VALUE	5
#define W5500_GET_VERSIONR		6


typedef unsigned char uint8_t;
typedef unsigned int u32;


typedef struct w5500_PhyConf_t
{
      uint8_t by;       ///< set by @ref PHY_CONFBY_HW or @ref PHY_CONFBY_SW
      uint8_t mode;     ///< set by @ref PHY_MODE_MANUAL or @ref PHY_MODE_AUTONEGO
      uint8_t speed;    ///< set by @ref PHY_SPEED_10 or @ref PHY_SPEED_100
      uint8_t duplex;   ///< set by @ref PHY_DUPLEX_HALF @ref PHY_DUPLEX_FULL 
      //uint8_t power;  ///< set by @ref PHY_POWER_NORM or @ref PHY_POWER_DOWN
      //uint8_t link;   ///< Valid only in CW_GET_PHYSTATUS. set by @ref PHY_LINK_ON or PHY_DUPLEX_OFF 
      uint8_t reg_value;
	  u32	reg_addr;
}w5500_PhyConf;


int w5500_get_phyconf(int fd,w5500_PhyConf * phyconf);

int w5500_set_phyconf(int fd,w5500_PhyConf * phyconf);

int w5500_get_phystatus(int fd,w5500_PhyConf * phyconf);

int w5500_get_version(int fd,w5500_PhyConf * phyconf);

int w5500_get_reg_value(int fd,w5500_PhyConf * phyconf);

void w5500_print(w5500_PhyConf phyconf,char * cmd);






