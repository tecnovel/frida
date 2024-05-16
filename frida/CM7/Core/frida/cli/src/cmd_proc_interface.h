/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 */

#ifndef __CMD_PROC_INTERFACE_H__
#define __CMD_PROC_INTERFACE_H__

/******************************************************************************/
/* type definitions                                                           */
/******************************************************************************/

typedef void (*cmd_proc_interface_func_putchar)(int c);
typedef int (*cmd_proc_interface_func_getchar) (void);
typedef char* (*cmd_proc_interface_func_get_hostname) (void);
typedef void (*cmd_proc_interface_func_usleep)(unsigned long time);


/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/
void cmd_proc_interface_init(   cmd_proc_interface_func_putchar putchar_fct,
                                cmd_proc_interface_func_getchar getchar_fct,
                                cmd_proc_interface_func_get_hostname hostname_fct,
                                cmd_proc_interface_func_usleep usleep_fct);
void cmd_proc_interface_putchar(int c);
int cmd_proc_interface_getchar(void);
char* cmd_proc_interface_get_hostname(void);
void cmd_proc_interface_usleep(unsigned long time);

/******************************************************************************/

#endif /* __CMD_PROC_INTERFACE_H__ */
