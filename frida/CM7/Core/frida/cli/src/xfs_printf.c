/******************************************************************************/
/*                                                                            */
/* file: xfs_printf.c                                                         */
/*                                                                            */
/* (PSI) tg32, se32                                                           */
/*                                                                            */
/* Grealy diverged from Public Domain version of printf :                     */
/*                                                                            */
/* Rud Merriam, Compsult, Inc. Houston, Tx.                                   */
/* For Embedded Systems Programming, 1991                                     */
/*                                                                            */
/*                                                                            */
/* The purpose of the routines is to output data as known                     */
/* from the standard printf function without the                              */
/* overhead most run-time libraries involve. Usually                          */
/* printf brings in many kilobytes of code and                                */
/* that is unacceptable in most embedded systems.                             */
/*                                                                            */
/* The routines operate like a printf / snprintf but not all of the           */
/* standard C format controls are supported. The ones provided are            */
/* primarily those needed for embedded systems to work.                       */
/*                                                                            */
/* The floaing point formats are omitted.                                     */
/*                                                                            */
/* (tg32) : Nonstandard format %b for binary output added                     */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* supported printf conversion syntax :                                       */
/*                                                                            */
/* %[flags][width][.precision][length_modifier]conversion_specifier           */
/*                                                                            */
/* flag                 : +, -, 0, #, ' ' (space)                             */
/* width                : <int_literal>, *                                    */
/* precision            : <int_literal>, *                                    */
/* length_modifier      : l                                                   */
/* conversion_specifier : i, d, u, p, X, x, o, b, c, s                        */
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/

#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "xfs_printf.h"
#include "cmd_proc_interface.h"

/******************************************************************************/
/* local constant definitions                                                 */
/******************************************************************************/

#define FLAG_ZERO      (1U <<  0)
#define FLAG_LEFT      (1U <<  1)
#define FLAG_PLUS      (1U <<  2)
#define FLAG_SPACE     (1U <<  3)
#define FLAG_HASH      (1U <<  4)
#define FLAG_UPPER     (1U <<  5)
#define FLAG_PREC      (1U <<  6)
#define FLAG_SIGNED    (1U <<  7)
#define FLAG_CHAR      (1U <<  8)
#define FLAG_SHORT     (1U <<  9)
#define FLAG_LONG      (1U << 10)
#define FLAG_LONG_LONG (1U << 11)


/******************************************************************************/
/* local type definitions                                                     */
/******************************************************************************/

/* Use parameter passing structure to make xfs_printf re-entrant */
typedef struct
{
  unsigned int flags;
  int  width;
  int  precision;
  int  arg_str_len;
  int  total_len;
  xfs_printf_char_callback_type char_callback_func;
  void *char_callback_opt;
} xfs_printf_conv_type;


/******************************************************************************/

typedef struct
{
  char *buffer;
  int  buffer_size;
  int  buffer_pos;
} xfs_buffer_char_opt_type;


/******************************************************************************/
/* local function declaration                                                 */
/******************************************************************************/

void xfs_print_char(char c, void *opt_ptr);
void xfs_buffer_char(char byte, void *opt_ptr);


/******************************************************************************/
/* global vars                                                                */
/******************************************************************************/

static xfs_printf_char_callback_type    xfs_local_printf_char_callback = xfs_print_char;
static void *xfs_local_printf_opt_ptr  = (void*) 0;

static xfs_printf_char_callback_type    xfs_printf_char_callback = xfs_print_char;
static xfs_printf_string_callback_type  xfs_printf_line_callback = 0;
static xfs_printf_string_callback_type  xfs_printf_call_callback = 0;
static void *xfs_printf_custom_opt_ptr = (void*) 0;

static char xfs_printf_global_buffer[XFS_PRINTF_GLOBAL_BUFFER_SIZE];
static int  xfs_printf_global_buffer_pos = 0;


/******************************************************************************/
/*                                                                            */
/* generate padding characters                                                */
/*                                                                            */
/******************************************************************************/

