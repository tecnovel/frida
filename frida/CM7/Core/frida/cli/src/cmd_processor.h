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

#ifndef CLI_MAX_ARG_COUNT
/*
 * count includes command name as argument 0
 * last argument contains rest of input which could not be split (if any)
 */
#define CLI_MAX_ARG_COUNT 11
#endif

#define CMD_PROG_ARGC_HELP_CALL  (-1)

#ifndef CLI_USE_PAR_HELP
#define CLI_USE_PAR_HELP    (1)
#endif

#ifndef CLI_USE_SHORT_HELP
#define CLI_USE_SHORT_HELP  (1)
#endif

#ifndef CLI_USE_LONG_HELP
#define CLI_USE_LONG_HELP   (1)
#endif

/******************************************************************************/
/* macro definitions                                                          */
/******************************************************************************/

#if CLI_USE_PAR_HELP
  #define CLI_HELP_PAR(str_ptr)     (str_ptr)
#else
  #define CLI_HELP_PAR(str_ptr)     ((char*)0)
#endif

#if CLI_USE_SHORT_HELP
  #define CLI_HELP_SHORT(str_ptr)   (str_ptr)
#else
  #define CLI_HELP_SHORT(str_ptr)   ((char*)0)
#endif

#if CLI_USE_LONG_HELP
  #define CLI_HELP_LONG(str_ptr)    ((char*)str_ptr+0)
#else
  #define CLI_HELP_LONG(str_ptr)    ((char*)0)
#endif

/******************************************************************************/

#define CLI_CMD_SHOW_HELP()       { xfs_printf("%s  %s  %s\r\n%s", argv[0], cmd_help_par, cmd_help_short, cmd_help_long); }
#define CLI_CMD_ARG_SYNTAX()      { xfs_printf("Invalid arguments. Syntax: %s %s\r\n", argv[0], cmd_help_par); }
#define CLI_CMD_ASSERT_ARGC_EQ(n) { if (argc != (n)) { CLI_CMD_ARG_SYNTAX(); return -1; } }
#define CLI_CMD_ASSERT_ARGC_GE(n) { if (argc <  (n)) { CLI_CMD_ARG_SYNTAX(); return -1; } }
#define CLI_CMD_ASSERT_ARGC_LE(n) { if (argc >  (n)) { CLI_CMD_ARG_SYNTAX(); return -1; } }

/******************************************************************************/

#define CLI_CMD_HELP_CHECK_HELP_CALL()       \
  if (argc == CMD_PROG_ARGC_HELP_CALL) { \
    argv[0] = (char*) cmd_help_par;      \
    argv[1] = (char*) cmd_help_short;    \
    argv[2] = (char*) cmd_help_long;     \
    return 0;                            \
  }

/******************************************************************************/

#define CLI_CMD_HELP_CHECK_SPLIT_ARGS()                     \
  if (argc > 1) {                                       \
    if (cmd_proc_arg_split_limit > 1) {                 \
      argc = cmd_proc_split_args(argv[1],               \
               cmd_proc_arg_split_limit, &argv[1]) + 1; \
    }                                                   \
    if (fstrcmp(argv[argc-1], "-h") ) {                 \
      CLI_CMD_SHOW_HELP();                                  \
      return 0;                                         \
    }                                                   \
  }

/******************************************************************************/

#define CMD_SPLIT_LIMIT(n)                                  \
  const int cmd_proc_arg_split_limit = (((n)>1)?((n)-1):0);

/******************************************************************************/

#define CLI_CMD_HELP(str_par, str_short, str_long...)            \
  static const char *cmd_help_par   = CLI_HELP_PAR(str_par);     \
  static const char *cmd_help_short = CLI_HELP_SHORT(str_short); \
  static const char *cmd_help_long  = CLI_HELP_LONG(str_long);  \
  CLI_CMD_HELP_CHECK_HELP_CALL();                                \
  CLI_CMD_HELP_CHECK_SPLIT_ARGS();

 // static const char *cmd_help_long  = CLI_HELP_LONG(str_long);

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef int (*cmd_func_ptr_type)(int argc, char **argv);

typedef struct
{
  const char         *cmd_name;
  cmd_func_ptr_type  cmd_func_ptr;
} cmd_table_entry_type;

extern cmd_table_entry_type *cmd_list[];


/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/

extern cmd_table_entry_type help_cmd_table[];
extern const int cmd_proc_arg_split_limit;

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

int  cmd_proc_split_args(char *line, int max_args, char **argv_ptr);
void cmd_proc_shift_args(int *argc, char **argv, int start, int shift, int max_args);
void cmd_proc_arg_info(int argc, char *argv[]);
int  cmd_proc_list_cmds(int argc, char **argv);
char* cmd_proc_remove_comment(char *s);
int  cmd_proc_help(int argc, char **argv);

int  cmd_process(char *cmd);

/******************************************************************************/

#endif /* __CMD_PROCESSOR_H__ */
