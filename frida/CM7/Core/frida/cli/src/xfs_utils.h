/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Authr  :  schmid_e, tg32
 *  Created :  02.05.2014 13:24:35
 *
 *  Description :  Utilities such as string operation functions.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

#ifndef __XFS_UTILS_H__
#define __XFS_UTILS_H__

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_types.h"

/******************************************************************************/
/* definitions                                                                */
/******************************************************************************/

#define XFS_SUCCESS          0L
#define XFS_FAILURE          1L

#define XFS_TRUE             1
#define XFS_FALSE            0


/******************************************************************************/
/* macro definitions                                                          */
/******************************************************************************/

#define fcp(dst,src)     ncpy((char *)&dst,(char *)&src,sizeof(src))
#define fcmp(cp1,cp2)    ncmp((char *)&cp1,(char *)&cp2,sizeof(cp2))
#define ncpy(dst,src,n)  ncpy_c((char *)(dst),(const char *)(src),(n))
#define btst(val,num) (((val)>>(num))&0x0001)


/* #define fstrcmp(cp1,cp2) ncmp((char *)(cp1),(char *)(cp2),(sizeof(cp2)-1)) */

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/
int xfs_atoi(const char *str);
unsigned int xfs_atoui(const char *str);
unsigned long xfs_atoul(const char *str);

void ncpy_c(char *cp1, const char *cp2, int len);
int  ncmp(const char *cp1, const char *cp2, int len);
int fstrpcmp(const char *cp1, const char *cp2);
int  fstrcmp(const char *cp1, const char *cp2);
int  fstricmp(const char *cp1, const char *cp2);
int  fstrcmpwc(const char *cp1, const char *cp2);
char *xfs_strchr(char * str, int character);
void print_frame(unsigned char* fbuff, int len);
int hex_val(char c);
int hatoi(const char *str);
int is_num(char c);
unsigned int packed_bcd_to_dec(unsigned int pbcd);
unsigned int dec_to_packed_bcd(unsigned int dec);

int skip_whitespace(const char *s, int pos, int reverse);

/******************************************************************************/
/* static inline functions                                                    */
/******************************************************************************/

#endif /* __XFS_UTILS_H__ */
