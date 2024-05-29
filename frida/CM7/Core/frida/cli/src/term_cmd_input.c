/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, theidel
 *  Created :  02.05.2014 13:24:35
 *
 *  Changes :  2022-11-01 major update by tg32
 *
 *  Description :  Capturing commands from terminal.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_printf.h"
#include "xfs_utils.h"
#include "term_cmd_input.h"

#ifdef LINUX_COMPILE
#include <unistd.h>
#endif


#define CLI_DBG_HISTORY 0

#if TERM_USE_SEQ_NAME
#define KEY_ENTRY(name) name, #name
#else
#define KEY_ENTRY(name) name
#endif


/******************************************************************************/
/* ESC sequence table                                                         */
/******************************************************************************/

static const keycode_t esc_seq_table[] = {
    { "[A",     KEY_ENTRY(KEY_UP)          },
    { "[B",     KEY_ENTRY(KEY_DOWN)        },
    { "[C",     KEY_ENTRY(KEY_RIGHT)       },
    { "[D",     KEY_ENTRY(KEY_LEFT)        },
    { "[E",     KEY_ENTRY(KEY_BEGIN)       },
    { "[F",     KEY_ENTRY(KEY_END)         },
    { "[G",     KEY_ENTRY(KEY_BEGIN)       },
    { "[H",     KEY_ENTRY(KEY_HOME)        },
    { "[1~",    KEY_ENTRY(KEY_HOME)        },
    { "[2~",    KEY_ENTRY(KEY_INS)         },
    { "[3~",    KEY_ENTRY(KEY_DEL)         },
    { "[4~",    KEY_ENTRY(KEY_END)         },
    { "[5~",    KEY_ENTRY(KEY_PGUP)        },
    { "[6~",    KEY_ENTRY(KEY_PGDWN)       },
    { "OF",     KEY_ENTRY(KEY_END)         },
    { "OH",     KEY_ENTRY(KEY_HOME)        },

    { "[1;2A",  KEY_ENTRY(KEY_SHIFT_UP)    },
    { "[1;2B",  KEY_ENTRY(KEY_SHIFT_DOWN)  },
    { "[1;2C",  KEY_ENTRY(KEY_SHIFT_RIGHT) },
    { "[1;2D",  KEY_ENTRY(KEY_SHIFT_LEFT)  },
    { "[1;2E",  KEY_ENTRY(KEY_SHIFT_BEGIN) },
    { "[1;2F",  KEY_ENTRY(KEY_SHIFT_END)   },
    { "[1;2H",  KEY_ENTRY(KEY_SHIFT_HOME)  },
    { "[3;2~",  KEY_ENTRY(KEY_SHIFT_DEL)   },

    { "[2;5A",  KEY_ENTRY(KEY_SHIFT_UP)    },
    { "[2;5B",  KEY_ENTRY(KEY_SHIFT_DOWN)  },
    { "[2;5C",  KEY_ENTRY(KEY_SHIFT_RIGHT) },
    { "[2;5D",  KEY_ENTRY(KEY_SHIFT_LEFT)  },

    { "[1;3A",  KEY_ENTRY(KEY_ALT_UP)      },
    { "[1;3B",  KEY_ENTRY(KEY_ALT_DOWN)    },
    { "[1;3C",  KEY_ENTRY(KEY_ALT_RIGHT)   },
    { "[1;3D",  KEY_ENTRY(KEY_ALT_LEFT)    },
    { "[1;3E",  KEY_ENTRY(KEY_ALT_BEGIN)   },
    { "[1;3F",  KEY_ENTRY(KEY_ALT_END)     },
    { "[1;3H",  KEY_ENTRY(KEY_ALT_HOME)    },
    { "[5;3~",  KEY_ENTRY(KEY_ALT_PGUP)    },
    { "[6;3~",  KEY_ENTRY(KEY_ALT_PGDOWN)  },
    { "[3;5A",  KEY_ENTRY(KEY_ALT_UP)      },
    { "[3;5B",  KEY_ENTRY(KEY_ALT_DOWN)    },
    { "[3;5C",  KEY_ENTRY(KEY_ALT_RIGHT)   },
    { "[3;5D",  KEY_ENTRY(KEY_ALT_LEFT)    },

    { "[1;5A",  KEY_ENTRY(KEY_CTRL_UP)     },
    { "[1;5B",  KEY_ENTRY(KEY_CTRL_DOWN)   },
    { "[1;5C",  KEY_ENTRY(KEY_CTRL_RIGHT)  },
    { "[1;5D",  KEY_ENTRY(KEY_CTRL_LEFT)   },
    { "[1;5E",  KEY_ENTRY(KEY_CTRL_BEGIN)  },
    { "[1;5F",  KEY_ENTRY(KEY_CTRL_END)    },
    { "[1;5H",  KEY_ENTRY(KEY_CTRL_HOME)   },
    { "[5;5~",  KEY_ENTRY(KEY_CTRL_PGUP)   },
    { "[6;5~",  KEY_ENTRY(KEY_CTRL_PGDOWN) },
    { "OA",     KEY_ENTRY(KEY_CTRL_UP)     },
    { "OB",     KEY_ENTRY(KEY_CTRL_DOWN)   },
    { "OC",     KEY_ENTRY(KEY_CTRL_RIGHT)  },
    { "OD",     KEY_ENTRY(KEY_CTRL_LEFT)   },

#if TERM_USE_FUNCTION_KEYS
    { "[11~",   KEY_ENTRY(KEY_F1)          },
    { "[12~",   KEY_ENTRY(KEY_F2)          },
    { "[13~",   KEY_ENTRY(KEY_F3)          },
    { "[14~",   KEY_ENTRY(KEY_F4)          },
    { "[15~",   KEY_ENTRY(KEY_F5)          },
    { "[17~",   KEY_ENTRY(KEY_F6)          },
    { "[18~",   KEY_ENTRY(KEY_F7)          },
    { "[19~",   KEY_ENTRY(KEY_F8)          },
    { "[20~",   KEY_ENTRY(KEY_F9)          },
    { "[21~",   KEY_ENTRY(KEY_F10)         },
    { "[23~",   KEY_ENTRY(KEY_F11)         },
    { "[24~",   KEY_ENTRY(KEY_F12)         },

    { "[1P",    KEY_ENTRY(KEY_F1)          },
    { "[1Q",    KEY_ENTRY(KEY_F2)          },
    { "[1R",    KEY_ENTRY(KEY_F3)          },
    { "[1S",    KEY_ENTRY(KEY_F4)          },

    { "OP",     KEY_ENTRY(KEY_F1)          },
    { "OQ",     KEY_ENTRY(KEY_F2)          },
    { "OR",     KEY_ENTRY(KEY_F3)          },
    { "OS",     KEY_ENTRY(KEY_F4)          },

    { "[11;2~", KEY_ENTRY(KEY_SHIFT_F1)    },
    { "[12;2~", KEY_ENTRY(KEY_SHIFT_F2)    },
    { "[13;2~", KEY_ENTRY(KEY_SHIFT_F3)    },
    { "[14;2~", KEY_ENTRY(KEY_SHIFT_F4)    },
    { "[15;2~", KEY_ENTRY(KEY_SHIFT_F5)    },
    { "[17;2~", KEY_ENTRY(KEY_SHIFT_F6)    },
    { "[18;2~", KEY_ENTRY(KEY_SHIFT_F7)    },
    { "[19;2~", KEY_ENTRY(KEY_SHIFT_F8)    },
    { "[20;2~", KEY_ENTRY(KEY_SHIFT_F9)    },
    { "[21;2~", KEY_ENTRY(KEY_SHIFT_F10)   },
    { "[23;2~", KEY_ENTRY(KEY_SHIFT_F11)   },
    { "[24;2~", KEY_ENTRY(KEY_SHIFT_F12)   },

    { "[1;2P",  KEY_ENTRY(KEY_SHIFT_F1)    },
    { "[1;2Q",  KEY_ENTRY(KEY_SHIFT_F2)    },
    { "[1;2R",  KEY_ENTRY(KEY_SHIFT_F3)    },
    { "[1;2S",  KEY_ENTRY(KEY_SHIFT_F4)    },

    { "[11;3~", KEY_ENTRY(KEY_ALT_F1)      },
    { "[12;3~", KEY_ENTRY(KEY_ALT_F2)      },
    { "[13;3~", KEY_ENTRY(KEY_ALT_F3)      },
    { "[14;3~", KEY_ENTRY(KEY_ALT_F4)      },
    { "[15;3~", KEY_ENTRY(KEY_ALT_F5)      },
    { "[17;3~", KEY_ENTRY(KEY_ALT_F6)      },
    { "[18;3~", KEY_ENTRY(KEY_ALT_F7)      },
    { "[19;3~", KEY_ENTRY(KEY_ALT_F8)      },
    { "[20;3~", KEY_ENTRY(KEY_ALT_F9)      },
    { "[21;3~", KEY_ENTRY(KEY_ALT_F10)     },
    { "[23;3~", KEY_ENTRY(KEY_ALT_F11)     },
    { "[24;3~", KEY_ENTRY(KEY_ALT_F12)     },

    { "[1;3P",  KEY_ENTRY(KEY_ALT_F1)      },
    { "[1;3Q",  KEY_ENTRY(KEY_ALT_F2)      },
    { "[1;3R",  KEY_ENTRY(KEY_ALT_F3)      },
    { "[1;3S",  KEY_ENTRY(KEY_ALT_F4)      },

    { "[11;5~", KEY_ENTRY(KEY_CTRL_F1)     },
    { "[12;5~", KEY_ENTRY(KEY_CTRL_F2)     },
    { "[13;5~", KEY_ENTRY(KEY_CTRL_F3)     },
    { "[14;5~", KEY_ENTRY(KEY_CTRL_F4)     },
    { "[15;5~", KEY_ENTRY(KEY_CTRL_F5)     },
    { "[17;5~", KEY_ENTRY(KEY_CTRL_F6)     },
    { "[18;5~", KEY_ENTRY(KEY_CTRL_F7)     },
    { "[19;5~", KEY_ENTRY(KEY_CTRL_F8)     },
    { "[20;5~", KEY_ENTRY(KEY_CTRL_F9)     },
    { "[21;5~", KEY_ENTRY(KEY_CTRL_F10)    },
    { "[23;5~", KEY_ENTRY(KEY_CTRL_F11)    },
    { "[24;5~", KEY_ENTRY(KEY_CTRL_F12)    },

    { "[1;5P",  KEY_ENTRY(KEY_CTRL_F1)     },
    { "[1;5Q",  KEY_ENTRY(KEY_CTRL_F2)     },
    { "[1;5R",  KEY_ENTRY(KEY_CTRL_F3)     },
    { "[1;5S",  KEY_ENTRY(KEY_CTRL_F4)     },
#endif
};



