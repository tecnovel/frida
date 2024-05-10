/*-----------------------------------------------------------------------/
/  Low level disk interface modlue include file   (C)ChaN, 2014          /
/-----------------------------------------------------------------------*/

#ifndef _FLASH_DISKIO_DEFINED
#define _FLASH_DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif


#include "../../diskio.h"
#include "../ff_gen_drv.h"


///*---------------------------------------*/
///* Prototypes for disk control functions */

DSTATUS flash_initialize (BYTE pdrv);
DSTATUS flash_status (BYTE pdrv);
DRESULT flash_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT flash_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT flash_ioctl (BYTE pdrv, BYTE cmd, void* buff);

extern const Diskio_drvTypeDef  FLASH_Driver;

#ifdef __cplusplus
}
#endif

#endif
