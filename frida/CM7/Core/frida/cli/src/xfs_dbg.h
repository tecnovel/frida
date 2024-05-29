/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, theidel
 *  Created :  31.07.2014 08:14:22
 *
 *  Description :  Debug level output control.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

#ifndef __XFS_DBG_H__
#define __XFS_DBG_H__


#include "xfs_printf.h"

/******************************************************************************/
/* definitions                                                                */
/******************************************************************************/

#define DBG_LEVEL_NONE     0
#define DBG_LEVEL_ERR      1
#define DBG_LEVEL_WARN     2
#define DBG_LEVEL_INIT     3
#define DBG_LEVEL_INF0     4
#define DBG_LEVEL_INF1     5
#define DBG_LEVEL_INF2     6
#define DBG_LEVEL_INF3     7
#define DBG_LEVEL_INF4     8
#define DBG_LEVEL_SPAM     9
#define DBG_LEVEL_ALL     10

#define DBG_LEVEL_MAX     DBG_LEVEL_ALL

/*
 *  NOTE:
 *  adjust dbg_level_str in dbg.c whenn changing debug levels
 */

extern const char *dbg_level_str[];

/* Error Flags: */
#define ERR_UNKNOWN_CMD      0x01
#define ERR_INVALID_ARGS     0x02
#define ERR_TOO_FEW_ARGS     0x03
#define ERR_WD2_HV           0x10
#define ERR_WD2_PLL_CONFIG   0x11

#define DBG_LEVEL_DEFAULT    DBG_LEVEL_INFO
#define DBG_LEVEL_INFO       DBG_LEVEL_INF0
#define DBG_INFO             DBG_INF0



#ifdef DBG_NONE /* may be set at compile time to reduce code size */

#define DBG_ERR   (0)
#define DBG_WARN  (0)
#define DBG_INIT  (0)
#define DBG_INF0  (0)
#define DBG_INF1  (0)
#define DBG_INF2  (0)
#define DBG_INF3  (0)
#define DBG_INF4  (0)
#define DBG_SPAM  (0)
#define DBG_ALL   (0)

#define DBG
#define DBG_MSG(s)
#define DBG_STR(s)
#define DBG_CHAR(c)
#define DBG_UINT(v)
#define DBG_INT(v)
#define DBG_UL(v)
#define DBG_HEX(v)

#else

#define DBG_ERR   (xfs_dbg_level >= DBG_LEVEL_ERR )
#define DBG_WARN  (xfs_dbg_level >= DBG_LEVEL_WARN)
#define DBG_INIT  (xfs_dbg_level >= DBG_LEVEL_INIT)
#define DBG_INF0  (xfs_dbg_level >= DBG_LEVEL_INF0)
#define DBG_INF1  (xfs_dbg_level >= DBG_LEVEL_INF1)
#define DBG_INF2  (xfs_dbg_level >= DBG_LEVEL_INF2)
#define DBG_INF3  (xfs_dbg_level >= DBG_LEVEL_INF3)
#define DBG_INF4  (xfs_dbg_level >= DBG_LEVEL_INF4)
#define DBG_SPAM  (xfs_dbg_level >= DBG_LEVEL_SPAM)
#define DBG_ALL   (xfs_dbg_level >= DBG_LEVEL_ALL )

#define DBG         if (DBG_INF0) xfs_printf("%s:%d DBG: func: %s\r\n",        __FILE__, __LINE__, __func__);
#define DBG_MSG(s)  if (DBG_INF0) xfs_printf("%s:%d DBG: %s\r\n",              __FILE__, __LINE__, (s)); // string litral
#define DBG_STR(s)  if (DBG_INF0) xfs_printf("%s:%d DBG: " #s " = %s\r\n",     __FILE__, __LINE__, (s));
#define DBG_CHAR(c) if (DBG_INF0) xfs_printf("%s:%d DBG: " #c " = 0x%02x (%d) : %c\r\n",  __FILE__, __LINE__, (c), (c), (((c)>=32)&&((c)<127))?(c):' ');
#define DBG_UINT(v) if (DBG_INF0) xfs_printf("%s:%d DBG: " #v " = %u\r\n",     __FILE__, __LINE__, (v));
#define DBG_INT(v)  if (DBG_INF0) xfs_printf("%s:%d DBG: " #v " = %d\r\n",     __FILE__, __LINE__, (v));
#define DBG_UL(v)   if (DBG_INF0) xfs_printf("%s:%d DBG: " #v " = %lu\r\n",    __FILE__, __LINE__, (v));
#define DBG_HEX(v)  if (DBG_INF0) xfs_printf("%s:%d DBG: " #v " = 0x%08x\r\n", __FILE__, __LINE__, (unsigned long long)(v));

#endif
// some shortcuts
#define DBGM   DBG_MSG
#define DBGS   DBG_STR
#define DBGC   DBG_CHAR
#define DBGU   DBG_UINT
#define DBGI   DBG_INT
#define DBGUL  DBG_UL
#define DBGX   DBG_HEX

/******************************************************************************/
/* external global var                                                        */
/******************************************************************************/

extern unsigned int xfs_dbg_level;

/*
  -- use as follows
  if (DBG_ERR)  xfs_printf("Error in xxxx\r\n");
*/

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

void set_error_flag(unsigned int error_flag);
void set_dbg_level(unsigned int dbg_level);
unsigned int get_dbg_level(void);
unsigned int parse_dbglvl(const char *str);

/******************************************************************************/

#endif /* __XFS_DBG_H__ */

/******************************************************************************/
