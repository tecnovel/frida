/*
 * frida.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_FRIDA_H_
#define FRIDA_FRIDA_H_


#include "main.h"

//fatfs
#include "fatfs/fatfs.h"
#include "fatfs/sd_diskio.h"


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



extern struct mg_mgr mgr;
extern struct mg_tcpip_if* s_ifp;


void frida_init();
void frida_loop();

// optional
//void frida_srvTask();
//void frida_usbTask();

#endif /* FRIDA_FRIDA_H_ */