/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/

term_cmd_input_type* cli_current_interface = 0;

unsigned int term_get_ticks_fb(void);
tick_timeout_t term_timeout = {term_get_ticks_fb, 100}; /* Use Fallback function as default */


/******************************************************************************/
/* function definitions                                                       */
/******************************************************************************/

void term_set_timeout_ticks(term_func_get_ticks get_ticks, unsigned int timeout_ticks)
{
  term_timeout.get_ticks     = get_ticks;
  term_timeout.timeout_ticks = timeout_ticks;
}

/******************************************************************************/
/* Fallback get_ticks function just incrementing ticks every call */

unsigned int term_get_ticks_fb(void)
{
  static unsigned int ticks = 0;
  return ++ticks;
}


/******************************************************************************/

void cli_clear_history(term_cmd_input_type* self)
{
  int i, n;

  for (i = 0; i < CLI_MAX_HISTORY_SIZE; i++) {
    for (n = 0; n < CLI_MAX_CMD_LEN; n++) {
      self->history[i][n] = 0;
    }
  }

  self->hist_wr_ptr  = 0;
  self->hist_rd_ptr  = 0;
  self->hist_entries = 0;
}


/******************************************************************************/


void term_cmd_input_init(term_cmd_input_type* self, xfs_term *term, term_func_getprompt fct_getprompt, enum prompt_opt newline)
{
  self->cli_active          = 0;
  self->cmd_pos             = 0;
  self->cmd_len             = 0;
  self->prompt_len          = 0;
  self->cursor_pos          = 0;
  self->command[0]          = 0;
  self->term                = term;
  self->rdkey.term          = term;
  self->rdkey.esc_seq_ptr   = 0;
  self->rdkey.esc_ticks     = 0;
  self->fct_getprompt       = fct_getprompt;
  self->newline             = newline;

  cli_clear_history(self);
}


