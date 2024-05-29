/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, theidel
 *  Created :  31.07.2014 08:14:22
 *
 *  Description :  Debug level output control.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_dbg.h"
#include "xfs_utils.h"
#include "xfs_printf.h"
#include "cmd_processor.h"

/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/
#ifndef DBG_NONE

unsigned int xfs_dbg_level = DBG_LEVEL_DEFAULT;
unsigned int global_error_flag = 0;
const char *dbg_level_str[] = {"NONE", "ERR", "WARN", "INIT", "INF0", "INF1", "INF2", "INF3", "INF4", "SPAM", "ALL"};


/******************************************************************************/
/* functions                                                                  */
/******************************************************************************/

void set_dbg_level(unsigned int dbg_level)
{
  xfs_dbg_level = ((dbg_level < DBG_LEVEL_MAX) ? dbg_level : DBG_LEVEL_MAX);
}

/******************************************************************************/

unsigned int get_dbg_level(void)
{
  return  ((xfs_dbg_level < DBG_LEVEL_MAX) ? xfs_dbg_level : DBG_LEVEL_MAX);
}


/******************************************************************************/

unsigned int parse_dbglvl(const char *str)
{
  unsigned int i, d, match = 0 ;

  for (i=0; i <=DBG_LEVEL_MAX; i++)
  {
    if (fstricmp(str, dbg_level_str[i]))
    {
      d = i;
      match = 1;
      break;
    }
  }
  if (!match)
  {
    if (fstricmp(str, "info"))
    {
      d = DBG_LEVEL_INF0;
    }
    else
    {
      d = xfs_atoi(str);
    }
  }

  if (d > DBG_LEVEL_MAX) d = DBG_LEVEL_MAX;

  return d;
}




/******************************************************************************/

void set_error_flag(unsigned int error_flag)
{
  global_error_flag |= error_flag;
}

/******************************************************************************/

int cmd_dbg(int argc, char **argv)
{
  unsigned int d;

  CLI_CMD_HELP("[<dbg_level>]","show or set debug level");

  if ((argc>1) && (*argv[1]))
  {
    d = parse_dbglvl(argv[1]);
    set_dbg_level(d);
  }
  d = get_dbg_level();
  if (d > DBG_LEVEL_MAX) d = DBG_LEVEL_MAX;
  xfs_printf("Debug level : %2d  (%s)\r\n", d, dbg_level_str[d]);

  return 0;
}

/******************************************************************************/

int mod_dbg_help(int argc, char **argv)
{
  int i;
  CLI_CMD_HELP("","debug functions");


  xfs_printf("\r\ndbg level\r\n");
  for (i=0; i <=DBG_LEVEL_MAX; i++)
  {
    xfs_printf("%2d %s\r\n", i, dbg_level_str[i]);
  }
  return 0;
}

/******************************************************************************/

cmd_table_entry_type cmd_table_mod_dbg[] =
{
  {"mod_dbg",    mod_dbg_help},
  {"dbg",        cmd_dbg},
  {0}
};

#endif /* DBG_NONE */

