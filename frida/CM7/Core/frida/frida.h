/*
 * frida.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_FRIDA_H_
#define FRIDA_FRIDA_H_


#include "main.h"

/* select memory driver (only one driver!!!) */
//#define USE_SD
#define USE_FLASH

//fatfs
#include "fatfs/fatfs.h"
#ifdef USE_SD
	#include "fatfs/drv/SD/sd_diskio.h"
#endif

#ifdef USE_FLASH
	#include "fatfs/drv/flash/flash_diskio.h"
#endif


// usb
#include "usb/TinyUSB/tusb.h"
#include "usb/msc_disk.h"


// websrv
#include "websrv/mongoose/mongoose.h"
#include "websrv/mongoose_com.h"
#include "websrv/dhcp-server/dhserver.h"
#include "websrv/dns-server/dnserver.h"
#include "websrv/ip_settings.h"
#include "eventHandler.h"



#include "fatfs/fatfs.h"
#ifdef USE_SD
	#define MEMORY_DRV		SD_Driver
#endif

#ifdef USE_FLASH
	#define MEMORY_DRV		FLASH_Driver
#endif



extern struct mg_mgr mgr;
extern struct mg_tcpip_if* s_ifp;
extern SD_HandleTypeDef hsd1;


void frida_init();
void frida_loop();

// optional
//void frida_srvTask();
//void frida_usbTask();

#endif /* FRIDA_FRIDA_H_ */
