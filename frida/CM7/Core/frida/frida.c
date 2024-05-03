/*
 * frida.c
 *
 *  Created on: May 2, 2024
 *      Author: kaestli_a
 */


#include "frida.h"


struct mg_mgr mgr;
struct mg_tcpip_if* s_ifp;



dhcp_entry_t entries[] =
{
    /* mac ip address                          lease time */
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 100), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 101), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 102), 24 * 60 * 60 },
    { {0}, IPADDR4_INIT_BYTES(10, 0, 0 , 103), 24 * 60 * 60 },
};

const dhcp_config_t dhcp_config =
{
    .router    = IP_ADDRESS,              		 			/* router address (if any) */
    .port      = 67,                       					/* listen port */
    .dns       = IP_ADDRESS,               					/* dns server (if any) */
    .domain    = "psi",               						/* dns suffix */
    .num_entry = sizeof(entries) / sizeof(dhcp_entry_t),   	/* num entry */
    .entries   = entries                   					/* entries */
};

bool dns_query_proc(const char *name, uint32_t *addr)
{
	//MG_DEBUG(("dns_query_proc: >>>%s<<<\n", name));
    if (strcmp(name, "frida") == 0 || strcmp(name, "frida.local") == 0 || strcmp(name, "frida.psi") == 0)
    {
        *addr = IP_ADDRESS;
        //MG_DEBUG(("dns_query_proc: >>>%s<<<\n", name));
        //MG_DEBUG(("dns_query_proc: IP_ADDRESS %08lx\n", IP_ADDRESS));

        return true;
    }
    return false;
}



void frida_init(){

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
		//                           .enable_dhcp_server = true,
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


void frida_srvTask(){
	mg_mgr_poll(&mgr, 1000);
}
void frida_usbTask(){
	tud_task();
}

void frida_loop(){
	frida_srvTask();
	frida_usbTask();
}


