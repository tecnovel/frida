/*
 * mongoose_com.h
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_WEBSRV_MONGOOSE_COM_H_
#define FRIDA_WEBSRV_MONGOOSE_COM_H_


#include "mongoose.h"

bool tud_network_recv_cb(const uint8_t *buf, uint16_t len);
void tud_network_init_cb(void);
void init_tud_network_mac_address(void);
uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg);

void log_fn(char ch, void *param);
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp);
bool usb_up(struct mg_tcpip_if *ifp);

uint64_t mg_millis(void);
void mg_random(void *buf, size_t len);



#endif /* FRIDA_WEBSRV_MONGOOSE_COM_H_ */
