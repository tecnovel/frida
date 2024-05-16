/*
 * cmd_io.h
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a
 */

#ifndef FRIDA_CLI_CMD_IO_H_
#define FRIDA_CLI_CMD_IO_H_

char* system_get_hostname(void);
int system_getchar();
void system_putchar(int c);

#endif /* FRIDA_CLI_CMD_IO_H_ */
