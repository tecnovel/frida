/*
 * msc_disk.h
 *
 *  Created on: Jan 29, 2024
 *      Author: pique_n
 */

#ifndef INC_MSC_DISK_H_
#define INC_MSC_DISK_H_

#include "ff.h"

void fatfs_init(void);

extern FATFS fatfs;

#endif /* INC_MSC_DISK_H_ */
