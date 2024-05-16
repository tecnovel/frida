/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, theidel
 *  Created :  02.05.2014 13:24:35
 *
 *  Description :  Processing the commands entered via the terminal command line
 *                 using the modular command implementation.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "cmd_proc_interface.h"



/******************************************************************************/
/* static vars                                                                */
/******************************************************************************/
static cmd_proc_interface_func_putchar fct_putchar;
static cmd_proc_interface_func_getchar fct_getchar;
static cmd_proc_interface_func_get_hostname fct_get_hostname;
static cmd_proc_interface_func_usleep fct_usleep;

/******************************************************************************/
/* function definitions                                                       */
/******************************************************************************/

void cmd_proc_interface_init(   cmd_proc_interface_func_putchar putchar_fct,
                                cmd_proc_interface_func_getchar getchar_fct,
                                cmd_proc_interface_func_get_hostname hostname_fct,
                                cmd_proc_interface_func_usleep usleep_fct)
{
  fct_putchar = putchar_fct;
  fct_getchar = getchar_fct;
  fct_get_hostname = hostname_fct;
  fct_usleep = usleep_fct;
}

void cmd_proc_interface_putchar(int c)
{
  fct_putchar(c);
}

int cmd_proc_interface_getchar(void)
{
  return fct_getchar();
}

char* cmd_proc_interface_get_hostname(void)
{
    return fct_get_hostname();
}

void cmd_proc_interface_usleep(unsigned long time)
{
    fct_usleep(time);
}

/******************************************************************************/
