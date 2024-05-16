/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, theidel
 *  Created :  02.05.2014 13:24:35
 *
 *  Description :  Capturing commands from terminal.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/



#include "xfs_utils.h"
#include "string.h"
#include "term_cmd_input.h"
#include <stdio.h>
#include "cmd_proc_interface.h"

#ifdef LINUX_COMPILE
#include <unistd.h>
#endif


#define DBG_HIST 0

/******************************************************************************/
/* function prototyps                                                         */
/******************************************************************************/

extern char* sys_get_hostname(void);
extern void sys_usleep(unsigned long time);

/******************************************************************************/
/* function definitions                                                       */
/******************************************************************************/




/******************************************************************************/

void term_cmd_input_init(term_cmd_input_type* self)
{
  self->cli_active   = 0;
  self->esc          = 0;
  self->esc_seq_ptr  = 0;
  self->cmd_pos      = 0;
  self->cmd_len      = 0;
  self->prompt_len   = 0;
  self->cursor_pos   = 0;
  self->hist_wr_ptr  = 0;
  self->hist_rd_ptr  = 0;
  self->hist_entries = 0;
  self->command[0]   = 0;
  for (int i = 0; i < MAX_HISTORY_SIZE; i++) {
	  self->history[i][0] = 0;
  }
}

/******************************************************************************/

#if DBG_HIST
void dbg_hist(term_cmd_input_type* self)
{
  int i;
  xfs_local_printf("self->cmd_pos  = %d\r\n",       self->cmd_pos );
  xfs_local_printf("self->cursor_pos  = %d\r\n",       self->cursor_pos );

  xfs_local_printf("\r\n" "self->hist_wr_ptr = %d\r\n",  self->hist_wr_ptr);
  xfs_local_printf("self->hist_rd_ptr = %d\r\n",       self->hist_rd_ptr);
  xfs_local_printf("self->hist_entries = %d\r\n",      self->hist_entries);
  for (i=0; i<self->hist_entries; i++)
  {
    xfs_local_printf("self->history[%d] = %s\r\n",  i, self->history[i]);
  }
}
#endif


/******************************************************************************/

void term_histo_select(term_cmd_input_type* self, int direction)
{
  int next_rd_ptr;

  if (direction < 0)
  {
    /* previous command */
    next_rd_ptr = self->hist_rd_ptr - 1;
    if (next_rd_ptr <  0)                 next_rd_ptr = self->hist_entries;
    if (next_rd_ptr >=  MAX_HISTORY_SIZE) next_rd_ptr = MAX_HISTORY_SIZE-1;
    if (next_rd_ptr == self->hist_wr_ptr) return;  /* already on oldest entry */
  }
  else if (self->hist_rd_ptr != self->hist_wr_ptr) /* if not current command  */
  {
    /* next command */
    next_rd_ptr = self->hist_rd_ptr + 1;
    if (next_rd_ptr > self->hist_entries) next_rd_ptr = 0;
    if (next_rd_ptr >=  MAX_HISTORY_SIZE) next_rd_ptr = 0;
  }
  else
  {
    return;
  }

  /* save current command to history */
  self->command[self->cmd_pos] = 0;
  strncpy(self->history[self->hist_rd_ptr], self->command, MAX_CMD_LEN);

  /* delete the terminal output */
  term_cursor_left(self->cursor_pos);   /* move cursor left to start */
  term_erase_to_end();

  /* use next read pointer */
  self->hist_rd_ptr = next_rd_ptr;

  /* copy the history command to the command buffer */
  strncpy(self->command, self->history[self->hist_rd_ptr], MAX_CMD_LEN);
  self->cmd_pos =  strlen (self->command) < MAX_CMD_LEN ? strlen (self->command) : MAX_CMD_LEN;
  self->cursor_pos = self->cmd_pos;

#if DBG_HIST
  dbg_hist(self);
#endif

  /* print the command buffer */
  xfs_local_print(self->command);
}


/******************************************************************************/

void term_histo_update(term_cmd_input_type* self)
{
  int matches_latest_hist_entry = 0;

  if(self->hist_entries > 0)
  {
    int tmp_wr_ptr = self->hist_wr_ptr - 1;
    if (tmp_wr_ptr <  0) tmp_wr_ptr = self->hist_entries-1;
    matches_latest_hist_entry = fstrcmp(self->history[tmp_wr_ptr], self->command);
  }

  if( (self->cmd_pos > 0) && !matches_latest_hist_entry )
  {
    /* if there is a command and it's not the same as the latest entry, add it to the history */
    strncpy(self->history[self->hist_wr_ptr], self->command, MAX_CMD_LEN);
    if (self->hist_entries < MAX_HISTORY_SIZE) ++self->hist_entries;
    if ((++self->hist_wr_ptr) >= MAX_HISTORY_SIZE) self->hist_wr_ptr = 0;
  }
  self->hist_rd_ptr = self->hist_wr_ptr;

#if DBG_HIST
  dbg_hist(self);
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

  self->prompt_len = xfs_local_printf("%s > ", cmd_proc_interface_get_hostname());
  xfs_local_printf("%s", self->command);
  term_cursor_left(self->cmd_pos - self->cursor_pos);
}


/******************************************************************************/

void term_hide_cli(term_cmd_input_type* self)
{
  self->cli_active = 0;
//  term_erase_line();
//  term_cursor_return();

  term_cursor_left(self->prompt_len + self->cursor_pos);
  term_erase_to_end();

}

/******************************************************************************/

