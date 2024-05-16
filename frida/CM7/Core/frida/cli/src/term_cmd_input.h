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

#ifndef __TERM_CMD_INPUT_H__
#define __TERM_CMD_INPUT_H__

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_printf.h"

/******************************************************************************/
/* macro definitions                                                          */
/******************************************************************************/

#define MAX_CMD_LEN  99
#define MAX_ESC_SEQ   7

/* Maximum number of entries in the command history */
#define MAX_HISTORY_SIZE  10


/******************************************************************************/
/* Escape Sequence key codes                                                  */
/******************************************************************************/

#define ESC_SEQ_UP            "[A"
#define ESC_SEQ_DOWN          "[B"
#define ESC_SEQ_RIGHT         "[C"
#define ESC_SEQ_LEFT          "[D"

#define ESC_SEQ_CTRL_UP       "[1;5A"
#define ESC_SEQ_CTRL_UP_2     "OA"

#define ESC_SEQ_CTRL_DOWN     "[1;5B"
#define ESC_SEQ_CTRL_DOWN_2   "OB"

#define ESC_SEQ_CTRL_RIGHT    "[1;5C"
#define ESC_SEQ_CTRL_RIGHT_2  "OC"

#define ESC_SEQ_CTRL_LEFT     "[1;5D"
#define ESC_SEQ_CTRL_LEFT_2   "OD"



#define ESC_SEQ_SHIFT_UP      "[2;5A"
#define ESC_SEQ_SHIFT_DOWN    "[2;5B"
#define ESC_SEQ_SHIFT_RIGHT   "[2;5C"
#define ESC_SEQ_SHIFT_LEFT    "[2;5D"

#define ESC_SEQ_ALT_UP        "[3;5A"
#define ESC_SEQ_ALT_DOWN      "[3;5B"
#define ESC_SEQ_ALT_RIGHT     "[3;5C"
#define ESC_SEQ_ALT_LEFT      "[3;5D"

#define ESC_SEQ_HOME          "[H"
#define ESC_SEQ_HOME_2        "OH"
#define ESC_SEQ_HOME_3        "[1~"

#define ESC_SEQ_END           "[F"
#define ESC_SEQ_END_2         "OF"
#define ESC_SEQ_END_3         "[4~"

#define ESC_SEQ_INS           "[2~"
#define ESC_SEQ_DEL           "[3~"

#define ESC_SEQ_PGUP          "[5~"
#define ESC_SEQ_PGDWN         "[6~"


/******************************************************************************/
/* Actions for prompt refresh                                                 */
/******************************************************************************/

#define TERM_CLEAR_SCREEN     1
#define TERM_CLEAR_LINE       2
#define TERM_NEW_LINE         3


/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef struct
{
  unsigned int baseaddr;
  unsigned int cmd_len;
  unsigned int cmd_pos;
  unsigned int cursor_pos;
  unsigned int prompt_len;
  char command[MAX_CMD_LEN+1];
  char history[MAX_HISTORY_SIZE][MAX_CMD_LEN+1];
  char esc_seq_buf[MAX_ESC_SEQ+1];
  int  esc_seq_ptr;
  int  esc;
  int  cli_active;
  int  hist_wr_ptr;
  int  hist_rd_ptr;
  int  hist_entries;
} term_cmd_input_type;

/******************************************************************************/
/* inline functions                                                           */
/******************************************************************************/

static inline void term_cursor_right(int n)   { if (n>0) xfs_local_printf("\x1b[%dC", n); }
static inline void term_cursor_left(int n)    { if (n>0) xfs_local_printf("\x1b[%dD", n); }
static inline void term_cursor_home(void)     { xfs_local_print("\x1b[H");  }
static inline void term_cursor_return(void)   { xfs_local_print("\r");      }
static inline void term_cursor_newline(void)  { xfs_local_print("\r\n");    }
static inline void term_erase_to_end(void)    { xfs_local_print("\x1b[K");  }
static inline void term_erase_to_start(void)  { xfs_local_print("\x1b[1K"); }
static inline void term_erase_line(void)      { xfs_local_print("\x1b[2K"); }
static inline void term_erase_screen(void)    { xfs_local_print("\x1b[2J"); }

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

void term_cmd_input_init(term_cmd_input_type* self);
void term_cmd_input_destruct(term_cmd_input_type* self);
int  term_cmd_input_get_cmd(term_cmd_input_type* self);
void term_hide_cli(term_cmd_input_type* self);

char getkey(term_cmd_input_type* self);

#endif /* __TERM_CMD_INPUT_H__ */
