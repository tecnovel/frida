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

/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/

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
