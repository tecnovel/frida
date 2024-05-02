/*
 * frida.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_FRIDA_H_
#define FRIDA_FRIDA_H_


#include "main.h"


// usb
#include "msc_disk.h"

// websrv
#include "mongoose.h"
#include "mongoose_com.h"
#include "dhserver.h"
#include "dnserver.h"
#include "ip_settings.h"
#include "eventHandler.h"



extern struct mg_mgr mgr;
extern struct mg_tcpip_if* s_ifp;

bool dns_query_proc(const char *name, uint32_t *addr);

void frida_init();
void frida_loop();

// optional
//void frida_srvTask();
//void frida_usbTask();

#endif /* FRIDA_FRIDA_H_ */
