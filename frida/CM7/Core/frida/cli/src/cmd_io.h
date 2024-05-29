/*
 * cmd_io.h
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_CLI_CMD_IO_H_
#define FRIDA_CLI_CMD_IO_H_

char* sys_get_hostname(void);

int sys_usb_getchar();
void sys_usb_putchar(int c);

void sys_net_putchar(int c);
int sys_net_getchar();

#endif /* FRIDA_CLI_CMD_IO_H_ */
