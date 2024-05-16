/*
 * mod_hello.c
 *
 *  Created on: 10.11.2021
 *      Author: koprek_w
 */


#include "src/cmd_processor.h"
#include "src/xfs_printf.h"

#include <string.h>


int mod_hello_help(int argc, char **argv)
{
  CMD_HELP("","example module ...");

  xfs_printf("\r\nSpecific help for Module A...\r\n");

  return 0;
}


/******************************************************************************/

int cmd_echo(int argc, char **argv)
{
  CMD_HELP("<text>", "display a line of text");

  if (argc < 2) return -1;

  xfs_printf("\n%s\r\n", argv[1]);
  return 0;
}


/******************************************************************************/

cmd_table_entry_type cmd_table_mod_hello[] =
{
  {0, "mod_hello",      mod_hello_help},  /* first entry is module name and help */
  {2, "echo",       	cmd_echo},
  {0, 0,         		0}
};

