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

#include "flexspi_hyper_flash_ops.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
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

/** Copied from bootloader. It will cause a "stay in bootloader" when doing 1200bps-touch */
#define BOOT_BPS_ADDRESS         (0x20007FFCul)
#define BOOT_BPS_DATA            (*((volatile uint32_t *) BOOT_BPS_ADDRESS))



/*${macro:end}*/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

 
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


/*flash option*/

#define FLASH_ADDR_LENGTH    32              

/* FLASH command */
#define W25Q_WriteEnable                0x06
#define W25Q_WriteDisable               0x04
#define W25Q_ReadStatusReg              0x05
#define W25Q_WriteStatusReg             0x01
#define W25Q_ReadData                   0x03
#define W25Q_ReadData_4Addr             0x13
#define W25Q_FastReadData               0x0B
#define W25Q_FastReadData_4Addr         0x0C
#define W25Q_FastReadDual               0x3B
#define W25Q_FastReadDual_4Addr         0x3C
#define W25Q_FastReadQuad               0x6B
#define W25Q_FastReadQuad_4Addr         0x6C
#define W25Q_PageProgram                0x02
#define W25Q_PageProgram_4Addr          0x12
#define W25Q_PageProgramQuad            0x32
#define W25Q_PageProgramQuad_4Addr      0x34
#define W25Q_BlockErase                 0xD8
#define W25Q_BlockErase_4Addr           0xDC
#define W25Q_SectorErase                0x20
#define W25Q_SectorErase_4Addr          0x21
#define W25Q_ChipErase                  0xC7
#define W25Q_PowerDown                  0xB9
#define W25Q_ReleasePowerDown           0xAB
#define W25Q_DeviceID                   0xAB
#define W25Q_ManufactDeviceID           0x90
#define W25Q_JedecDeviceID              0x9F
/*other*/
#define FLASH_ID                        0X18  
#define FLASH_WINBOND_JEDECDEVICE_ID    0XEF4019   
#define FLASH_ISSI_JEDECDEVICE_ID       0X9D6019  

/* The polarity of the BUSY bit of the status register of FLASH,When is 1, it means busy */
#define FLASH_BUSY_STATUS_POL       1
/* Offset of the BUSY bit of FLASH's status register,zero */
#define FLASH_BUSY_STATUS_OFFSET    0


/*! @brief FlexSPI pin config */
#define NORFLASH_SS_IOMUXC            IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B
#define NORFLASH_SCLK_IOMUXC          IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK
#define NORFLASH_DATA00_IOMUXC        IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00
#define NORFLASH_DATA01_IOMUXC        IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01
#define NORFLASH_DATA02_IOMUXC        IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02
#define NORFLASH_DATA03_IOMUXC        IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03

/* FlexSPI_AMBA_BAS 0x6000 0000 */

#define NORFLASH_AHB_POINTER(addr)              (void*)(FlexSPI_AMBA_BASE + addr)
/* 8*/
#define NORFLASH_AHB_READ_BYTE(addr)            *(uint8_t *)NORFLASH_AHB_POINTER(addr)
#define NORFLASH_AHB_WRITE_BYTE(addr, data)     *(uint8_t *)NORFLASH_AHB_POINTER(addr) = data
/* 16 */
#define NORFLASH_AHB_READ_HALFWORD(addr)        *(uint16_t *)NORFLASH_AHB_POINTER(addr)
#define NORFLASH_AHB_WRITE_HALFWORD(addr, data) *(uint16_t *)NORFLASH_AHB_POINTER(addr) = data
/* 32 */
#define NORFLASH_AHB_READ_WORD(addr)            *(uint32_t *)NORFLASH_AHB_POINTER(addr)
#define NORFLASH_AHB_WRITE_WORD(addr, data)     *(uint32_t *)NORFLASH_AHB_POINTER(addr) = data

