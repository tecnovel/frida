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

#ifndef __XFS_IO_H__
#define __XFS_IO_H__

/******************************************************************************/
/* include files                                                              */
/******************************************************************************/

#include "xfs_types.h"

/******************************************************************************/
/* definitions                                                                */
/******************************************************************************/

#ifdef LINUX_COMPILE
xfs_u32 io_remap(xfs_u32 phys_address);
#else
#define io_remap(phys_address)  ((xfs_u32)(phys_address))
#endif

/******************************************************************************/
/* macro definitions                                                          */
/******************************************************************************/

#ifdef PROC_PPC
  #if defined __GNUC__
    #define SYNCHRONIZE_IO() __asm__ volatile ("eieio")
  #elif defined __DCC__
    #define SYNCHRONIZE_IO() __asm volatile(" eieio")
  #endif
#else
  #define SYNCHRONIZE_IO()
#endif

/* #define fstrcmp(cp1,cp2) ncmp((char *)(cp1),(char *)(cp2),(sizeof(cp2)-1)) */

/******************************************************************************/
/* function prototypes                                                        */
/******************************************************************************/

/******************************************************************************/
/* static inline functions                                                    */
/******************************************************************************/

static inline xfs_u8 xfs_in8(xfs_u32 InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    xfs_u8 IoContents;
    SYNCHRONIZE_IO();
    IoContents = (*(volatile xfs_u8 *)(long)(InAddress));
    /*__asm__ volatile ("eieio; lbz %0,0(%1)":"=r" (IoContents):"b" */
    /*          (InAddress));                                       */
    return IoContents;
}

/******************************************************************************/

static inline xfs_u16 xfs_in16(xfs_u32 InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    xfs_u16 IoContents;
    SYNCHRONIZE_IO();
    IoContents = *((volatile xfs_u16 *)(long)(InAddress));
    /*__asm__ volatile ("eieio; lhz %0,0(%1)":"=r" (IoContents):"b" */
    /*          (InAddress));                                       */
    return IoContents;
}

/******************************************************************************/

static inline xfs_u32 xfs_in32(xfs_u32 InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    xfs_u32 IoContents;
    SYNCHRONIZE_IO();
    IoContents = *((volatile xfs_u32 *)(long)(InAddress));
    /*__asm__ volatile ("eieio; lwz %0,0(%1)":"=r" (IoContents):"b" */
    /*          (InAddress));                                       */
    return IoContents;
}

/******************************************************************************/

static inline void xfs_out8(xfs_u32 OutAddress, xfs_u8 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    *((volatile xfs_u8 *)(long)(OutAddress)) = Value;
    SYNCHRONIZE_IO();
    /*__asm__ volatile ("stb %0,0(%1); eieio"::"r" (Value), "b"(OutAddress)); */
}

/******************************************************************************/
static inline void xfs_out16(xfs_u32 OutAddress, xfs_u16 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    *((volatile xfs_u16 *)(long)(OutAddress)) = Value;
    SYNCHRONIZE_IO();
    /*__asm__ volatile ("sth %0,0(%1); eieio"::"r" (Value), "b"(OutAddress)); */
}

/******************************************************************************/

static inline void xfs_out32(xfs_u32 OutAddress, xfs_u32 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

    *((volatile xfs_u32 *)(long)(OutAddress)) = Value;
    SYNCHRONIZE_IO();
    /*__asm__ volatile ("stw %0,0(%1); eieio"::"r" (Value), "b"(OutAddress)); */
}

/******************************************************************************/

#endif /* __XFS_IO_H__ */
