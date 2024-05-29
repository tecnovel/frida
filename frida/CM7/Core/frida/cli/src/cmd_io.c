/*
 * cmd_io.c
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a & pique_n
 */

#include "frida.h"

// cmd processor functions

char* sys_get_hostname(void) {
	return "FRIDA";
}

// usb CDC

int sys_usb_getchar() {
  char c; // has to be a char because cdc_read only writes chars to it

	if (tud_cdc_available())
	{
	  tud_cdc_read(&c, 1);
	}
	else
	{
	  c = EOF;
	}

	return (int) c;
}

 //Write Character
void sys_usb_putchar(int c) {
  char ch = c;
  tud_cdc_write(&ch, 1);
  tud_cdc_write_flush();
}

// network

//Write Character
void sys_net_putchar(int c) {
    char ch = (char)c;
    char msg[2] = {ch, '\0'}; // Prepare a proper string for sending

    broadcast_message(msg);
}


int sys_net_getchar() {
    return buffer_pop_char(); // This will return EOF if the buffer is empty
}
