/*
 * mongoose_custom.h
 *
 *  Created on: Apr 4, 2024
 *      Author: pique_n
 */
#pragma once

#ifndef INC_MONGOOSE_CUSTOM_H_
#define INC_MONGOOSE_CUSTOM_H_

// See https://mongoose.ws/documentation/#build-options
#define MG_ARCH MG_ARCH_NEWLIB

#define MG_ENABLE_TCPIP 1
#define MG_ENABLE_CUSTOM_MILLIS 1
#define MG_ENABLE_CUSTOM_RANDOM 1
#define MG_ENABLE_PACKED_FS 1
#define MG_IO_SIZE 512
#define MG_ENABLE_LINES 1



#endif /* INC_MONGOOSE_CUSTOM_H_ */
