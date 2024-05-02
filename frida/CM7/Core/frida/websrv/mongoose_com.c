/*
 * mongoose_com.c
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#include "frida.h"

// callback when data is coming from usb
bool tud_network_recv_cb(const uint8_t *buf, uint16_t len) {
  mg_tcpip_qwrite((void *) buf, len, s_ifp);
  // MG_INFO(("RECV %hu", len));
  // mg_hexdump(buf, len);
  tud_network_recv_renew();
  return true;
}

/* this is used by this code, ./class/net/net_driver.c, and usb_descriptors.c */
/* ideally speaking, this should be generated from the hardware's unique ID (if available) */
/* it is suggested that the first byte is 0x02 to indicate a link-local address */
uint8_t tud_network_mac_address[6];

// init the mac address of tiny usb
void init_tud_network_mac_address(void) {
    uint8_t temp_mac_address[6] = MAC_ADDRESS;
    for (int i = 0; i < 6; i++) {
        tud_network_mac_address[i] = temp_mac_address[i];
    }
}

// callback when network is init
void tud_network_init_cb(void) {
}


// sends via usb network data (tiny USB)
uint16_t tud_network_xmit_cb(uint8_t *dst, void *ref, uint16_t arg) {
  // MG_INFO(("SEND %hu", arg));
  memcpy(dst, ref, arg);
  return arg;
}

// logging function
void log_fn(char ch, void *param) {
	tud_cdc_write_char(ch);
	tud_cdc_write_flush();
}

// send via usb network (mongoose)
size_t usb_tx(const void *buf, size_t len, struct mg_tcpip_if *ifp) {
  if (!tud_ready()) return 0;
  while (!tud_network_can_xmit(len)) tud_task();
  tud_network_xmit((void *) buf, len);
  (void) ifp;
  return len;
}

// mongoose aks if usb is working
bool usb_up(struct mg_tcpip_if *ifp) {
  (void) ifp;
  return tud_inited() && tud_ready() && tud_connected();
}

uint64_t mg_millis(void) {	// Let Mongoose use our uptime function
	return HAL_GetTick();	// Return number of milliseconds since boot
}

// Hardware random generator
void mg_random(void *buf, size_t len) {  // Use on-board RNG
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}
