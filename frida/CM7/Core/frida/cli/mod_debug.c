/*
 * mod_debug.c
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a
 */

#include "frida.h"

char debug = 0;

void cli_log_mg(char ch, void *param){
	if(debug){
		xfs_printf("%c", ch);
	}
}



/******************************************************************************/

int mod_debug_help(int argc, char **argv)
{
  CMD_HELP("","debug related commands");

  return 0;
}


/******************************************************************************/

int cmd_log(int argc, char **argv)
{
  CMD_HELP("", "displays logging messages");

  debug = !debug;
  return 0;
}


/******************************************************************************/

cmd_table_entry_type cmd_table_mod_debug[] =
{
  {0, "mod_debug",      mod_debug_help},  /* first entry is module name and help */
  {0, "log",       		cmd_log},
  {0, 0,         		0}
};