/******************************************************************************/

#if CLI_DBG_HISTORY
void cli_dbg_history(term_cmd_input_type* self)
{
  int i;
  xfs_local_printf("self->cmd_pos      = %d\r\n", self->cmd_pos );
  xfs_local_printf("self->cursor_pos   = %d\r\n\r\n", self->cursor_pos );

  xfs_local_printf("self->hist_wr_ptr  = %d\r\n", self->hist_wr_ptr);
  xfs_local_printf("self->hist_rd_ptr  = %d\r\n", self->hist_rd_ptr);
  xfs_local_printf("self->hist_entries = %d\r\n", self->hist_entries);

  for (i=0; i<self->hist_entries; i++)
  {
    xfs_local_printf("self->history[%d] = %s\r\n", i, self->history[i]);
  }
}
#endif


/******************************************************************************/

void cli_cmd_history_add(term_cmd_input_type* self, char* cmd) {
  int n = 0;
  char c = 0;
  int cpy_from_src = 1;

  for (n = 0; n < CLI_MAX_CMD_LEN; n++) {
    if (cpy_from_src) {
      c = cmd[n];
      if (!c) cpy_from_src = 0;
    }
    self->history[self->hist_wr_ptr][n] = c;
  }

  if (self->hist_entries < CLI_MAX_HISTORY_SIZE) ++self->hist_entries;

  self->hist_wr_ptr = (self->hist_wr_ptr + 1) % CLI_MAX_HISTORY_SIZE;
  self->hist_rd_ptr = self->hist_wr_ptr;
}


