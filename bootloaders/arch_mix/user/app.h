/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 /**
 * The MIT License (MIT)
 * 
 * Author: Hongtai Liu (lht856@foxmail.com)
 * 
 * Copyright (C) 2019  Seeed Technology Co.,Ltd. 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _APP_H_
#define _APP_H_

//#define _DEBUG_ 1

/*${header:start}*/
#include "fsl_flexspi.h"
#include "fsl_cache.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#ifdef QSPI_FLASH
#define BOOTLOADER_FLEXSPI FLEXSPI
#define FLASH_SIZE 8*1024u
#define BOOTLOADER_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 256
#define BOOTLOADER_DATA_AREA 15
#define SECTOR_SIZE 0x1000

#define BOOTLOADER_FLEXSPI_CLOCK kCLOCK_FlexSpi

#define FLASH_START_ADDRESS 0x60010000
#define FLASH_MAX_LENGTH 0x60800000- FLASH_START_ADDRESS

#define APP_START_ADDRESS 0x60010000
#define BOOTLOADER_START_ADDRESS 0x60000000



/*static variable*/
#define BOOT_STATUS_ADDRESS           (0x6000F000 + 4*8)
#define BOOT_STATUS_DATA              (*((volatile uint32_t *) BOOT_STATUS_ADDRESS))
#define BOOT_STATUS_MAGIC							(0x424F4F54)
#else
#define BOOTLOADER_FLEXSPI FLEXSPI
#define FLASH_SIZE 0x10000
#define BOOTLOADER_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define FLASH_PAGE_SIZE 512
#define BOOTLOADER_DATA_AREA 1
#define SECTOR_SIZE 0x40000

#define BOOTLOADER_FLEXSPI_CLOCK kCLOCK_FlexSpi

#define FLASH_START_ADDRESS 0x60080000
#define FLASH_MAX_LENGTH 0x64000000- FLASH_START_ADDRESS

#define APP_START_ADDRESS 0x60080000
#define BOOTLOADER_START_ADDRESS 0x60000000



/*static variable*/
#define BOOT_STATUS_ADDRESS           (0x60040000 + 4*8)
#define BOOT_STATUS_DATA              (*((volatile uint32_t *) BOOT_STATUS_ADDRESS))
#define BOOT_STATUS_MAGIC							(0x424F4F54)
#endif

/*${macro:end}*/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern int flexspi_nor_flash_init(FLEXSPI_Type *base);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src);
extern status_t flexspi_nor_flash_buffer_program(FLEXSPI_Type *base, uint32_t address, const uint32_t *src, uint32_t length);
extern status_t flexspi_nor_hyperflash_cfi(FLEXSPI_Type *base);
extern status_t flexspi_nor_flash_erase_chip(FLEXSPI_Type *base);
 

//extern void AppStart(void);
extern void Reset_Handler(void);

typedef struct _dev_ {
	uint32_t familyID;	
  uint32_t extID;
	uint32_t pageSize;
	uint32_t totalPage;
	uint32_t sectorSize;
	uint32_t bootloaderAddr;
	uint32_t appAddr;
	uint32_t userAddr;
	uint32_t status;
}	dev;

/*${prototype:start}*/

/*${prototype:end}*/

#endif /* _APP_H_ */
