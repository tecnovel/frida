/*
 * frida.c
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */

#include "frida.h"

/**
  * @brief Mongoose event manager structure.
  */
struct mg_mgr mgr;

/**
  * @brief Pointer to the TCP/IP network interface.
  */
struct mg_tcpip_if* s_ifp;

/**
  * @brief DHCP entries for IP address allocation.
  */
dhcp_entry_t entries[] =
{
    /* mac ip address                          lease time */
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 100), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 101), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 102), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 103), 24 * 60 * 60 },
};

/**
  * @brief DHCP configuration structure.
  */
const dhcp_config_t dhcp_config =
{
    .router    = IP_ADDRESS,              		 /* router address (if any) */
    .port      = 67,                       		 /* listen port */
    .dns       = IP_ADDRESS,               		 /* dns server (if any) */
    .domain    = "psi",               			 /* dns suffix */
    .num_entry = sizeof(entries) / sizeof(dhcp_entry_t),   /* num entry */
    .entries   = entries                   		 /* entries */
};

/**
  * @brief Callback function for DNS query processing.
  * @param name The DNS name to resolve.
  * @param addr Pointer to store the resolved IP address.
  * @note This function resolves specific DNS names and returns their corresponding IP address.
  * @retval true if the name is resolved successfully, false otherwise.
  */
bool dns_query_proc(const char *name, uint32_t *addr) {
    if (strcmp(name, "frida") == 0 || strcmp(name, "frida.local") == 0 || strcmp(name, "frida.psi") == 0) {
        *addr = IP_ADDRESS;
        return true;
    }
    return false;
}

/**
  * @brief Initializes the Frida device.
  * @note This function initializes the FATFS filesystem, TCP/IP stack, USB, and other necessary components.
  * @retval None
  */
void frida_init() {
    MX_FATFS_Init();
    mg_mgr_init(&mgr);        // Initialise Mongoose event manager
    mg_log_set(MG_LL_DEBUG);  // Set log level
    mg_log_set_fn(log_fn, NULL);

    MG_INFO(("Init TCP/IP stack ..."));
    static struct mg_tcpip_driver driver = {.tx = usb_tx, .up = usb_up};
    static struct mg_tcpip_if mif = {.mac = MAC_ADDRESS,
                                     .ip = IP_ADDRESS,
                                     .mask = IP_NETMASK,
                                     .gw = IP_GATEWAY,
                                     .driver = &driver,
                                     .recv_queue.size = 4096};

    s_ifp = &mif;

    mg_tcpip_init(&mgr, &mif);

    dhserv_init(&mgr, &dhcp_config);

    dnserv_init(&mgr, dns_query_proc);

    mg_http_listen(&mgr, "http://0.0.0.0:80", fn, &mgr);

    MG_INFO(("Init USB ..."));
    init_tud_network_mac_address();

    fatfs_init();
    tud_init(BOARD_TUD_RHPORT);

    MG_INFO(("Init done, starting main loop ..."));
}

/**
  * @brief Task for handling server operations.
  * @note This function polls the Mongoose event manager for server operations.
  * @retval None
  */
void frida_srvTask() {
    mg_mgr_poll(&mgr, 1000);
}

/**
  * @brief Task for handling USB operations.
  * @note This function handles USB tasks.
  * @retval None
  */
void frida_usbTask() {
    tud_task();
}

/**
  * @brief Main loop for Frida device operations.
  * @note This function runs the main loop for the Frida device, including server and USB tasks.
  * @retval None
  */
void frida_loop() {
    frida_srvTask();
    frida_usbTask();
}
