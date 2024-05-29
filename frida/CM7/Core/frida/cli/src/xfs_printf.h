/******************************************************************************/
/*                                                                            */
/*  file: xfs_printf.h                                                        */
/*                                                                            */
/*  (c) 2010 PSI se32, 2018 tg32                                              */
/*                                                                            */
/******************************************************************************/

#ifndef __XFS_PRINTF_H__
#define __XFS_PRINTF_H__


#ifndef USE_STDIO
#define USE_STDIO (0)
#endif

#ifndef XFS_PRINT_CHAR_CONV_LF_TO_CRLF
#define XFS_PRINT_CHAR_CONV_LF_TO_CRLF (1)
#endif

#if USE_STDIO

#include <stdio.h>

#define xfs_vsnprintf vsnprintf
#define xfs_snprintf  snprintf

#define xfs_local_print(s) printf("%s",s)
#define xfs_local_printf   printf

#define xfs_print(s) printf("%s",s)
#define xfs_printf   printf

#define xfs_flush_line_buffer()

#else

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

#include <stdarg.h>


/******************************************************************************/
/* constant definitions                                                       */
/******************************************************************************/

#define XFS_PRINTF_BUFFER_SIZE        256
#define XFS_PRINTF_GLOBAL_BUFFER_SIZE 256

#define XFS_EOL "\r\n"
#define XFS_EOF (-1)

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef void (*xfs_printf_string_callback_type)(const char*, int, void*);
typedef void (*xfs_printf_char_callback_type)(char, void*);


typedef void  (*xfs_term_putchar)(int c);
typedef int   (*xfs_term_getchar)(void);

typedef struct
{
  xfs_term_putchar putchar;
  xfs_term_getchar getchar;
} xfs_term;


static inline char char_is_printable(char c) { return ((c>=32) && (c<=126)); }
static inline char printable_char(char c)    { return char_is_printable(c) ? c : ' '; }

/******************************************************************************/
/* main print functions                                                       */
/******************************************************************************/


int  xfs_vsnprintf(char *outbuf1, int max_buffer_size, const char *format, va_list argp);
int  xfs_snprintf(char *outbuf1, int max_buffer_size, const char *format, ...);

void xfs_local_print(const char *s);
int  xfs_local_printf(const char *format, ...);

void xfs_print(const char *s);
int  xfs_printf(const char *format, ...);

void xfs_flush_line_buffer(void);

void xfs_term_init(xfs_term* self, xfs_term_putchar fct_putchar, xfs_term_getchar fct_getchar);
void xfs_term_set_std(xfs_term* std);
int  xfs_getchar(void);
void xfs_putchar(int c);

/******************************************************************************/
/* set callback functions                                                     */
/******************************************************************************/

void xfs_set_local_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr);
void xfs_set_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr);
void xfs_set_printf_line_callback(xfs_printf_string_callback_type func, void *opt_ptr);
void xfs_set_printf_call_callback(xfs_printf_string_callback_type func, void *opt_ptr);


#endif /* USE_STDIO */

#endif /* __XFS_PRINTF_H__ */