int term_cmd_input_get_cmd(term_cmd_input_type* self)
{
  unsigned int n;
  int c;

  if (self->cmd_len > 0)
  {
    self->command[0] = 0;
    self->cmd_len    = 0;
  }

  if (self->cli_active == 0)
  {
    term_prompt(self, 0);  /* term_prompt(self, TERM_NEW_LINE); */
  }

  /* disable cli_active and set it when leaving function (if it is still active),                  */
  /* otherwise calls to xfs_[local_]print[f] during this function would hide command line again... */
  self->cli_active = 0;

  c = cmd_proc_interface_getchar();
  if (c != EOF)
  {
    //xfs_local_printf("c =   %c    (%d)   self->cmd_pos=%d \r\n",c,c,self->cmd_pos);

    if (c==0x01b)  /* escape */
    {
      self->esc = 200;
      self->esc_seq_ptr = 0;
    }
    else if (self->esc)
    {
      if (self->esc_seq_ptr < MAX_ESC_SEQ)
      {
        self->esc_seq_buf[self->esc_seq_ptr++] = c;
        self->esc_seq_buf[self->esc_seq_ptr  ] = 0;
      }

      /* End of Escape Sequence */
      if ( ((c >= 'A') && (c <='H')) || ((c >= 'P') && (c <='Z')) || (c=='~') || ((c >= 'a') && (c <='z')) )
      {
        /* xfs_local_printf("Escape Sequence : %s\r\n", self->esc_seq_buf); */
        self->esc = 0;

        if (fstrcmp(self->esc_seq_buf, ESC_SEQ_UP))                    /* Arrow-Up */
        {
          term_histo_select(self, -1);
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_DOWN))             /* Arrow-Down */
        {
          term_histo_select(self, +1);
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_RIGHT))            /* Arrow-Right */
        {
          if (self->cursor_pos < self->cmd_pos)
          {
            term_cursor_right(1);
            self->cursor_pos++;
          }
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_LEFT))             /* Arrow-Left */
        {
          if (self->cursor_pos > 0)
          {
            term_cursor_left(1);
            self->cursor_pos--;
          }
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_CTRL_RIGHT)   ||
                 fstrcmp(self->esc_seq_buf, ESC_SEQ_CTRL_RIGHT_2)  )   /* Ctrl-Arrow-Right */
        {
          n = skip_char_r(self->command, self->cmd_pos, self->cursor_pos, 1);
          n = skip_char_r(self->command, self->cmd_pos, n,          0);
          term_cursor_right(n - self->cursor_pos);
          self->cursor_pos = n;
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_CTRL_LEFT  ) ||
                 fstrcmp(self->esc_seq_buf, ESC_SEQ_CTRL_LEFT_2)  )    /* Ctrl-Arrow-Left */
        {
          n = skip_char_l(self->command, self->cmd_pos, self->cursor_pos, 1);
          n = skip_char_l(self->command, self->cmd_pos, n,          0);
          term_cursor_left(self->cursor_pos - n);
          self->cursor_pos = n;
        }

        else if ( fstrcmp(self->esc_seq_buf, ESC_SEQ_HOME)   ||
                  fstrcmp(self->esc_seq_buf, ESC_SEQ_HOME_2) ||
                  fstrcmp(self->esc_seq_buf, ESC_SEQ_HOME_3)  )        /* Home */
        {
          term_cursor_left(self->cursor_pos);
          self->cursor_pos= 0;
        }
        else if ( fstrcmp(self->esc_seq_buf, ESC_SEQ_END)   ||
                  fstrcmp(self->esc_seq_buf, ESC_SEQ_END_2) ||
                  fstrcmp(self->esc_seq_buf, ESC_SEQ_END_3)  )         /* End */
        {
          term_cursor_right(self->cmd_pos - self->cursor_pos);
          self->cursor_pos = self->cmd_pos;
        }
        else if (fstrcmp(self->esc_seq_buf, ESC_SEQ_DEL))              /* Delete */
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
      }
    }
    else if ((c==0x08) || (c==0x7f))                                   /* Backspace */
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
    else if ((c==0x0a) || (c==0x0d))                                   /* Enter */
    {
      xfs_local_print("\r\n");
      term_histo_update(self);
      self->command[self->cmd_pos]=0;
      self->cmd_len = self->cmd_pos;
      self->cmd_pos = 0;
      self->cursor_pos = 0;
   //   self->cli_active = 0;
      return self->cmd_len;
    }
    else if (c==0x0b)                                                  /* Ctrl-K : clear to end of line */
    {
      self->command[self->cursor_pos] = 0;
      self->cmd_pos = self->cursor_pos;
      term_erase_to_end();
    }
    else if (c==0x0c)                                                  /* Ctrl-L : clear screen and refresh prompt */
    {
      term_erase_screen();
      term_cursor_home();
      term_prompt(self, TERM_CLEAR_SCREEN);
    }
    else if (c==0x07)                                                  /* Ctrl-G : clear line and refresh prompt */
    {
      term_erase_line();
      term_cursor_return();
      term_prompt(self, TERM_CLEAR_LINE);
    }

    else if (c==0x15)                                                  /* Ctrl-U : clear to start of line */
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
    else if  ((c >= 32) && (c <= 126))                                 /* writable character */
    {
      if (self->cmd_pos < MAX_CMD_LEN)
      {
        for(n=self->cmd_pos+1; n > self->cursor_pos; n--) self->command[n] = self->command[n-1];
        self->command[self->cursor_pos] = c;
        self->command[++self->cmd_pos] = 0;
        xfs_local_print(&self->command[self->cursor_pos++]);
        term_cursor_left(self->cmd_pos - self->cursor_pos);
      }
    }
  }
  else if (self->esc)
  {
    self->esc--;
    if (!self->esc)
    {
      self->cli_active = 1;
      return -1;    /* abort ESC Sequence after timeout */
    }
    cmd_proc_interface_usleep(10);

  }

  self->cli_active = 1;
  return -1;
}


/******************************************************************************/
