/*******************************************************************************
 *  Paul Scherrer Institut
 *------------------------------------------------------------------------------
 *
 *  Project :  generic
 *
 *  Author  :  schmid_e, theidel
 *  Created :  02.05.2014 13:24:35
 *
 *  Changes :  2022-11-01 major update by tg32
 *
 *  Description :  Processing the commands entered via the terminal
 *                 command line using the modular command implementation.
 *
 ******************************************************************************/


/*******************************************************************************
 * include files
 ******************************************************************************/

#include "cmd_processor.h"
#include "term_cmd_input.h"
#include "xfs_printf.h"
#include "xfs_utils.h"
#include "xfs_dbg.h"


/*******************************************************************************
 * global vars
 ******************************************************************************/


const int cmd_proc_arg_split_limit = (CLI_MAX_ARG_COUNT-1);

/*******************************************************************************
 * function definitions
 ******************************************************************************/


/*******************************************************************************
 * string MUST be null terminated !!!
 * last arguments contains remainder (if any) with quoting and spaces unchanged
 * overwrites input string!
 */

int cmd_proc_split_args(char *line, int max_args, char **argv_ptr)
{
  int i;
  int argc   = 0;
  int squote = 0;
  int dquote = 0;
  int ignore_quotes = 0;
  int start_new_arg = 1;
  char *args = line;

  /* initialize args */
  for (i=0; i < max_args; i++) argv_ptr[i] = 0;

  for (i=0; line[i] && (line[i] != 0x0a) && (line[i] != 0x0d); i++)
  {

    if ((line[i]=='\'') && (!dquote) && (!ignore_quotes))
    {
      squote = !squote;
    }
    else if ((line[i]=='"') && (!squote) && (!ignore_quotes))
    {
      dquote = !dquote;
    }
    else if (((line[i]==0x09) || (line[i]==0x20)) && (!squote) && (!dquote))
    {
      /* unquoted whitespace */
      *args++ = 0x00;
      start_new_arg = 1;
      /* do not change quotes for last argument */
      if (argc == max_args-1) ignore_quotes=1;
    }
    else /* no whitespace or within quotes */
    {
      if (start_new_arg)
      {
        if (argc < max_args-1)
        {
          argv_ptr[argc++] = args;
        }
        else
        {
          argv_ptr[argc++] = &line[i];
          return argc;
        }
        start_new_arg = 0;
      }
      *args++ = line[i];
    }
  }

  *args = 0x00; /* ensure that last arg is null terminated */
  return argc;
}


/******************************************************************************/