static void xfs_write_padding(xfs_printf_conv_type *conv_ptr)
{
  int i;

  for (i=conv_ptr->arg_str_len; i<conv_ptr->width; i++)
  {
    conv_ptr->char_callback_func(' ', conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }
}


/******************************************************************************/
/*                                                                            */
/* write a string to the output buffer as directed by the padding and         */
/* positioning flags.                                                         */
/*                                                                            */
/******************************************************************************/

static void xfs_write_string(char *lp, xfs_printf_conv_type *conv_ptr)
{
  int len;

  if (conv_ptr->flags & FLAG_PREC) len = strlen (lp) < conv_ptr->precision ? strlen (lp) : conv_ptr->precision;
  else len = strlen(lp);
  conv_ptr->arg_str_len = len;

  /* pad on left if needed */
  if (!(conv_ptr->flags & FLAG_LEFT)) xfs_write_padding(conv_ptr);

  /* move string to the buffer */
  while (len-- > 0)
  {
    conv_ptr->char_callback_func(*lp++, conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }

  /* pad on right if needed */
  if (conv_ptr->flags & FLAG_LEFT) xfs_write_padding(conv_ptr);
}


/******************************************************************************/
/*                                                                            */
/*  wrtie a number to the output buffer as directed by the padding            */
/*  and positioning flags.                                                    */
/*                                                                            */
/******************************************************************************/

static void xfs_write_num(const long n, const long radix, xfs_printf_conv_type *conv_ptr)
{
  const char digits[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
  char tmpbuf[64];
  unsigned long num = n;
  char sign = 0;
  char prepend_str[2] = {0,0};
  int prepend_chars = 0;
  int num_chars     = 0;
  int leading_zeros = 0;
  int d;

  if (radix == 10)
  {
    if ((conv_ptr->flags & FLAG_SIGNED) && n < 0L)   /* check if number is negative */
    {
      sign = '-';
      num  = -(n);
    }
    else if (conv_ptr->flags & FLAG_PLUS)
    {
      sign = '+';
    }
    else if (conv_ptr->flags & FLAG_SPACE)
    {
      sign = ' ';
    }
  }

  if (conv_ptr->flags & FLAG_HASH)    /* check for alternativ format */
  {
    if (radix == 8)
    {
      prepend_str[0] = '0';
      prepend_chars  = 1;
    }
    else if (radix == 16)
    {
      prepend_str[1] = '0';
      prepend_str[0] = (conv_ptr->flags & FLAG_UPPER) ? 'X' : 'x';
      prepend_chars  = 2;
    }
  }

  /* Build number (backwards) in tmpbuf */
  if (num || !(conv_ptr->flags & FLAG_PREC)) do
  {
    d = num % radix;
    tmpbuf[num_chars++] = (conv_ptr->flags & FLAG_UPPER) ? toupper(digits[d]) : digits[d];
  } while ((num /= radix) > 0);


  if (conv_ptr->flags & FLAG_PREC)
  {
    leading_zeros = conv_ptr->precision - num_chars;
  }
  else if (!(conv_ptr->flags & FLAG_LEFT) && (conv_ptr->flags & FLAG_ZERO) )
  {
    leading_zeros = conv_ptr->width - (sign ? 1 : 0) - prepend_chars - num_chars;
  }

  conv_ptr->arg_str_len = (sign ? 1 : 0) + (leading_zeros > 0 ? leading_zeros : 0) + prepend_chars + num_chars;

  /* padding in front of number, if needed */
  if (!(conv_ptr->flags & FLAG_LEFT)) xfs_write_padding(conv_ptr);

  if (sign)
  {
    conv_ptr->char_callback_func(sign, conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }

  /* prepend string for alternate format (# hash tag) */
  while (prepend_chars-- > 0)
  {
    conv_ptr->char_callback_func(prepend_str[prepend_chars], conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }

  while (leading_zeros-- > 0)
  {
    conv_ptr->char_callback_func('0', conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }

  /* use the converted number from temporary buffer */
  while (num_chars-- > 0)
  {
    conv_ptr->char_callback_func(tmpbuf[num_chars], conv_ptr->char_callback_opt);
    conv_ptr->total_len++;
  }

  /* add the padding if needed.   */
  if (conv_ptr->flags & FLAG_LEFT) xfs_write_padding(conv_ptr);
}


/******************************************************************************/
/*                                                                            */
/*  get a decimal number from the format string and update the line pointer   */
/*                                                                            */
/******************************************************************************/

static int xfs_get_num(const char **linep)
{
  int n = 0;

  while (isdigit((int)(**linep))) n = n*10 + *(*linep)++ - '0';
  (*linep)--;

  return(n);
}


/******************************************************************************/
/*                                                                            */
/*  do format string conversion with given arguments                          */
/*                                                                            */
/******************************************************************************/

int xfs_convert_format(xfs_printf_char_callback_type char_callback_func, void *char_callback_opt, const char *format, va_list argp)
{
  const char *cp;
  long num;
  char single_char_string [] = "c";
  xfs_printf_conv_type conv;
  xfs_printf_conv_type *conv_ptr = &conv;

  conv_ptr->char_callback_func = char_callback_func;
  conv_ptr->char_callback_opt  = char_callback_opt;
  conv_ptr->total_len      = 0;

  for (cp = format; *cp; cp++)
  {
    if (*cp != '%')
    {
      /* move format string chars to buffer until a format control is found. */
      conv_ptr->char_callback_func(*cp, conv_ptr->char_callback_opt);
      conv_ptr->total_len++;
    }
    else
    {
      /* initialize all the format specifier for this format. */
      conv_ptr->flags     = 0;
      conv_ptr->width     = 0;
      conv_ptr->precision = 0;

      for (cp++; *cp; cp++)  /* loop until end of format control */
      {
        if (isdigit(*cp) || (*cp=='*'))
        {
          if (conv_ptr->flags & FLAG_PREC)
          {
            conv_ptr->precision = (*cp=='*') ? va_arg(argp, int) : xfs_get_num(&cp);
            if (conv_ptr->precision < 0) conv_ptr->precision = 0;
          }
          else
          {
            if (*cp == '0') conv_ptr->flags |= FLAG_ZERO;
            conv_ptr->width = (*cp=='*') ? va_arg(argp, int) : xfs_get_num(&cp);
            if (conv_ptr->width < 0)
            {
              conv_ptr->flags |= FLAG_LEFT;
              conv_ptr->width = -conv_ptr->width;
            }
          }
          continue;
        }

        switch (*cp)
        {
          case '-':
              conv_ptr->flags |= FLAG_LEFT;
              continue;

          case '+':
              conv_ptr->flags |= FLAG_PLUS ;
              continue;

          case '#':
              conv_ptr->flags |= FLAG_HASH ;
              continue;

          case ' ':
              conv_ptr->flags |= FLAG_SPACE;
              continue;

          case '.':
              conv_ptr->flags |= FLAG_PREC ;
              continue;

          case 'l':
              if (conv_ptr->flags & FLAG_LONG)
              {
                conv_ptr->flags |= FLAG_LONG_LONG;
              }
              conv_ptr->flags |= FLAG_LONG;
              continue;

          case 'h':
              if (conv_ptr->flags & FLAG_SHORT)
              {
                conv_ptr->flags |= FLAG_CHAR;
              }
              conv_ptr->flags |= FLAG_SHORT;
              continue;

          case '%':
              conv_ptr->char_callback_func('%', conv_ptr->char_callback_opt);
              conv_ptr->total_len++;
              break;

          case 'i':
          case 'd':
              conv_ptr->flags |= FLAG_SIGNED;
              num = (conv_ptr->flags & FLAG_LONG) ? va_arg(argp, long) : va_arg(argp, int);
              xfs_write_num(num, 10L, conv_ptr);
              break;

          case 'u':
              conv_ptr->flags &= ~(FLAG_PLUS | FLAG_SPACE);
              num = (conv_ptr->flags & FLAG_LONG) ? va_arg(argp, unsigned long) : va_arg(argp, unsigned int);
              xfs_write_num(num, 10L, conv_ptr);
              break;

          case 'p':
              if (!conv_ptr->width && !(conv_ptr->flags & FLAG_PREC))
              {
                conv_ptr->flags |= (FLAG_HASH | FLAG_PREC);
                conv_ptr->precision = 2 * sizeof(void*);
              }
              conv_ptr->flags |= FLAG_LONG;
              num = (unsigned long) va_arg(argp, void *);
              xfs_write_num(num, 16L, conv_ptr);
              break;

          case 'X':
              conv_ptr->flags |= FLAG_UPPER;
          case 'x':
              num = (conv_ptr->flags & FLAG_LONG) ? va_arg(argp, unsigned long) : va_arg(argp, unsigned int);
              xfs_write_num(num, 16L, conv_ptr);
              break;

          case 'o':
              num = (conv_ptr->flags & FLAG_LONG) ? va_arg(argp, unsigned long) : va_arg(argp, unsigned int);
              xfs_write_num(num, 8L, conv_ptr);
              break;

          case 'b':
              num = (conv_ptr->flags & FLAG_LONG) ? va_arg(argp, unsigned long) : va_arg(argp, unsigned int);
              xfs_write_num(num, 2L, conv_ptr);
              break;

          case 's':
              xfs_write_string(va_arg(argp, char*), conv_ptr);
              break;

          case 'c':
              single_char_string[0] = va_arg(argp, int);
              xfs_write_string(single_char_string, conv_ptr);
              break;

          default:
              break;
        }
        break; /* leave conversion control loop */
      }
    }

  }

  return (conv_ptr->total_len);
}


/******************************************************************************/
/*                                                                            */
/* print to a buffer, function called with a given va_list                    */
/*                                                                            */
/******************************************************************************/

int xfs_vsnprintf(char *buffer, int buffer_size, const char *format, va_list argp)
{
  int len;
  xfs_buffer_char_opt_type buffer_char_opt;

  buffer_char_opt.buffer      = buffer;
  buffer_char_opt.buffer_size = buffer_size;
  buffer_char_opt.buffer_pos  = 0;

  len = xfs_convert_format(xfs_buffer_char, &buffer_char_opt, format, argp);

  if (buffer && buffer_size > 0) buffer[buffer_char_opt.buffer_pos] = 0;

  return len;
}


/******************************************************************************/
/*                                                                            */
/* print to a buffer, function called with variable number of arguments       */
/*                                                                            */
/******************************************************************************/

int xfs_snprintf(char *buffer, int buffer_size, const char *format, ...)
{
  int len;
  va_list argp;

  va_start(argp, format);
  len = xfs_vsnprintf(buffer, buffer_size, format, argp);
  va_end(argp);

  return len;
}


/******************************************************************************/
/*                                                                            */
/* default callback for printing single character                             */
/*                                                                            */
/******************************************************************************/

void xfs_print_char(char c, void *opt_ptr)
{
  cmd_proc_interface_putchar(c);
}


/******************************************************************************/
/*                                                                            */
/* buffer characters into opt buffer                                          */
/*                                                                            */
/******************************************************************************/

void xfs_buffer_char(char byte, void *opt_ptr)
{
  xfs_buffer_char_opt_type *opt = (xfs_buffer_char_opt_type*) opt_ptr;

  if (opt->buffer && opt->buffer_pos < opt->buffer_size-1)
  {
    opt->buffer[opt->buffer_pos++] = byte;
  }
}


/******************************************************************************/
/*                                                                            */
/* buffer characters and call function at end of line or when buffer is full  */
/*                                                                            */
/******************************************************************************/

void xfs_line_buffer_char(char c, void *opt_ptr)
{
  if (xfs_printf_global_buffer_pos < XFS_PRINTF_GLOBAL_BUFFER_SIZE-1)
  {
    xfs_printf_global_buffer[xfs_printf_global_buffer_pos++] = c;
  }

  if ((c == '\n') || (xfs_printf_global_buffer_pos == XFS_PRINTF_GLOBAL_BUFFER_SIZE-1))
  {
    xfs_printf_global_buffer[xfs_printf_global_buffer_pos] = 0;
    if (xfs_printf_line_callback)  xfs_printf_line_callback(xfs_printf_global_buffer, xfs_printf_global_buffer_pos, xfs_printf_custom_opt_ptr);
    xfs_printf_global_buffer_pos = 0;
  }
}


/******************************************************************************/
/*                                                                            */
/* set callback per char for xfs_local_printf                                 */
/*                                                                            */
/******************************************************************************/

void xfs_set_local_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr)
{
  xfs_local_printf_char_callback  = func ? func : xfs_print_char;
  xfs_local_printf_opt_ptr        = opt_ptr;
}


/******************************************************************************/
/*                                                                            */
/* set callback per char for xfs_printf                                       */
/*                                                                            */
/******************************************************************************/

void xfs_set_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr)
{
  /* callback per char */
  xfs_printf_char_callback  = func ? func : xfs_print_char;
  xfs_printf_line_callback  = 0;
  xfs_printf_call_callback  = 0;
  xfs_printf_custom_opt_ptr = opt_ptr;
}


/******************************************************************************/
/*                                                                            */
/* set callback per line or when buffer is full for xfs_printf                */
/*                                                                            */
/******************************************************************************/

void xfs_set_printf_line_callback(xfs_printf_string_callback_type func, void *opt_ptr)
{
  xfs_printf_char_callback  = xfs_line_buffer_char;
  xfs_printf_line_callback  = func;
  xfs_printf_call_callback  = 0;
  xfs_printf_custom_opt_ptr = opt_ptr;
}


/******************************************************************************/
/*                                                                            */
/* set callback at end of xfs_printf call                                     */
/*                                                                            */
/******************************************************************************/

void xfs_set_printf_call_callback(xfs_printf_string_callback_type func, void *opt_ptr)
{
  xfs_printf_char_callback  = xfs_buffer_char;
  xfs_printf_line_callback  = 0;
  xfs_printf_call_callback  = func;
  xfs_printf_custom_opt_ptr = opt_ptr;
}


/******************************************************************************/
/*                                                                            */
/* flush line buffer                                                          */
/*                                                                            */
/******************************************************************************/

void xfs_flush_line_buffer(void)
{
  xfs_printf_global_buffer[xfs_printf_global_buffer_pos] = 0;
  if (xfs_printf_line_callback)
  {
    xfs_printf_line_callback(xfs_printf_global_buffer, xfs_printf_global_buffer_pos, xfs_printf_custom_opt_ptr);
  }
  xfs_printf_global_buffer_pos = 0;
}


/******************************************************************************/
/*                                                                            */
/* print to stdout, function called with a string constant                    */
/*                                                                            */
/******************************************************************************/

void xfs_local_print(const char *s)
{
  while (*s) xfs_local_printf_char_callback(*s++, xfs_local_printf_opt_ptr);
}


/******************************************************************************/
/*                                                                            */
/* print to stdout, function called with variable number of arguments         */
/*                                                                            */
/******************************************************************************/

int xfs_local_printf(const char *format, ...)
{
  int len;
  va_list argp;

  va_start(argp, format);
  len = xfs_convert_format(xfs_local_printf_char_callback, xfs_local_printf_opt_ptr, format, argp);
  va_end(argp);

  return len;
}


/******************************************************************************/
/*                                                                            */
/* print string constant                                                      */
/*                                                                            */
/******************************************************************************/

void xfs_print(const char *s)
{

  if (xfs_printf_line_callback)
  {
    for (; *s; s++)
    {
      if (xfs_printf_global_buffer_pos < XFS_PRINTF_GLOBAL_BUFFER_SIZE-1)
      {
        xfs_printf_global_buffer[xfs_printf_global_buffer_pos++] = *s;
      }

      if ((*s == '\n') || (xfs_printf_global_buffer_pos == XFS_PRINTF_GLOBAL_BUFFER_SIZE-1))
      {
        xfs_printf_global_buffer[xfs_printf_global_buffer_pos] = 0;
        xfs_printf_line_callback(xfs_printf_global_buffer, xfs_printf_global_buffer_pos, xfs_printf_custom_opt_ptr);
        xfs_printf_global_buffer_pos = 0;
      }
    }
  }
  else if (xfs_printf_call_callback)
  {
    xfs_printf_call_callback(s, strlen(s), xfs_printf_custom_opt_ptr);
  }
  else if (xfs_printf_char_callback)
  {
    while (*s) xfs_printf_char_callback(*s++, xfs_printf_custom_opt_ptr);
  }

}


/******************************************************************************/
/*                                                                            */
/* print with specified format conversion                                     */
/*                                                                            */
/******************************************************************************/

int xfs_printf(const char *format, ...)
{
  int len;
  char local_buffer[XFS_PRINTF_BUFFER_SIZE];
  int  local_buffer_len = 0;
  xfs_buffer_char_opt_type buffer_char_opt;
  xfs_buffer_char_opt_type *callback_char_opt;
  va_list argp;


  if (xfs_printf_line_callback)
  {
    callback_char_opt = NULL;
  }
  else if (xfs_printf_call_callback)
  {
    buffer_char_opt.buffer      = local_buffer;
    buffer_char_opt.buffer_size = XFS_PRINTF_BUFFER_SIZE;
    buffer_char_opt.buffer_pos  = 0;
    callback_char_opt = &buffer_char_opt;
  }
  else
  {
    callback_char_opt = (xfs_buffer_char_opt_type*)xfs_printf_custom_opt_ptr;
  }

  va_start(argp, format);
  len = xfs_convert_format(xfs_printf_char_callback, callback_char_opt, format, argp);
  va_end(argp);

  if (xfs_printf_call_callback)
  {
    local_buffer_len = buffer_char_opt.buffer_pos;
    local_buffer[local_buffer_len] = 0;
    xfs_printf_call_callback(local_buffer, local_buffer_len, xfs_printf_custom_opt_ptr);
  }

  return len;
}


/******************************************************************************/
/******************************************************************************/