/******************************************************************************/

char* cli_cmd_history_get(term_cmd_input_type* self, int past)
{
  int pos = 0;
  if (past < self->hist_entries) pos = (self->hist_wr_ptr + CLI_MAX_HISTORY_SIZE - 1 - past) % CLI_MAX_HISTORY_SIZE;
  return self->history[pos];
}


/******************************************************************************/

void cli_history_select(term_cmd_input_type* self, int direction)
{
  int next_rd_ptr;

  if (direction < 0)
  {
    /* previous command */
    next_rd_ptr = self->hist_rd_ptr - 1;
    if (next_rd_ptr <  0)                    next_rd_ptr = self->hist_entries;
    if (next_rd_ptr >= CLI_MAX_HISTORY_SIZE) next_rd_ptr = CLI_MAX_HISTORY_SIZE-1;
    if (next_rd_ptr == self->hist_wr_ptr)    return;  /* already on oldest entry */
  }
  else if (self->hist_rd_ptr != self->hist_wr_ptr) /* if not current command  */
  {
    /* next command */
    next_rd_ptr = self->hist_rd_ptr + 1;
    if (next_rd_ptr > self->hist_entries) next_rd_ptr = 0;
    if (next_rd_ptr >=  CLI_MAX_HISTORY_SIZE) next_rd_ptr = 0;
  }
  else
  {
    return;
  }

  /* save current command to history */
  self->command[self->cmd_pos] = 0;
  fstrncpy(self->history[self->hist_rd_ptr], self->command, CLI_MAX_CMD_LEN);

  /* delete the terminal output */
  term_cursor_left(self->cursor_pos);   /* move cursor left to start */
  term_erase_to_end();

  /* use next read pointer */
  self->hist_rd_ptr = next_rd_ptr;

  /* copy the history command to the command buffer */
  fstrncpy(self->command, self->history[self->hist_rd_ptr], CLI_MAX_CMD_LEN);
  self->cmd_pos = fstrnlen(self->command, CLI_MAX_CMD_LEN);
  self->cursor_pos = self->cmd_pos;

#if CLI_DBG_HISTORY
  cli_dbg_history(self);
#endif

  /* print the command buffer */
  xfs_local_print(self->command);
}


/******************************************************************************/

void cli_history_update(term_cmd_input_type* self)
{
  int matches_latest_hist_entry = 0;

  if (self->hist_entries > 0)
  {
    int tmp_wr_ptr = self->hist_wr_ptr - 1;
    if (tmp_wr_ptr <  0) tmp_wr_ptr = self->hist_entries-1;
    matches_latest_hist_entry = fstrcmp(self->history[tmp_wr_ptr], self->command);
  }

  if ((self->cmd_pos > 0) && !matches_latest_hist_entry)
  {
    /* if there is a command and it's not the same as the latest entry, add it to the history */
    cli_cmd_history_add(self, self->command);
  } else {
    self->hist_rd_ptr = self->hist_wr_ptr;
  }

#if CLI_DBG_HISTORY
  cli_dbg_history(self);
#endif
}


/******************************************************************************/

int is_word_char(char c)
{
  if (c >= '0' && c <= '9') return 1;
  if (c >= 'A' && c <= 'Z') return 1;
  if (c >= 'a' && c <= 'z') return 1;
  return 0;
}


/******************************************************************************/

int skip_char_l(char* buf, int max, int pos, int non)
{
  if (pos > 0) pos--;
  while ((pos > 0) && (pos <= max) && (is_word_char(buf[pos])^non) ) pos--;
  if ((pos > 0) && (pos < max)) pos++;
  return pos;
}