void cmd_proc_shift_args(int *argc, char **argv, int start, int shift, int max_args)
{
  int i, s;

  for (s=0; s<shift; s++) // iterate through shift because last argv{} may contain unsplit elements
  {
    int end = (*argc-2);
    for (i = start; i <= end; i++) argv[i] = argv[i + 1];

    if (*argc == max_args)
    {
      *argc = cmd_proc_split_args(argv[end], max_args-end, &argv[end]) + end;
    }
    else
    {
      *argc = (*argc > 0) ? (*argc - 1) : 0 ;
    }

  }
  for (i = *argc; i < max_args; i++) argv[i] =0; // empty arguments
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

char* cmd_proc_remove_comment(char *s)
{
  int  dquote  = 0;
  int  squote  = 0;
  int  start;
  int  end;
  int  i;

  for (i=0; s[i] && (s[i] != 0x0a) && (s[i] != 0x0d); i++)
  {
    if      ((s[i]=='\'')  && (!dquote)) squote = !squote;
    else if ((s[i]=='"')   && (!squote)) dquote = !dquote;
    else if ((s[i] == '#') && (!dquote)  && (!squote)) break;
  }

  start = skip_whitespace(s, 0,   0);
  end   = skip_whitespace(s, i-1, 1);

  if (end < start)
  {
    s[0] = 0;
    return s;
  }

  if (((s[start]=='"' && s[end]=='"') || (s[start]=='\'' && s[end]=='\'')))
  {
    end   = skip_whitespace(s, end-1, 1);
    start = skip_whitespace(s, start+1, 0);
  }
  s[end+1] = 0;
  return s+start;
}


/******************************************************************************/

int cmd_process(char *cmd)
{
  char *argv[CLI_MAX_ARG_COUNT];
  int argc;
  cmd_table_entry_type **cmd_list_ptr;
  cmd_table_entry_type *cmd_ptr;

  cmd = cmd_proc_remove_comment(cmd);
  if (!cmd[0]) return -1;

  /* first split in 2 parts for seperating command only */
  argc = cmd_proc_split_args(cmd, 2, argv);
  if (DBG_ALL) cmd_proc_arg_info(argc, argv);

  for (cmd_list_ptr = cmd_list; *cmd_list_ptr; cmd_list_ptr++)
  {
    for (cmd_ptr = *cmd_list_ptr; cmd_ptr->cmd_name != 0; cmd_ptr++)
    {
      if( fstrcmp(argv[0], cmd_ptr->cmd_name) )
      {
        if (DBG_ALL) cmd_proc_arg_info(argc, argv);
        return cmd_ptr->cmd_func_ptr(argc, argv);
      }
    }
  }

  xfs_printf("E%02X: Unknown Command: %s\r\n", ERR_UNKNOWN_CMD, argv[0]);
  return -1;
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
  char *help_argv[3]; /* pointer for help strings set by the called cmd function: */
                      /* help_argv[0] used for parameter help                     */
                      /* help_argv[1] used for short help                         */
                      /* help_argv[2] used for long help                          */
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
  char *cmd_sel = 0;
  char *cp;
  const char seperator_line[] = "-------------------------------------------------------------------------------------------------------------------------------";

  CLI_CMD_HELP("[-s|-c] [<cmd>|<module>] | -a | -h", "help for all or specified commands or modules",
           "  []        optional arguments\r\n"
           "  |         alternative arguments\r\n"
           "  -a        list all modules and commands\r\n"
           "  -s        short description only\r\n"
           "  -c        show command name only\r\n"
           "  <cmd>     command name (may end with * as wildcard)\r\n"
           "\r\n"
           "most commands can also be called with '-h' as last argument to show\r\n"
           "command specific help, e.g. printenv -h\r\n"
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
          cmd_ptr->cmd_func_ptr(CMD_PROG_ARGC_HELP_CALL, help_argv);
          if (!cmd_only)
          {
            if (get_len)
            {
              cmd_len = fstrlen(cmd_ptr->cmd_name) + 1;  /* * in front of module name */
              if (cmd_len > max_cmd_len) max_cmd_len = cmd_len;
            }
            else
            {
              if (cmd_all) xfs_printf("\r\n\r\n");
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%-%ds  %%-%ds   %%s\r\n", max_cmd_len, max_par_len);
              mod_len = xfs_printf(fmt_str, cmd_ptr->cmd_name, "", help_argv[1]);
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%.%ds\r\n", mod_len-2);
              if (cmd_all) xfs_printf(fmt_str, seperator_line); //----------------------------------------
            }
          }
          /* list functions only without wildcard match*/
          if (fstrcmp(cmd_sel,cmd_ptr->cmd_name)) list_mod_funcs = 1;
        }
        else if ((cmd_all) || (list_mod_funcs) || cmd_sel_match )
        {
          cmd_ptr->cmd_func_ptr(CMD_PROG_ARGC_HELP_CALL, help_argv);

          if (get_len)
          {
            cmd_len = fstrlen(cmd_ptr->cmd_name);
            par_len = fstrlen(help_argv[0]);
            if (cmd_len > max_cmd_len) max_cmd_len = cmd_len;
            if (par_len > max_par_len) max_par_len = par_len;
          }
          else
          {
            if (cmd_only)
            {
              xfs_printf("%s\r\n", cmd_ptr->cmd_name);
            }
            else
            {
              xfs_snprintf(fmt_str, sizeof(fmt_str), "%%-%ds %%-%ds  # %%s\r\n", max_cmd_len, max_par_len);
              xfs_printf(fmt_str, cmd_ptr->cmd_name, help_argv[0], help_argv[1]);
            }

            if ((cmd_sel) && (!list_mod_funcs) && (help_argv[2]) && (!short_only)  && (!cmd_only))
            {
               cmd_proc_print_long_help(help_argv[2]);
            }
          }
        }
        module = 0;
      }
    }
  }
  if (argc==1)
  {
    xfs_printf("\r\nuse 'help <module>' or 'help <cmd>' or 'help help' for more information\r\n");
    xfs_printf("    '<cmd> -h' with '-h' as last argument is also accepted by most commands\r\n\r\n");
  }

  return 0;
}


/******************************************************************************/

int module_cmd_proc(int argc, char **argv)
{
  CLI_CMD_HELP("","cmd processor");
  return 0;
}

/******************************************************************************/

int cli_cmd_history(int argc, char **argv)
{
  CLI_CMD_HELP("","cmd history");
  int i;

  if (!cli_current_interface) return 0;

  for (i = cli_cmd_history_entries(cli_current_interface) - 1; i >= 0; i--)
  {
    xfs_local_printf("[%d] = %s\r\n", i, cli_cmd_history_get(cli_current_interface, i) );
  }

  return 0;
}



/************************************************************/
/* COMMAND TABLE                                            */
/************************************************************/

cmd_table_entry_type cmd_table_cmd_proc[] =
{
  {"cmd",     module_cmd_proc},
  {"help",    cmd_proc_help},
  {"history", cli_cmd_history},
  {0}
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
