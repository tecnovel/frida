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

#ifndef __CMD_PROCESSOR_H__
#define __CMD_PROCESSOR_H__

#include "xfs_utils.h"

/******************************************************************************/
/* constant definitions                                                       */
/******************************************************************************/

#define MAX_ARG_COUNT 10  /* including command name */


/******************************************************************************/
/* macro definitions                                                          */
/******************************************************************************/

#define CMD_HELP(HELP_PAR, HELP_SHORT, args...)    \
if ((argc > 1) && (fstrcmp(argv[0], "-?") ))       \
{                                                  \
  cmd_proc_help_par   = HELP_PAR;                  \
  cmd_proc_help_short = HELP_SHORT;                \
  cmd_proc_help_long  = (char*)args+0;             \
  return 0;                                        \
}

/*
#define CMD_HELP(HELP_PAR, HELP_SHORT, args...)    \
const static char cmd_help_par[] = HELP_PAR;       \
const static char cmd_help_short[] = HELP_SHORT;   \
if ((argc > 1) && (fstrcmp(argv[0], "-?") ))       \
{                                                  \
  cmd_proc_help_par   = cmd_help_par;              \
  cmd_proc_help_short = cmd_help_short;            \
  cmd_proc_help_long  = args+0;;                   \
  return 0;                                        \
}
*/

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef int (*cmd_func_ptr_type)(int argc, char **argv);

typedef struct
{
  int                arg_limit;
  const char         *cmd_name;
  cmd_func_ptr_type  cmd_func_ptr;
} cmd_table_entry_type;

extern cmd_table_entry_type *cmd_list[];


/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/



extern const char* cmd_proc_help_par;
extern const char* cmd_proc_help_short;
extern const char* cmd_proc_help_long;
extern cmd_table_entry_type help_cmd_table[];


/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

int  cmd_proc_split_args(char *line, int len, unsigned int max_args, int *argc_ptr, char **argv_ptr);
void cmd_proc_arg_info(int argc, char *argv[]);
int  cmd_proc_list_cmds(int argc, char **argv);
int  cmd_proc_help(int argc, char **argv);

int  cmd_process(char *buffer_i,unsigned int len);

/******************************************************************************/

#endif /* __CMD_PROCESSOR_H__ */
