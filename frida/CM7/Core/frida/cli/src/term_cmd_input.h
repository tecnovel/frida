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

#ifndef CLI_MAX_CMD_LEN
/* buffer size will be (CLI_MAX_CMD_LEN + 1) for 0 termination */
#define CLI_MAX_CMD_LEN  127
#endif

#ifndef CLI_MAX_ESC_SEQ
#define CLI_MAX_ESC_SEQ   7
#endif

/* Maximum number of entries in the command history */
#ifndef CLI_MAX_HISTORY_SIZE
#define CLI_MAX_HISTORY_SIZE  10
#endif

#ifndef TERM_USE_FUNCTION_KEYS
#define TERM_USE_FUNCTION_KEYS 0
#endif

#ifndef TERM_USE_SEQ_NAME
#define TERM_USE_SEQ_NAME 0
#endif


/******************************************************************************/
/* Escape Sequence key codes                                                  */
/******************************************************************************/

enum KEY_CODE{
        KEY_NO_CHAR           =  -4,
        KEY_CHAR_CONSUMED     =  -3,
        KEY_HANDLING_ESC_SEQ  =  -2,
        KEY_UNKNOWN_ESC_SEQ   =  -1,
        KEY_NULL              =   0,
        KEY_CTRL_A            =   1,
        KEY_CTRL_B            =   2,
        KEY_CTRL_C            =   3,
        KEY_CTRL_D            =   4,
        KEY_CTRL_E            =   5,
        KEY_CTRL_F            =   6,
        KEY_CTRL_G            =   7,
        KEY_ALERT             =  KEY_CTRL_G,
        KEY_BELL              =  KEY_CTRL_G,
        KEY_BEL               =  KEY_CTRL_G,
        KEY_CTRL_H            =   8,
        KEY_CTRL_I            =   9,
        KEY_TAB               =  KEY_CTRL_I,
        KEY_HT                =  KEY_CTRL_I,
        KEY_CTRL_J            =  10,
        KEY_NEWLINE           =  KEY_CTRL_J,
        KEY_LINE_FEED         =  KEY_CTRL_J,
        KEY_LF                =  KEY_CTRL_J,
        KEY_CTRL_K            =  11,
        KEY_VERTICAL_TAB      =  KEY_CTRL_K,
        KEY_VT                =  KEY_CTRL_K,
        KEY_CTRL_L            =  12,
        KEY_FORM_FEED         =  KEY_CTRL_L,
        KEY_FF                =  KEY_CTRL_L,
        KEY_CTRL_M            =  13,
        KEY_ENTER             =  KEY_CTRL_M,
        KEY_RETURN            =  KEY_CTRL_M,
        KEY_CR                =  KEY_CTRL_M,
        KEY_CTRL_N            =  14,
        KEY_CTRL_O            =  15,
        KEY_CTRL_P            =  16,
        KEY_CTRL_Q            =  17,
        KEY_CTRL_R            =  18,
        KEY_CTRL_S            =  19,
        KEY_CTRL_T            =  20,
        KEY_CTRL_U            =  21,
        KEY_CTRL_V            =  22,
        KEY_CTRL_W            =  23,
        KEY_CTRL_X            =  24,
        KEY_CTRL_Y            =  25,
        KEY_CTRL_Z            =  26,
        KEY_ESC               =  27,
        KEY_BACKSPACE         = 127,

        KEY_UP                = 256,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_HOME,
        KEY_INS,
        KEY_DEL,
        KEY_BEGIN,               /* alternate function of keypad 5 */
        KEY_CLEAR = KEY_BEGIN,
        KEY_END,
        KEY_PGUP,
        KEY_PGDWN,

        KEY_SHIFT_UP,
        KEY_SHIFT_DOWN,
        KEY_SHIFT_RIGHT,
        KEY_SHIFT_LEFT,
        KEY_SHIFT_BEGIN,
        KEY_SHIFT_CLEAR = KEY_SHIFT_BEGIN,
        KEY_SHIFT_END,
        KEY_SHIFT_HOME,
        KEY_SHIFT_DEL,

        KEY_ALT_UP,
        KEY_ALT_DOWN,
        KEY_ALT_RIGHT,
        KEY_ALT_LEFT,
        KEY_ALT_BEGIN,
        KEY_ALT_CLEAR = KEY_ALT_BEGIN,
        KEY_ALT_END,
        KEY_ALT_HOME,
        KEY_ALT_PGUP,
        KEY_ALT_PGDOWN,

