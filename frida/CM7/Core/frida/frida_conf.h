/*
 * frida_conf.h
 *
 *  Created on: May 16, 2024
 *      Author: kaestli_a
 */

//#ifndef FRIDA_FRIDA_CONF_H_
//#define FRIDA_FRIDA_CONF_H_

#include "usb/TinyUSB/tusb_option.h"



/*
 * webserver settings
 */
#define SLD		"test"
#define TLD		"psi"


#define FULL_DOMAIN		SLD "." TLD
#define LOCAL_DOMAIN	SLD ".local"


/******************************************************************
 * USB settings
 */
#define FRIDA_MCU		OPT_MCU_STM32H7
#define FRIDA_MODE 		OPT_MODE_HIGH_SPEED
#define FRIDA_PORT		1


/*
 * memory settings
 */

/* select memory driver */
#define USE_SD
//#define USE_FLASH

#define DRIVE_LABEL		"FRIDA"



#if defined(USE_SD)
	#define SD_SDMMC	hsd1
#elif defined(USE_FLASH)
	#define FLASH_SPI	hspi2
#endif

#if defined(USE_FLASH) && defined (USE_SD)
	#error "ERROR: Only use one memory driver!!!"
#endif

//#endif /* FRIDA_FRIDA_CONF_H_ */



