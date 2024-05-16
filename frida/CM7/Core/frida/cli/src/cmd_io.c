/*
 * cmd_io.c
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a
 */

#include "frida.h"

char* system_get_hostname(void) {
 	return "FRIDA";
 }

 int system_getchar() {
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
 void system_putchar(int c) {
	  char ch = c;
	  tud_cdc_write(&ch, 1);
	  tud_cdc_write_flush();
 }