        KEY_CTRL_UP,
        KEY_CTRL_DOWN,
        KEY_CTRL_RIGHT,
        KEY_CTRL_LEFT,
        KEY_CTRL_BEGIN,
        KEY_CTRL_CLEAR = KEY_CTRL_BEGIN,
        KEY_CTRL_END,
        KEY_CTRL_HOME,
        KEY_CTRL_PGUP,
        KEY_CTRL_PGDOWN,

#if TERM_USE_FUNCTION_KEYS
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,

        KEY_SHIFT_F1,
        KEY_SHIFT_F2,
        KEY_SHIFT_F3,
        KEY_SHIFT_F4,
        KEY_SHIFT_F5,
        KEY_SHIFT_F6,
        KEY_SHIFT_F7,
        KEY_SHIFT_F8,
        KEY_SHIFT_F9,
        KEY_SHIFT_F10,
        KEY_SHIFT_F11,
        KEY_SHIFT_F12,

        KEY_ALT_F1,
        KEY_ALT_F2,
        KEY_ALT_F3,
        KEY_ALT_F4,
        KEY_ALT_F5,
        KEY_ALT_F6,
        KEY_ALT_F7,
        KEY_ALT_F8,
        KEY_ALT_F9,
        KEY_ALT_F10,
        KEY_ALT_F11,
        KEY_ALT_F12,

        KEY_CTRL_F1,
        KEY_CTRL_F2,
        KEY_CTRL_F3,
        KEY_CTRL_F4,
        KEY_CTRL_F5,
        KEY_CTRL_F6,
        KEY_CTRL_F7,
        KEY_CTRL_F8,
        KEY_CTRL_F9,
        KEY_CTRL_F10,
        KEY_CTRL_F11,
        KEY_CTRL_F12,

#endif

};


/******************************************************************************/
/* Actions for prompt refresh                                                 */
/******************************************************************************/



enum prompt_opt {
   TERM_NO_NEW_LINE   =  0,
   TERM_NEW_LINE      =  1,
   TERM_CLEAR_LINE    =  2,
   TERM_CLEAR_SCREEN  =  3
};


#define TERMRK_STD {0, 0, 0, ""}


#define ESC_SEQ                        "\x1b"

#define ESC_CURSOR_HOME                ESC_SEQ "[H"
#define ESC_CURSOR_HIDE                ESC_SEQ "[?25l"
#define ESC_CURSOR_SHOW                ESC_SEQ "[?25h"
#define ESC_CURSOR_BLINK               ESC_SEQ "[?12h"
#define ESC_CURSOR_NOBLINK             ESC_SEQ "[?12l"

// Request a Cursor Position response from the device inf format <ESC>[{ROW};{COLUMN}R
#define ESC_CURSOR_GET_POS             ESC_SEQ "[6n"

#define ESC_CURSOR_UP_LIT(n)           ESC_SEQ "["#n"A"
#define ESC_CURSOR_DOWN_LIT(n)         ESC_SEQ "["#n"B"
#define ESC_CURSOR_RIGHT_LIT(n)        ESC_SEQ "["#n"C"
#define ESC_CURSOR_LEFT_LIT(n)         ESC_SEQ "["#n"D"
#define ESC_CURSOR_LINE_DOWN_LIT(n)    ESC_SEQ "["#n"E"
#define ESC_CURSOR_LINE_UP_LIT(n)      ESC_SEQ "["#n"F"
#define ESC_CURSOR_MOVE_LIT(row, col)  ESC_SEQ "["#row";"#col"H"

#define ESC_CURSOR_UP_PRF(n)           ESC_SEQ "[%dA", (n)
#define ESC_CURSOR_DOWN_PRF(n)         ESC_SEQ "[%dB", (n)
#define ESC_CURSOR_RIGHT_PRF(n)        ESC_SEQ "[%dC", (n)
#define ESC_CURSOR_LEFT_PRF(n)         ESC_SEQ "[%dD", (n)
#define ESC_CURSOR_LINE_DOWN_PRF(n)    ESC_SEQ "[%dE", (n)
#define ESC_CURSOR_LINE_UP_PRF(n)      ESC_SEQ "[%dF", (n)
#define ESC_CURSOR_MOVE_PRF(row, col)  ESC_SEQ "[%d;%dH", (row), (col)

