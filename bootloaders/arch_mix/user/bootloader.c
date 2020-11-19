/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_flexspi.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"
#include "virtual_com.h"
#include "imxrt_ba_monitor.h"
#include "board_drive_led.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

dev devInfo = {
	.familyID = 0x00005254, // 'R' 'T'
	.extID = 0x31303532,    // '1' '0' '5' '2'
	.pageSize = FLASH_PAGE_SIZE,
	.totalPage = 0x7DE00, //
	.sectorSize = SECTOR_SIZE,
	.bootloaderAddr = BOOTLOADER_START_ADDRESS,
	.appAddr = APP_START_ADDRESS,
	.userAddr = 0x20021FC0,
	.status = 0x00
};	
 
extern void JumpToApp(uint32_t address);

void writeStatus(uint32_t magic)
{
	  status_t status;
		status = flexspi_nor_flash_erase_sector(BOOTLOADER_FLEXSPI, BOOTLOADER_DATA_AREA * SECTOR_SIZE);
    if (status != kStatus_Success)
    {
       //PRINTF("Erase sector failure !\r\n");
       return ;
    }
		
		devInfo.status = magic;
		
		status = flexspi_nor_flash_buffer_program(BOOTLOADER_FLEXSPI, BOOTLOADER_DATA_AREA * SECTOR_SIZE,
                                            (void *)&devInfo, 40);
    if (status != kStatus_Success)
    {
        //PRINTF("Page program failure !\r\n");
        return ;
    }
}

void call_application(uint32_t address)
{
	__disable_irq();
	
	uint32_t vector = address + 0x2000; 

	
	//write_vtor
	unsigned long *pVTOR = (unsigned long*)0xE000ED08;
  *pVTOR = vector;
	
	/* Rebase the Stack Pointer */
  __set_MSP(*(uint32_t *) vector);
	
	//SystemInit();
	
	__enable_irq();
	
	 /* Load the Reset Handler address of the application */
  uint32_t  app_start_address = vector + 0x4b5;  // jump to reset handle address

  /* Jump to application Reset Handler in the application */
  JumpToApp(app_start_address);
	
}

void check_application()
{
	/* Check if bootloader data area (in Flash at 0xF000) contains the bootloader info structure */
	 if(BOOT_STATUS_MAGIC != BOOT_STATUS_DATA)
	 {
			writeStatus(BOOT_STATUS_MAGIC);
		 LED_on();
  	 for(uint32_t i = 0; i < 4096; i++)
			 for(uint32_t j = 0; j < 102400; j++);
		 LED_off();
	 }

	 if (GPIO_PinRead(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN)) {
			// user button NOT pressed, continue to application
			//usb_echo("APP MODE\r\n");
			LED_off();
			LEDTX_off();
			LEDRX_off(); // app will run, turn off al LEDs
			call_application(APP_START_ADDRESS);
	 }
	 else {
		 // user button pressed, stay in bootloader mode
		 LEDTX_on(); // blue, bootloader mode
		 //usb_echo("BOOTLOADER MODE\r\n");
		 return;
	 }
}

int main(void)
{
		BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    SCB_DisableDCache();
	  flexspi_nor_flash_init(BOOTLOADER_FLEXSPI);
	  // flexspi_nor_enable_quad_mode(BOOTLOADER_FLEXSPI);
    LED_init();
	  LEDRX_init();
	  LEDTX_init();
		LED_off();
		LEDRX_off();
		LEDTX_off();

		// Init user button
		gpio_pin_config_t button_config = {kGPIO_DigitalInput, 0, kGPIO_NoIntmode};
  	GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &button_config);
	 
	  check_application();
	
	  vcom_cdc_init();
	   
	  imxrt_ba_monitor_init(IMRXT_BA_INTERFACE_USBCDC);
  
    while (1)
    {
			imxrt_ba_monitor_run();
    }
}
