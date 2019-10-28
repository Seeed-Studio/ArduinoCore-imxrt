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
 
 #include "app.h"
 #include "fsl_debug_console.h"
 
uint8_t flash_read_buff[FLASH_PAGE_SIZE];
 
 
 
 /* erase flash from begin address, and the length to erase */
status_t flash_erase(uint32_t address, uint32_t length)
{
	 status_t status = kStatus_Success;
	
	 /*sectors to earse*/
   uint32_t sectors = 0;
	
	 /*Check if the parameters are valid*/
	 if(0 != address % (uint32_t)SECTOR_SIZE){ 
		 return !kStatus_Success;
	 }
	 
	 if((address+length - BOOTLOADER_FLEXSPI_AMBA_BASE) > FLASH_MAX_LENGTH)
	 {
		  return !kStatus_Success;
	 }
	 
	 
	 /*calulate the sectors to erase*/
	 sectors = length / (uint32_t)SECTOR_SIZE;
	 
	 if(0 != length % (uint32_t)SECTOR_SIZE){
		 sectors += 1;
	 }
	 
	 
	 for(uint32_t i = 0; i < sectors; i++){
		
		status = FlexSPI_NorFlash_Erase_Sector(BOOTLOADER_FLEXSPI, address-BOOTLOADER_FLEXSPI_AMBA_BASE);
		if(kStatus_Success != status)
		{
			 return !kStatus_Success;
		}
		address += SECTOR_SIZE;
	 }
	 
	 return status;
}

 /* program flash from begin address, and the length to program */
status_t flash_program(uint32_t address, void * data)
{
	 status_t status;
	
	 status = FlexSPI_NorFlash_Page_Program(BOOTLOADER_FLEXSPI, address-BOOTLOADER_FLEXSPI_AMBA_BASE, (void *)data, FLASH_PAGE_SIZE);
	
	 if(kStatus_Success != status)
	 {
			 return !kStatus_Success;
	 }
	 
	 return status;
}

status_t flash_program_buffer(uint32_t address, void * data, uint32_t length)
{
	 status_t status;
	
	 if(address % SECTOR_SIZE == 0)
	 {
		 status = flash_erase(address, SECTOR_SIZE);
		 
		  if(kStatus_Success != status)
	    {
			 return !kStatus_Success;
	    }
	 }
	 
	 status = FlexSPI_NorFlash_Buffer_Program(BOOTLOADER_FLEXSPI, address-BOOTLOADER_FLEXSPI_AMBA_BASE, (void *)data, length);
	
	 if(kStatus_Success != status)
	 {
			 return !kStatus_Success;
	 }
	 
		
	 return status;

}

 /* read flash from begin address, and the length to read */
uint32_t flash_read(uint32_t address, void * data,const uint32_t length)
{
	
	 DCACHE_CleanInvalidateByRange(FlexSPI_AMBA_BASE +address, length);

   memcpy(flash_read_buff, (void *)(FlexSPI_AMBA_BASE + address),length);

	 
	 return length;
}