#define ESC_CURSOR_MOVE_REL_LIT(row, col)  ESC_MOVE_CURSOR_FORW_LIT(col) ESC_MOVE_CURSOR_DOWN_LIT(row)

#define ESC_SCREEN_CLEAR_TO_END        ESC_SEQ "[J"
#define ESC_SCREEN_CLEAR_TO_START      ESC_SEQ "[1J"
#define ESC_SCREEN_CLEAR               ESC_SEQ "[2J"

#define ESC_TERMINAL_RESET             ESC_SEQ "c"
#define ESC_TERMINAL_LINE_WRAP         ESC_SEQ "[7h"
#define ESC_TERMINAL_NO_LINE_WRAP      ESC_SEQ "[7l"

#define ESC_LINE_CLEAR_TO_END          ESC_SEQ "[K"
#define ESC_LINE_CLEAR_TO_START        ESC_SEQ "[1K"
#define ESC_LINE_CLEAR                 ESC_SEQ "[2K"


#define ESC_CURSOR_SAVE_POS            ESC_SEQ "[s"
#define ESC_CURSOR_RESTORE_POS         ESC_SEQ "[u"

#define ESC_CURSOR_SAVE_ATTR           ESC_SEQ "[7"
#define ESC_CURSOR_RESTORE_ATTR        ESC_SEQ "[8"

#define ESC_COLOR_SET_DEFAULT          ESC_SEQ "[39m"
#define ESC_COLOR_SET_PRF(color)       ESC_SEQ "[%dm", (color)
#define ESC_COLOR_FB_PRF(fg, bg)       ESC_SEQ "[%d;%dm", (fg), (bg)
#define ESC_COLOR_SFB_PRF(s, fg, bg)   ESC_SEQ "[%d;%d;%dm", (s), (fg), (bg)

#define ESC_TEXT_NORMAL                ESC_SEQ "[0m"
#define ESC_TEXT_BRIGHT                ESC_SEQ "[1m"
#define ESC_TEXT_DIM                   ESC_SEQ "[2m"
#define ESC_TEXT_UNDERSCORE            ESC_SEQ "[4m"
#define ESC_TEXT_BLINK                 ESC_SEQ "[5m"
#define ESC_TEXT_INVERSE               ESC_SEQ "[7m"
#define ESC_TEXT_HIDDEN                ESC_SEQ "[8m"

// Foreground Colours
#define ESC_COL_FG_BLACK       30
#define ESC_COL_FG_RED         31
#define ESC_COL_FG_GREEN       32
#define ESC_COL_FG_YELLOW      33
#define ESC_COL_FG_BLUE        34
#define ESC_COL_FG_MAGENTA     35
#define ESC_COL_FG_CYAN        36
#define ESC_COL_FG_WHITE       37
#define ESC_COL_FG_DEFAULT     39

// Bright Foreground Colours
#define ESC_COL_FG_BR_BLACK    90
#define ESC_COL_FG_BR_RED      91
#define ESC_COL_FG_BR_GREEN    92
#define ESC_COL_FG_BR_YELLOW   93
#define ESC_COL_FG_BR_BLUE     94
#define ESC_COL_FG_BR_MAGENTA  95
#define ESC_COL_FG_BR_CYAN     96
#define ESC_COL_FG_BR_WHITE    97

// Background Colours
#define ESC_COL_BG_BLACK       40
#define ESC_COL_BG_RED         41
#define ESC_COL_BG_GREEN       42
#define ESC_COL_BG_YELLOW      43
#define ESC_COL_BG_BLUE        44
#define ESC_COL_BG_MAGENTA     45
#define ESC_COL_BG_CYAN        46
#define ESC_COL_BG_WHITE       47
#define ESC_COL_BG_DEFAULT     49

// Bright Background Colours
#define ESC_COL_BG_BR_BLACK   100
#define ESC_COL_BG_BR_RED     101
#define ESC_COL_BG_BR_GREEN   102
#define ESC_COL_BG_BR_YELLOW  103
#define ESC_COL_BG_BR_BLUE    104
#define ESC_COL_BG_BR_MAGENTA 105
#define ESC_COL_BG_BR_CYAN    106
#define ESC_COL_BG_BR_WHITE   107