/* for debug */
#define FLASH_DEBUG_ON             1
#define FLASH_DEBUG_ARRAY_ON       1
#define FLASH_DEBUG_FUNC_ON        1
// Log define
#define FLASH_INFO(fmt,arg...)           PRINTF("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          PRINTF("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                         if(FLASH_DEBUG_ON)\
                                         PRINTF("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define FLASH_DEBUG_ARRAY(array, num)    do{\
                                             int32_t i;\
                                             uint8_t* a = array;\
                                             if(FLASH_DEBUG_ARRAY_ON)\
                                             {\
                                                PRINTF("<<-FLASH-DEBUG-ARRAY->>\n");\
                                                for (i = 0; i < (num); i++)\
                                                {\
                                                    PRINTF("%02x   ", (a)[i]);\
                                                    if ((i + 1 ) %10 == 0)\
                                                    {\
                                                        PRINTF("\n");\
                                                    }\
                                                }\
                                                PRINTF("\n");\
                                            }\
                                          }while(0)

#define FLASH_DEBUG_FUNC()             do{\
                                           if(FLASH_DEBUG_FUNC_ON)\
                                           PRINTF("<<-FLASH-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                          }while(0)

/*******************************************************************************
 * 
 ******************************************************************************/
void FlexSPI_NorFlash_Init(void);
status_t FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI_Type *base, uint32_t *vendorID);
status_t FlexSPI_NorFlash_Get_Device_ID(FLEXSPI_Type *base, uint8_t *vendorID);
status_t FlexSPI_NorFlash_Write_Enable(FLEXSPI_Type *base);
status_t FlexSPI_NorFlash_Erase_Sector(FLEXSPI_Type *base, uint32_t dstAddr);
status_t FlexSPI_NorFlash_Wait_Bus_Busy(FLEXSPI_Type *base);
status_t FlexSPI_NorFlash_Write_Status_Register(FLEXSPI_Type *base, uint8_t *config);
status_t FlexSPI_NorFlash_Read_Status_Register(FLEXSPI_Type *base, uint8_t *SR_value);	 
status_t FlexSPI_NorFlash_Enable_Quad_Mode(FLEXSPI_Type *base);

status_t FlexSPI_NorFlash_Page_Program(FLEXSPI_Type *base, 
                                       uint32_t dstAddr, 
                                       uint8_t *src,
                                       uint16_t dataSize);

status_t FlexSPI_NorFlash_Buffer_Program(FLEXSPI_Type *base, 
                                         uint32_t dstAddr, 
                                         uint8_t *src,
                                         uint16_t dataSize);

status_t FlexSPI_NorFlash_Buffer_Read(FLEXSPI_Type *base, 
                                      uint32_t address, 
                                      uint8_t *dst,
                                      uint16_t dataSize);
																			
/* SRE  */
#define SRE_0_SLOW_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(0)
#define SRE_1_FAST_SLEW_RATE                IOMUXC_SW_PAD_CTL_PAD_SRE(1)

/* , */
#define DSE_0_OUTPUT_DRIVER_DISABLED        IOMUXC_SW_PAD_CTL_PAD_DSE(0)
/* R0 260 Ohm @ 3.3V, 150Ohm@1.8V, 240 Ohm for DDR */
#define DSE_1_R0_1                          IOMUXC_SW_PAD_CTL_PAD_DSE(1) 
/* R0/2 */
#define DSE_2_R0_2                          IOMUXC_SW_PAD_CTL_PAD_DSE(2)
/* R0/3 */
#define DSE_3_R0_3                          IOMUXC_SW_PAD_CTL_PAD_DSE(3)
/* R0/4 */
#define DSE_4_R0_4                          IOMUXC_SW_PAD_CTL_PAD_DSE(4)
/* R0/5 */
#define DSE_5_R0_5                          IOMUXC_SW_PAD_CTL_PAD_DSE(5)
/* R0/6 */
#define DSE_6_R0_6                          IOMUXC_SW_PAD_CTL_PAD_DSE(6)
/* R0/7 */
#define DSE_7_R0_7                          IOMUXC_SW_PAD_CTL_PAD_DSE(7)

/* SPEED  */
#define SPEED_0_LOW_50MHz                   IOMUXC_SW_PAD_CTL_PAD_SPEED(0)
#define SPEED_1_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(1)
#define SPEED_2_MEDIUM_100MHz               IOMUXC_SW_PAD_CTL_PAD_SPEED(2)
#define SPEED_3_MAX_200MHz                  IOMUXC_SW_PAD_CTL_PAD_SPEED(3)

/* ODE  */
#define ODE_0_OPEN_DRAIN_DISABLED           IOMUXC_SW_PAD_CTL_PAD_ODE(0)     
#define ODE_1_OPEN_DRAIN_ENABLED            IOMUXC_SW_PAD_CTL_PAD_ODE(1)     

/* PKE  */
#define PKE_0_PULL_KEEPER_DISABLED          IOMUXC_SW_PAD_CTL_PAD_PKE(0)      
#define PKE_1_PULL_KEEPER_ENABLED           IOMUXC_SW_PAD_CTL_PAD_PKE(1)      

/* PUE  */
#define PUE_0_KEEPER_SELECTED               IOMUXC_SW_PAD_CTL_PAD_PUE(0)   
#define PUE_1_PULL_SELECTED                 IOMUXC_SW_PAD_CTL_PAD_PUE(1)   

/* PUS  */
#define PUS_0_100K_OHM_PULL_DOWN            IOMUXC_SW_PAD_CTL_PAD_PUS(0)     
#define PUS_1_47K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(1)   
#define PUS_2_100K_OHM_PULL_UP              IOMUXC_SW_PAD_CTL_PAD_PUS(2)   
#define PUS_3_22K_OHM_PULL_UP               IOMUXC_SW_PAD_CTL_PAD_PUS(3)   

/* HYS  */
#define HYS_0_HYSTERESIS_DISABLED           IOMUXC_SW_PAD_CTL_PAD_HYS(0)  
#define HYS_1_HYSTERESIS_ENABLED            IOMUXC_SW_PAD_CTL_PAD_HYS(1)  	


#endif /* _APP_H_ */
