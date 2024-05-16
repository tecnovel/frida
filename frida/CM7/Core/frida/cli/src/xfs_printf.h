/******************************************************************************/
/*                                                                            */
/*  file: xfs_printf.h                                                        */
/*                                                                            */
/*  (c) 2010 PSI se32, 2018 tg32                                              */
/*                                                                            */
/******************************************************************************/

#ifndef __XFS_PRINTF_H__
#define __XFS_PRINTF_H__

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

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef void (*xfs_printf_string_callback_type)(const char*, int, void*);
typedef void (*xfs_printf_char_callback_type)(char, void*);


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


/******************************************************************************/
/* set callback functions                                                     */
/******************************************************************************/

void xfs_set_local_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr);
void xfs_set_printf_char_callback(xfs_printf_char_callback_type func, void * opt_ptr);
void xfs_set_printf_line_callback(xfs_printf_string_callback_type func, void *opt_ptr);
void xfs_set_printf_call_callback(xfs_printf_string_callback_type func, void *opt_ptr);


#endif /* __XFS_PRINTF_H__ */
