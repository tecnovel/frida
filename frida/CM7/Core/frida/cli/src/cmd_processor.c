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

#include "cmd_processor.h"
#include "xfs_printf.h"
#include "xfs_utils.h"
#include <string.h>
#include "xfs_dbg.h"


/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/

const char* cmd_proc_help_par;
const char* cmd_proc_help_short;
const char* cmd_proc_help_long;

/******************************************************************************/
/* function definitions                                                       */
/******************************************************************************/


/******************************************************************************/
/* len can be set < 0 if string is null terminated
 * last arguments contains remainder (if any) with quoting and spaces unchanged
 * overwrites input string!
 */

int cmd_proc_split_args(char *line, int len, unsigned int max_args, int *argc_ptr, char **argv_ptr)
{
  unsigned int i;
  char c;
  int idx = 0;
  int squote = 0;
  int dquote = 0;
  int ignore_quotes = 0;
  int start_new_arg = 1;
  char *args = line;

  /* initialize args */
  for(i=0; i < max_args; i++) argv_ptr[i] = NULL;
  *argc_ptr = 0;

  while(((idx<len) || (len<0)) && line[idx] && (line[idx]!=0x0a) && (line[idx]!=0x0d))
  {
    c = line[idx++];

    if ((c=='\'') && (!dquote) && (!ignore_quotes))
    {
      squote = !squote;
    }
    else if ((c=='"') && (!squote) && (!ignore_quotes))
    {
      dquote = !dquote;
    }
    else if (((c==0x09) || (c==0x20)) && (!squote) && (!dquote))
    {
      /* unquoted whitespace */
      *args++ = 0x00;
      start_new_arg = 1;
      /* do not change quotes for last argument */
      if (*argc_ptr == (int)max_args-1) ignore_quotes=1;
    }
    else /* no whitespace or within quotes */
    {
      if (start_new_arg)
      {
        if (*argc_ptr < (int)max_args-1)
        {
          argv_ptr[(*argc_ptr)++] = args;
        }
        else
        {
          argv_ptr[(*argc_ptr)++] = &line[--idx];
          return idx;
        }
        start_new_arg = 0;
      }
      *args++ = c;
    }
  }

  *args = 0x00;

  return idx;
}

/******************************************************************************/

void cmd_proc_arg_info(int argc, char *argv[])
{
  int i;

  for (i=0; i<argc; i++)
  {
    xfs_printf("argv[%d] = %s\r\n", i, argv[i]);
  }
}

/******************************************************************************/

int cmd_process(char *buffer_i, unsigned int len)
{
  char *argv[MAX_ARG_COUNT];
  int argc;
  unsigned int idx;
  int arg_limit;
  cmd_table_entry_type **cmd_list_ptr;
  cmd_table_entry_type *cmd_ptr;

  /* first split in 2 parts for seperating command only */
  idx = cmd_proc_split_args(buffer_i, len, 2, &argc, argv);
  if (DBG_ALL) cmd_proc_arg_info(argc, argv);

  for (cmd_list_ptr = cmd_list; *cmd_list_ptr; cmd_list_ptr++)
  {
    for (cmd_ptr = *cmd_list_ptr; cmd_ptr->cmd_name != NULL; cmd_ptr++)
    {
      if( fstrcmp(argv[0], cmd_ptr->cmd_name) )
      {
        /* 0 means no limit, i.e. set to MAX */
        arg_limit = cmd_ptr->arg_limit ? cmd_ptr->arg_limit : MAX_ARG_COUNT;

        if ((argc > 1) && (arg_limit>argc) && (idx<len))
        {
          /* split other arguments */
          cmd_proc_split_args(argv[1], len-idx, MAX_ARG_COUNT-1, &argc, argv+1);
          argc++;
        }
        if (DBG_ALL) cmd_proc_arg_info(argc, argv);
        cmd_ptr->cmd_func_ptr(argc, argv);
        return 0;
      }
    }
  }

  xfs_printf("E%02X: Unknown Command: %s\r\n", ERR_UNKNOWN_CMD, argv[0]);

  return 0;

}


/******************************************************************************/

void cmd_proc_print_long_help(const char* str)
{
  char fmt_str[20];
  const char* cp_start = str;
  const char* cp;

  if (!str) return;

  for (cp = str; *cp; cp++)
  {
    if (*cp == '\n')
    {
      xfs_snprintf(fmt_str, sizeof(fmt_str), "%%.%ds", ++cp - cp_start);
      xfs_printf(fmt_str, cp_start);
      if (!*cp) return;
      cp_start = cp;
    }
  }
}


/******************************************************************************/

