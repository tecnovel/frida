/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2017        */
/*                                                                       */
/*   Portions COPYRIGHT 2017 STMicroelectronics                          */
/*   Portions Copyright (C) 2017, ChaN, all right reserved               */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "frida_conf.h"
#ifdef USE_FLASH

#include "flash_diskio.h"
#include "w25qxx.h"
#include <string.h>



#if defined ( __GNUC__ )
#ifndef __weak
#define __weak __attribute__((weak))
#endif
#endif

#define BLOCK_SIZE		512

W25QXX_HandleTypeDef flash;

const Diskio_drvTypeDef  FLASH_Driver =
{
  flash_initialize,
  flash_status,
  flash_read,
  flash_write,
  flash_ioctl,
};

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS flash_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
  return RES_OK;
}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS flash_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	w25qxx_init(&flash, &FLASH_SPI, FLASH_CS_PORT, FLASH_CS_PIN);
	return RES_OK;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT flash_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{

  w25qxx_read(&flash, sector*BLOCK_SIZE, buff, count*BLOCK_SIZE);
  return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */

DRESULT flash_write (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count        	/* Number of sectors to write */
)
{
	uint8_t buf[4096];
	w25qxx_read(&flash, (sector/8)*8*BLOCK_SIZE, buf, 4096);
	w25qxx_erase(&flash, sector*BLOCK_SIZE, count*BLOCK_SIZE);
	memcpy(buf+(sector%8)*BLOCK_SIZE, buff, count*BLOCK_SIZE);
	w25qxx_write(&flash, (sector/8)*8*BLOCK_SIZE, buf, 4096);

//	w25qxx_erase(&flash, sector*BLOCK_SIZE, count*BLOCK_SIZE);
//	w25qxx_write(&flash, sector*BLOCK_SIZE, buf, count*BLOCK_SIZE);

//	uint8_t buf[count*BLOCK_SIZE];
//	w25qxx_read(&flash, sector*BLOCK_SIZE, (uint8_t*) buff, count*BLOCK_SIZE);
//	w25qxx_erase(&flash, sector*BLOCK_SIZE, count*BLOCK_SIZE);
//
//	w25qxx_write(&flash, sector*BLOCK_SIZE, (uint8_t*) buf, count*BLOCK_SIZE);
  return RES_OK;
}


/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */

DRESULT flash_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
  DRESULT res;

  switch (cmd){
  case GET_SECTOR_COUNT:
	  // 16 MB
	*(DWORD*) buff = 0x8000;
	  res = RES_OK;
	break;
  case GET_SECTOR_SIZE:
	*(WORD*) buff = BLOCK_SIZE;
	res = RES_OK;
	break;
  case CTRL_SYNC:
  case MMC_GET_CSD:
  case MMC_GET_CID:
  case MMC_GET_OCR:

	  res = RES_OK;

	break;
  default:
	res = RES_PARERR;
  }

  return res;
}


/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
__weak DWORD get_fattime (void)
{
  return 0;
}

#endif