/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef char* (*term_func_getprompt) (void);

typedef struct {
  char* esc_seq;
  enum  KEY_CODE key;
#if TERM_USE_SEQ_NAME
  char* seq_name;
#endif
} keycode_t;


typedef struct
{
  xfs_term *term;
  unsigned int esc_ticks;
  int  esc_seq_ptr;
  char esc_seq_buf[CLI_MAX_ESC_SEQ+1];
#if TERM_USE_SEQ_NAME
  char* seq_name;
#endif
} term_readkey_type;


typedef struct
{
  unsigned int cmd_len;
  unsigned int cmd_pos;
  unsigned int cursor_pos;
  unsigned int prompt_len;
  int  cli_active;
  int  hist_wr_ptr;
  int  hist_rd_ptr;
  int  hist_entries;
  int  newline;
  xfs_term *term;
  term_readkey_type rdkey;
  term_func_getprompt fct_getprompt;
  char command[CLI_MAX_CMD_LEN+1];
  char history[CLI_MAX_HISTORY_SIZE][CLI_MAX_CMD_LEN+1];
} term_cmd_input_type;


typedef unsigned int  (*term_func_get_ticks)(void);

typedef struct
{
  term_func_get_ticks get_ticks;
  unsigned int timeout_ticks;
} tick_timeout_t;


/******************************************************************************/
/* external global var                                                        */
/******************************************************************************/

extern term_cmd_input_type* cli_current_interface;


/******************************************************************************/
/* inline functions                                                           */
/******************************************************************************/

static inline unsigned int term_get_ticks(void)      { extern tick_timeout_t term_timeout; return term_timeout.get_ticks(); }
static inline unsigned int term_timeout_ticks(void)  { extern tick_timeout_t term_timeout; return term_timeout.timeout_ticks; }

static inline void term_cursor_right(int n)   { if (n>0) xfs_local_printf(ESC_CURSOR_RIGHT_PRF(n)); }
static inline void term_cursor_left(int n)    { if (n>0) xfs_local_printf(ESC_CURSOR_LEFT_PRF(n)); }
static inline void term_cursor_home(void)     { xfs_local_print(ESC_CURSOR_HOME);  }
static inline void term_erase_to_end(void)    { xfs_local_print(ESC_LINE_CLEAR_TO_END);  }
static inline void term_erase_to_start(void)  { xfs_local_print(ESC_LINE_CLEAR_TO_START); }
static inline void term_erase_line(void)      { xfs_local_print(ESC_LINE_CLEAR); }
static inline void term_erase_screen(void)    { xfs_local_print(ESC_SCREEN_CLEAR); }
static inline void term_cursor_newline(void)  { xfs_local_print("\r\n");    }
static inline void term_cursor_return(void)   { xfs_local_print("\r");      }

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

int  term_read_key(term_readkey_type* self);
void term_cmd_input_init(term_cmd_input_type* self, xfs_term *term, term_func_getprompt fct_getprompt, enum prompt_opt newline);

/**
  *  \param  self pointer to CmdProcessor object
  *  \return enum KEY_CODE
  *            >0: number of characters of command which is ready for processing
  *            KEY_CHAR_CONSUMED:    new character consumed by CmdProcessor
  *            KEY_UNKNOWN_ESC_SEQ:  new character but ESC sequence is unknown
  *            KEY_HANDLING_ESC_SEQ: new character, handling ESC sequence
  *            KEY_NO_CHAR:          no new character
  */
int  term_cmd_input_get_cmd(term_cmd_input_type* self);
void term_hide_cli(term_cmd_input_type* self);

/**
 *
 * Note: get_ticks function MUST return full unsigned int range
 *       and overrun to 0 at maximum unsigned int value
 */
void term_set_timeout_ticks(term_func_get_ticks get_ticks, unsigned int timeout_ticks);

void  cli_clear_history(term_cmd_input_type* self);
void  cli_cmd_history_add(term_cmd_input_type* self, char* cmd);
char* cli_cmd_history_get(term_cmd_input_type* self, int past);
static inline int cli_cmd_history_entries(term_cmd_input_type* self) { return self->hist_entries; }


#endif /* __TERM_CMD_INPUT_H__ */
