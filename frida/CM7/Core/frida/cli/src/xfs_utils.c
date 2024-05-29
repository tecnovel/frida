/*-------------------------------------------------------------------------------------
 *  Paul Scherrer Institut
 *-------------------------------------------------------------------------------------
 *
 *  Project :  WaveDream2
 *
 *  Author  :  schmid_e, tg32
 *  Created :  02.05.2014 13:24:35
 *
 *  Description :  Utilities such as string operation functions.
 *
 *-------------------------------------------------------------------------------------
 *-------------------------------------------------------------------------------------
 */

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_utils.h"
#include <stdlib.h>
#include <ctype.h>
//#include "xfs_printf.h"
//#define IO_REMAP_INFO 0

int xfs_atoi(const char *str)
{
  if (str) return strtol(str, 0, 0);
  return 0;
}

/******************************************************************************/


int is_num(char c)
{
  return (c >= '0' &&  c<= '9');
}

/************************************************************/

unsigned int xfs_atoui(const char *str)
{
  if (str) return strtoul(str, 0, 0);
  return 0;
}

/************************************************************/

unsigned long xfs_atoul(const char *str)
{
  if (str) return strtoul(str, 0, 0);
  return 0;
}

/******************************************************************************/

#if 0
const char* pad(int l)
{
  static const char pad_str[] = "                                        ";
  const int max_pad = (sizeof(pad_str)-1);

  if (l>max_pad) l = max_pad;
  if (l<0) l = 0;

  return pad_str+max_pad-l;
}
#endif

/******************************************************************************/

void ncpy_c(char *cp1, const char *cp2, int len)
{
 int i;
 for (i=0; i<len; i++)
 {
   cp1[i] = cp2[i];
 }
}

/******************************************************************************/

int ncmp(const char *cp1, const char *cp2, int len)
{
 int i;

 if (!cp1 || !cp2) return 0;

 for (i=0; i<len; i++)
 {
   if (cp1[i] != cp2[i]) return 0;
 }

 return 1;
}

/******************************************************************************/

/*  cp1 must match beginning part of cp2 */
int fstrpcmp(const char *cp1, const char *cp2)
{
  int i=0;

  if (!cp1 || !cp2) return 0;
  while(1)
  {
     if(cp1[i] == 0) return i;        /* return  matched length */
     if(cp1[i] != cp2[i]) return 0;
     i++;
  }
}

/******************************************************************************/

/*  cp1 must match cp2 completely */
int fstrcmp(const char *cp1, const char *cp2)
{
  int i=0;

  if (!cp1 || !cp2) return 0;

  while(1)
  {
     if(cp1[i] != cp2[i]) return 0;
     if(cp1[i] == 0) return i;    /* return  matched length */
     i++;
  }
}

/******************************************************************************/

/* ignore case */
int fstricmp(const char *cp1, const char *cp2)
{
  int i=0;

  if (!cp1 || !cp2) return 0;
  while(1)
  {
     if (tolower((int)(cp1[i])) != tolower((int)(cp2[i]))) return 0;
     if (cp1[i] == 0) return i;     /* return  matched length */
     i++;
  }
}

/******************************************************************************/

/* wildcard compare
 * NOTE:  first * matches rest of string, e.g.
 *        abc*   matched everything starting with abc
 * WARNING:
 *        *xys   matches everything, same as * alone !!!
 */

int fstrcmpwc(const char *cp1, const char *cp2)
{
  int i=0;

  if (!cp1 || !cp2) return 0;

  while(1)
  {
     if((cp1[i] == '*') || (cp2[i] == '*')) return i;   /* return  matched length */
     if(cp1[i] != cp2[i]) return 0;
     if(cp1[i] == 0) return i;     /* return  matched length */
     i++;
  }
}

/******************************************************************************/

char *xfs_strchr(char * str, int character)
{
  while(*str)
  {
    if(*str == character)
    {
      return str;
    }
    str++;
  }
  return 0;
}

/******************************************************************************/

#if 0
void print_frame(unsigned char* fbuff, int len)
{
  int i;

  xfs_local_printf("\r\n----Frame of len : %d Byte\r\n", len);
  for (i=0; i<len; i++)
  {
    xfs_local_printf("%02x ",fbuff[i] );
    if ((i&0xf) == 0x7) xfs_local_printf(" ");
    if ((i&0xf) == 0xf) xfs_local_printf("\r\n");
  }
  xfs_local_printf("\r\n");
}
#endif

/******************************************************************************/

int hex_val(char c)
{
  if (c >= '0' &&  c <= '9')
  {
    return (c - '0');
  }
  else if (c >= 'a' &&  c <= 'f')
  {
    return (c - 'a' + 10);
  }
  else if (c >= 'A' &&  c <= 'F')
  {
    return (c - 'A' + 10);
  }
  else
  {
    return -1;
  }
}

/******************************************************************************/

int hatoi(const char *str)
{
  int val = 0;

  while(*str && (*str != ':'))
  {
    int digit = hex_val(*str++);
    if (digit<0) return 0;
    val = val*16 + digit;
  }
  return val;
}

/******************************************************************************/

unsigned int dec_to_packed_bcd(unsigned int dec)
{
  unsigned int pbcd = 0;
  unsigned int base = 1;

  while(dec > 0)
  {
    pbcd += base * (dec % 10);
    base <<= 4;
    dec  /= 10;
  }
  return pbcd;
}


/******************************************************************************/

unsigned int packed_bcd_to_dec(unsigned int pbcd)
{
  unsigned int dec  = 0;
  unsigned int base = 1;

  while(pbcd > 0)
  {
    unsigned int digit = (pbcd % 16);
    if (digit > 9) digit = 9;
    dec  += base * digit;
    base *= 10;
    pbcd >>= 4;
  }
  return dec;
}


/******************************************************************************/

int skip_whitespace(const char *s, int pos, int reverse)
{
  int inc = reverse ? -1 : 1;
  for (; (s[pos] && ((s[pos]==' ') || (s[pos]==0x09))); pos+=inc);
  return pos;
}


/******************************************************************************/