/******************************************************************************/

int skip_char_r(char* buf, int max, int pos, int non)
{
  while ((pos >= 0) && (pos < max) && (is_word_char(buf[pos])^non) ) pos++;
  return pos;
}


/******************************************************************************/

void term_prompt(term_cmd_input_type* self, int action)
{
  if (action == TERM_CLEAR_LINE)
  {
    term_erase_line();
    term_cursor_return();
  }
  else if (action == TERM_CLEAR_SCREEN)
  {
    term_erase_screen();
    term_cursor_home();
  }
  else if (action == TERM_NEW_LINE)
  {
    term_cursor_newline();
  }
  if (self->fct_getprompt)
  {
    self->prompt_len = xfs_local_printf("%s > ", self->fct_getprompt());
    xfs_local_printf("%s", self->command);
  }
  else
  {
    self->prompt_len = 2;
    xfs_local_printf("> %s", self->command);
  }
  term_cursor_left(self->cmd_pos - self->cursor_pos);
}


/******************************************************************************/

void term_hide_cli(term_cmd_input_type* self)
{
  self->cli_active = 0;

//  term_erase_line();
//  term_cursor_return();

  term_cursor_left(self->prompt_len + self->cursor_pos );
  term_erase_to_end();
}

/******************************************************************************/

int term_read_key(term_readkey_type* self)
{
  int i;
  int c;

  #if TERM_USE_SEQ_NAME
    self->seq_name = "";
  #endif

  c = (self->term && self->term->getchar) ? self->term->getchar() : xfs_getchar();

  if (c != XFS_EOF)
  {
  //  xfs_local_printf("c =  %c  (%d)  \r\n",c ,c);

    if (c == KEY_ESC)  /* escape */
    {
      self->esc_ticks = term_get_ticks();
      /* make sure that esc_ticks is not 0 */
      if (!self->esc_ticks) self->esc_ticks = 1;
      self->esc_seq_ptr = 0;
      return KEY_HANDLING_ESC_SEQ;
    }
    else if (self->esc_ticks)
    {
      if (self->esc_seq_ptr < CLI_MAX_ESC_SEQ)
      {
        self->esc_seq_buf[self->esc_seq_ptr++] = c;
        self->esc_seq_buf[self->esc_seq_ptr  ] = 0;
      }

      /* End of Escape Sequence */
      if ( ((c >= 'A') && (c <='H')) || ((c >= 'P') && (c <='Z')) || (c=='~') || ((c >= 'a') && (c <='z')) )
      {
        /* xfs_local_printf("Escape Sequence : %s\r\n", self->esc_seq_buf); */
        self->esc_ticks = 0;
        for (i=0; i< (sizeof(esc_seq_table) / sizeof(keycode_t)); i++)
        {
          if (fstrcmp(self->esc_seq_buf, esc_seq_table[i].esc_seq))
          {
            #if TERM_USE_SEQ_NAME
              self->seq_name = esc_seq_table[i].seq_name;
            #endif
            return esc_seq_table[i].key;
          }
        }
        return KEY_UNKNOWN_ESC_SEQ;
      }
      return KEY_HANDLING_ESC_SEQ;
    }
    return c;
  }
  else if (self->esc_ticks)
  {
    if ((term_get_ticks() - self->esc_ticks) > term_timeout.timeout_ticks)
    {
      self->esc_ticks = 0;  /* abort ESC Sequence after timeout and discard buffer */
      return KEY_ESC;
    }
  }
  return KEY_NO_CHAR; /* no new character */
}


/******************************************************************************/