int cmd_proc_help(int argc, char **argv)
{
  cmd_table_entry_type **cmd_list_ptr;
  cmd_table_entry_type *cmd_ptr;
  char help_arg[] = "-?";
  char *help_argv[2] = {help_arg, help_arg};
  char fmt_str[40];
  int get_len;
  int do_get_len = 1;
  unsigned int max_cmd_len = 0;
  unsigned int mod_len;
  unsigned int max_par_len = 0;
  unsigned int cmd_len = 0;
  unsigned int par_len = 0;

  unsigned int short_only = 0;
  unsigned int cmd_only   = 0;
  unsigned int cmd_all    = 0;
  unsigned int arg_cmd    = 2;
  unsigned int module;
  unsigned int cmd_sel_match;
  unsigned int list_mod_funcs = 0;
  char *cmd_sel = NULL;
  char *cp;

  CMD_HELP("[-a|-s|-c] [<cmd>|<module>]", "help for all or specified commands or modules",
           "  []        optional arguments\r\n"
           "  |         alternative arguments\r\n"
           "  -a        list all modules and commands\r\n"
           "  -s        short description only\r\n"
           "  -c        show command name only\r\n"
           "  <cmd>     command name (may end with * as wildcard)\r\n"
           "  <module>  module name (may end with * as wildcard)\r\n"
          );

  if (argc > 1)
  {
    if      (fstrcmp(argv[1], "-s")) short_only = 1;
    else if (fstrcmp(argv[1], "-c")) cmd_only = 1;
    else if (fstrcmp(argv[1], "-a")) cmd_all = 1;
    else    arg_cmd--;

    if ((int)arg_cmd < argc) cmd_sel = argv[arg_cmd];
  }

  if (cmd_only)
  {
     do_get_len = 0;
     if (argc == 2) cmd_all = 1;
  }

  for (cp = cmd_sel; cp && *cp; cp++)
  {
    if (*cp == '*')
    {
      short_only = 1;
      break;
    }
  }


  if (argc==1) xfs_printf("List of available modules:\r\n\r\n");
  for (get_len = do_get_len; get_len >= 0; get_len--)
  {
    for (cmd_list_ptr = cmd_list; *cmd_list_ptr; cmd_list_ptr++)
    {
      module = 1;
      list_mod_funcs = 0;
      for (cmd_ptr = *cmd_list_ptr; cmd_ptr->cmd_name && cmd_ptr->cmd_func_ptr; cmd_ptr++)
      {
        cmd_sel_match = fstrcmpwc(cmd_sel, cmd_ptr->cmd_name);
        if (module && ( cmd_sel_match || cmd_all|| !cmd_sel))
        {

          cmd_ptr->cmd_func_ptr(2, help_argv);
          if (!cmd_only)
          {
            if (get_len)
            {
              cmd_len = strlen(cmd_ptr->cmd_name) + 1;  /* * in front of module nbame */
              par_len = 7; /* strlen("*module"); */
              if (cmd_len > max_cmd_len) max_cmd_len = cmd_len;
              if (par_len > max_par_len) max_par_len = par_len;
            }
            else
            {

              if (cmd_all) xfs_printf("\r\n\r\n");
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%-%ds  %%-%ds  %%s\r\n", max_cmd_len, max_par_len);
              mod_len = xfs_printf(fmt_str, cmd_ptr->cmd_name,"*module", cmd_proc_help_short);
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%.%ds\r\n", mod_len-2);
              if (cmd_all) xfs_printf(fmt_str,"-------------------------------------------------------------------------------------------------------------------------------");

            }
          }
          /* list functions only without wildcard match*/
          if (fstrcmp(cmd_sel,cmd_ptr->cmd_name)) list_mod_funcs = 1;
        }
        else if ((cmd_all) || (list_mod_funcs) || cmd_sel_match )
        {
          cmd_proc_help_par   = "";
          cmd_proc_help_short = "";
          cmd_ptr->cmd_func_ptr(2, help_argv);

          if (get_len)
          {
            cmd_len = strlen(cmd_ptr->cmd_name);
            par_len = strlen(cmd_proc_help_par);
            if (cmd_len > max_cmd_len) max_cmd_len = cmd_len;
            if (par_len > max_par_len) max_par_len = par_len;
          }
          else
          {
            if(cmd_only)
            {
              xfs_printf("%s\r\n", cmd_ptr->cmd_name);
            }
            else
            {
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%-%ds  %%-%ds  %%s\r\n", max_cmd_len, max_par_len);
              xfs_printf(fmt_str, cmd_ptr->cmd_name, cmd_proc_help_par, cmd_proc_help_short);
            }

            if ((cmd_sel) && (!list_mod_funcs) && (cmd_proc_help_long) && (!short_only)  && (!cmd_only))
            {
               cmd_proc_print_long_help(cmd_proc_help_long);
            }
          }
        }
        module = 0;
      }
    }
  }
  if (argc==1) xfs_printf("\r\nuse 'help <module>' or 'help <cmd>' or 'help help' for more information\r\n");

  return 0;
}


/******************************************************************************/

int module_cmd_proc(int argc, char **argv)
{
  CMD_HELP("","cmd processor");
  return 0;

}

/************************************************************/
/* COMMAND TABLE                                            */
/************************************************************/

cmd_table_entry_type cmd_table_cmd_proc[] =
{
  {0, "cmd",        module_cmd_proc},
  {0, "help",       cmd_proc_help},
  {0, NULL, NULL}
};


/******************************************************************************/
/*                                                                            */
/* Generate global cmd_list with information from "mod_config.h"              */
/*                                                                            */
/******************************************************************************/

#define DECLARE_CMD_TABLE_ENTRY_0(n)
#define DECLARE_CMD_TABLE_ENTRY_1(n) extern cmd_table_entry_type cmd_table_##n[];

#define CMD_TABLE_ENTRY_0(n)
#define CMD_TABLE_ENTRY_1(n) cmd_table_##n,


/* generate cmd_table_entry_type declarations */

#undef  MODULE
#define MODULE(state, name)  DECLARE_CMD_TABLE_ENTRY_##state(name)

#include "../mod_config.h"


/* generate cmd_list */

#undef  MODULE
#define MODULE(state, name)  CMD_TABLE_ENTRY_##state(name)

  cmd_table_entry_type *cmd_list[] =
  {
    #include "../mod_config.h"
    (cmd_table_entry_type*)0
  };


/******************************************************************************/
