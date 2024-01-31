/*
 * mongoose_custom.h
 *
 *  Created on: Jan 26, 2024
 *      Author: pique_n
 */

#ifndef INC_MONGOOSE_CUSTOM_H_
#define INC_MONGOOSE_CUSTOM_H_

#pragma once

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_NEWLIB
#define MG_OTA MG_OTA_FLASH
#define MG_DEVICE MG_DEVICE_STM32H7

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_PACKED_FS 1
#define MG_ENABLE_DRIVER_STM32H 1

#endif /* INC_MONGOOSE_CUSTOM_H_ */