int term_cmd_input_get_cmd(term_cmd_input_type* self)
{
  int c;

  cli_current_interface = self;

  if (!self->term) return KEY_NO_CHAR;
  xfs_term_set_std(self->term);

  if (self->cmd_len > 0)
  {
    self->command[0] = 0;
    self->cmd_len    = 0;
  }

  if (self->cli_active == 0)
  {
    term_prompt(self, self->newline);
  }

  /* disable cli_active and set it when leaving function (if it is still active),                  */
  /* otherwise calls to xfs_[local_]print[f] during this function would hide command line again... */
  self->cli_active = 0;

  c = term_read_key(&(self->rdkey));

  if (c >= 0)
  {
    unsigned int n;

    if (c == KEY_UP) {
      cli_history_select(self, -1);
    }
    else if (c == KEY_DOWN) {
      cli_history_select(self, +1);
    }
    else if (c == KEY_RIGHT) {
      if (self->cursor_pos < self->cmd_pos) {
        term_cursor_right(1);
        self->cursor_pos++;
      }
    }
    else if (c == KEY_LEFT) {
      if (self->cursor_pos > 0) {
        term_cursor_left(1);
        self->cursor_pos--;
      }
    }
    else if (c == KEY_CTRL_RIGHT) {
      n = skip_char_r(self->command, self->cmd_pos, self->cursor_pos, 1);
      n = skip_char_r(self->command, self->cmd_pos, n, 0);
      term_cursor_right(n - self->cursor_pos);
      self->cursor_pos = n;
    }
    else if (c == KEY_CTRL_LEFT) {
      n = skip_char_l(self->command, self->cmd_pos, self->cursor_pos, 1);
      n = skip_char_l(self->command, self->cmd_pos, n, 0);
      term_cursor_left(self->cursor_pos - n);
      self->cursor_pos = n;
    }
    else if (c == KEY_HOME) {
      term_cursor_left(self->cursor_pos);
      self->cursor_pos= 0;
    }
    else if (c == KEY_END)
    {
      term_cursor_right(self->cmd_pos - self->cursor_pos);
      self->cursor_pos = self->cmd_pos;
    }
    else if (c == KEY_DEL)
    {
      if(self->cursor_pos < self->cmd_pos)
      {
        for(n=self->cursor_pos; n < self->cmd_pos; n++) self->command[n] = self->command[n+1];
        --self->cmd_pos;
        xfs_local_print(&self->command[self->cursor_pos]);
        term_erase_to_end();
        term_cursor_left(self->cmd_pos - self->cursor_pos);
      }
    }
    else if ((c == KEY_CTRL_H) || (c == KEY_BACKSPACE))
    {
      if(self->cursor_pos > 0)
      {
        for(n=self->cursor_pos; n <= self->cmd_pos; n++) self->command[n-1] = self->command[n];
        --self->cmd_pos;
        --self->cursor_pos;
        term_cursor_left(1);
        xfs_local_print(&self->command[self->cursor_pos]);
        term_erase_to_end();
        term_cursor_left(self->cmd_pos - self->cursor_pos);
      }
    }
    else if ((c == KEY_NEWLINE) || (c == KEY_RETURN))
    {
      xfs_local_print("\r\n");
      cli_history_update(self);
      self->command[self->cmd_pos]=0;
      self->cmd_len = self->cmd_pos;
      self->cmd_pos = 0;
      self->cursor_pos = 0;
      return self->cmd_len;       /* command is ready for processing */
    }
    else if (c == KEY_CTRL_K)     /* Ctrl-K : clear to end of line */
    {
      self->command[self->cursor_pos] = 0;
      self->cmd_pos = self->cursor_pos;
      term_erase_to_end();
    }
    else if (c == KEY_CTRL_L)     /* Ctrl-L : clear screen and refresh prompt */
    {
      term_erase_screen();
      term_cursor_home();
      term_prompt(self, TERM_CLEAR_SCREEN);
    }
    else if (c == KEY_CTRL_G)     /* Ctrl-G : clear line and refresh prompt */
    {
      term_erase_line();
      term_cursor_return();
      term_prompt(self, TERM_CLEAR_LINE);
    }

    else if (c == KEY_CTRL_U)     /* Ctrl-U : clear to start of line */
    {
      if(self->cursor_pos > 0)
      {
        for(n=self->cursor_pos; n <= self->cmd_pos; n++) self->command[n-self->cursor_pos] = self->command[n];
        term_cursor_left(self->cursor_pos);
        term_erase_to_end();
        xfs_local_print(self->command);
        self->cmd_pos -= self->cursor_pos;
        self->cursor_pos = 0;
        term_cursor_left(self->cmd_pos);
      }
    }
    else if (char_is_printable(c))
    {
      if (self->cmd_pos < CLI_MAX_CMD_LEN)
      {
        for(n=self->cmd_pos+1; n > self->cursor_pos; n--) self->command[n] = self->command[n-1];
        self->command[self->cursor_pos] = c;
        self->command[++self->cmd_pos] = 0;
        xfs_local_print(&self->command[self->cursor_pos++]);
        term_cursor_left(self->cmd_pos - self->cursor_pos);
      }
    }
    c = KEY_CHAR_CONSUMED; /* new character consumed by CmdProcessor */
  }
  self->cli_active = 1;
  return c;  /* return negative return value from term_read_key */
}


/******************************************************************************/

