/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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

#include <string.h>
#include "imrxt_ba_monitor.h"
#include "imrxt_ba_cdc.h"
#include "imrxt_ba_flash.h"
#include "board_drive_led.h"
#include "app.h"
#include <stdlib.h>

const char RomBOOT_Version[] = IMRXT_BA_VERSION;
const char RomBOOT_ExtendedCapabilities[] = "[Arduino:Z]";

/* Provides one common interface to handle both USART and USB-CDC */
typedef struct
{
  /* send one byte of data */
  int (*put_c)(int value);
  /* Get one byte */
  int (*get_c)(void);
  /* Receive buffer not empty */
  bool (*is_rx_ready)(void);
  /* Send given data (polling) */
  uint32_t (*putdata)(void const* data, uint32_t length);
  /* Get data from comm. device */
  uint32_t (*getdata)(void* data, uint32_t length);
  /* Send given data (polling) using xmodem (if necessary) */
  uint32_t (*putdata_xmd)(void const* data, uint32_t length);
  /* Get data from comm. device using xmodem (if necessary) */
  uint32_t (*getdata_xmd)(void* data, uint32_t length);
} t_monitor_if;

#if defined(IMRXT_BA_USBCDC_ONLY)  ||  defined(IMRXT_BA_BOTH_INTERFACES)
//Please note that USB doesn't use Xmodem protocol, since USB already includes flow control and data verification
//Data are simply forwarded without further coding.
const t_monitor_if usbcdc_if =
{
  .put_c =         cdc_putc,
  .get_c =         cdc_getc,
  .is_rx_ready =   cdc_is_rx_ready,
  .putdata =       cdc_write_buf,
  .getdata =       cdc_read_buf,
  .putdata_xmd =   cdc_write_buf,
  .getdata_xmd =   cdc_read_buf_xmd
};
#endif


static const uint16_t crc16Table[256]=
{
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};

//*----------------------------------------------------------------------------
//* \brief Compute the CRC
//*----------------------------------------------------------------------------
unsigned short serial_add_crc(char ptr, unsigned short crc)
{
	return (crc << 8) ^ crc16Table[((crc >> 8) ^ ptr) & 0xff];
}


/* The pointer to the interface object use by the monitor */
t_monitor_if * ptr_monitor_if;

/* b_terminal_mode mode (ascii) or hex mode */
volatile bool b_terminal_mode = false;
volatile bool b_imrxt_ba_interface_usart = false;

/* Pulse generation counters to keep track of the time remaining for each pulse type */
#define TX_RX_LED_PULSE_PERIOD 100
volatile uint16_t txLEDPulse = 0; // time remaining for Tx LED pulse
volatile uint16_t rxLEDPulse = 0; // time remaining for Rx LED pulse


void imrxt_ba_monitor_init(uint8_t com_interface)
{
  if (com_interface == IMRXT_BA_INTERFACE_USBCDC)
  {
    ptr_monitor_if = (t_monitor_if*) &usbcdc_if;
  }
}

/*
 * Central IMRXT_BA monitor putdata function using the board LEDs
 */
static uint32_t imrxt_ba_putdata(t_monitor_if* pInterface, void const* data, uint32_t length)
{
	uint32_t result ;

	result=pInterface->putdata(data, length);
	#if _DEBUG_
	usb_echo("send:");
	uint8_t * ptr = (uint8_t *)data;
	for(int i = 0; i < length; i++){
		usb_echo("%c", *(ptr++));
	}
	usb_echo("\r\n");
  #endif
	LEDRX_off();
	LEDTX_on();
	txLEDPulse = TX_RX_LED_PULSE_PERIOD;

	return result;
}

/*
 * Central IMRXT_BA monitor getdata function using the board LEDs
 */
static uint32_t imrxt_ba_getdata(t_monitor_if* pInterface, void* data, uint32_t length)
{
	uint32_t result ;

	result=pInterface->getdata(data, length);

	if (result)
	{
		LEDTX_off();
		LEDRX_on();
		rxLEDPulse = TX_RX_LED_PULSE_PERIOD;
	}

	return result;
}

/*
 * Central IMRXT_BA monitor putdata function using the board LEDs
 */
static uint32_t imrxt_ba_putdata_xmd(t_monitor_if* pInterface, void const* data, uint32_t length)
{
	uint32_t result ;

	result=pInterface->putdata_xmd(data, length);
	
  LEDRX_off();
	LEDTX_on();
	txLEDPulse = TX_RX_LED_PULSE_PERIOD;

	return result;
}

/*
 * Central IMRXT_BA monitor getdata function using the board LEDs
 */
static uint32_t imrxt_ba_getdata_xmd(t_monitor_if* pInterface, void* data, uint32_t length)
{
	uint32_t result ;

	result=pInterface->getdata_xmd(data, length);

	if (result)
	{
		LEDTX_off();
		LEDRX_on();
		rxLEDPulse = TX_RX_LED_PULSE_PERIOD;
	}

	return result;
}

/**
 * \brief This function allows data emission by USART
 *
 * \param *data  Data pointer
 * \param length Length of the data
 */
void imrxt_ba_putdata_term(uint8_t* data, uint32_t length)
{
  uint8_t temp, buf[12], *data_ascii;
  uint32_t i, int_value;

  if (b_terminal_mode)
  {
    if (length == 4)
      int_value = *(uint32_t *) data;
    else if (length == 2)
      int_value = *(uint16_t *) data;
    else
      int_value = *(uint8_t *) data;

    data_ascii = buf + 2;
    data_ascii += length * 2 - 1;

    for (i = 0; i < length * 2; i++)
    {
      temp = (uint8_t) (int_value & 0xf);

      if (temp <= 0x9)
        *data_ascii = temp | 0x30;
      else
        *data_ascii = temp + 0x37;

      int_value >>= 4;
      data_ascii--;
    }
    buf[0] = '0';
    buf[1] = 'x';
    buf[length * 2 + 2] = '\n';
    buf[length * 2 + 3] = '\r';
    imrxt_ba_putdata(ptr_monitor_if, buf, length * 2 + 4);
  }
  else
    imrxt_ba_putdata(ptr_monitor_if, data, length);
  return;
}


extern void JumpToApp(uint32_t adderss);
void call_applet(uint32_t address)
{
	LEDRX_off();
	LEDRX_off();
	LED_off();
	__NVIC_SystemReset();
}

uint32_t current_number;
uint32_t i, length, flash_size;
uint8_t command, *ptr_data, *ptr, data[SIZEBUFMAX];
uint8_t j;
uint32_t u32tmp;

uint32_t PAGE_SIZE, PAGES, MAX_FLASH;

// Prints a 32-bit integer in hex.
static void put_uint32(uint32_t n)
{
  char buff[8];
  int i;
  for (i=0; i<8; i++)
  {
    int d = n & 0XF;
    n = (n >> 4);

    buff[7-i] = d > 9 ? 'A' + d - 10 : '0' + d;
  }
  imrxt_ba_putdata( ptr_monitor_if, buff, 8);
}

static void imrxt_ba_monitor_loop(void)
{
  length = imrxt_ba_getdata(ptr_monitor_if, data, 512);
  ptr = data;
	#if _DEBUG_
	if(length != 0){
		usb_echo("rec:");
		for(int i = 0; i < length; i++){
			usb_echo("%c", data[i]);
		}
		usb_echo("\r\n");
	}
	#endif
	
  for (i = 0; i < length; i++, ptr++)
  {
    if (*ptr == 0xff) continue;

    if (*ptr == '#')
    {
      if (b_terminal_mode)
      {
        imrxt_ba_putdata(ptr_monitor_if, "\n\r", 2);
      }
      if (command == 'S')
      {
				//Check if some data are remaining in the "data" buffer
        if(length>i)
        {
          //Move current indexes to next avail data (currently ptr points to "#")
          ptr++;
          i++;

          //We need to add first the remaining data of the current buffer already read from usb
          //read a maximum of "current_number" bytes
          if ((length-i) < current_number)
          {
            u32tmp=(length-i);
          }
          else
          {
            u32tmp=current_number;
          }

          memcpy(ptr_data, ptr, u32tmp);
          i += u32tmp;
          ptr += u32tmp;
          j = u32tmp;
        }
        //update i with the data read from the buffer
        i--;
        ptr--;
        //Do we expect more data ?
				flash_size = current_number - j;
			
        if(j<current_number)
          imrxt_ba_getdata_xmd(ptr_monitor_if, ptr_data, current_number-j);
				
        break;
      }
      else if (command == 'R')
      {
        imrxt_ba_putdata_xmd(ptr_monitor_if, ptr_data, current_number);
      }
      else if (command == 'O')
      {
        *ptr_data = (char) current_number;
      }
      else if (command == 'H')
      {
        *((uint16_t *) ptr_data) = (uint16_t) current_number;
				imrxt_ba_putdata_term((uint8_t*) &current_number, 2);
      }
      else if (command == 'W')
      {
        break;
      }
      else if (command == 'o')
      {
        imrxt_ba_putdata_term(ptr_data, 1);
      }
      else if (command == 'h')
      {
        current_number = *((uint16_t *) ptr_data);
        imrxt_ba_putdata_term((uint8_t*) &current_number, 2);
      }
      else if (command == 'w')
      {
        current_number = *((uint32_t *) ptr_data);
        imrxt_ba_putdata_term((uint8_t*) &current_number, 4);
      }
      else if (command == 'G')
      {
        call_applet(current_number);
      }
      else if (command == 'T')
      {
        b_terminal_mode = 1;
        imrxt_ba_putdata(ptr_monitor_if, "\n\r", 2);
      }
      else if (command == 'N')
      {
        if (b_terminal_mode == 0)
        {
          imrxt_ba_putdata( ptr_monitor_if, "\n\r", 2);
        }
        b_terminal_mode = 0;
      }
      else if (command == 'V')
      {
        imrxt_ba_putdata( ptr_monitor_if, "v", 1);
        imrxt_ba_putdata( ptr_monitor_if, (uint8_t *) RomBOOT_Version, strlen(RomBOOT_Version));
        imrxt_ba_putdata( ptr_monitor_if, " ", 1);
        imrxt_ba_putdata( ptr_monitor_if, (uint8_t *) RomBOOT_ExtendedCapabilities, strlen(RomBOOT_ExtendedCapabilities));
        imrxt_ba_putdata( ptr_monitor_if, " ", 1);
        ptr = (uint8_t*) &(__DATE__);
        i = 0;
        while (*ptr++ != '\0')
          i++;
        imrxt_ba_putdata( ptr_monitor_if, (uint8_t *) &(__DATE__), i);
        imrxt_ba_putdata( ptr_monitor_if, " ", 1);
        i = 0;
        ptr = (uint8_t*) &(__TIME__);
        while (*ptr++ != '\0')
          i++;
        imrxt_ba_putdata( ptr_monitor_if, (uint8_t *) &(__TIME__), i);
        imrxt_ba_putdata( ptr_monitor_if, "\n\r", 2);
      }
      else if (command == 'X')
      {
        // Syntax: X[ADDR]#
        // Erase the flash memory starting from ADDR to the end of flash.

        // Note: the flash memory is erased in ROWS, that is in block of 4 pages.
        //       Even if the starting address is the last byte of a ROW the entire
        //       ROW is erased anyway.
				#if _DEBUG_
				usb_echo("%X \r\n", current_number);
				#endif
       
      }
      else if (command == 'Y')
      {
        // This command writes the content of a buffer in SRAM into flash memory.

        // Syntax: Y[ADDR],0#
        // Set the starting address of the SRAM buffer.

        // Syntax: Y[ROM_ADDR],[SIZE]#
        // Write the first SIZE bytes from the SRAM buffer (previously set) into
        // flash memory starting from address ROM_ADDR
				#if _DEBUG_
        usb_echo("%X \r\n", current_number);
				#endif
				break;
        // Set automatic page write
      }else if (command == 'y')
      {
        // This command writes the content of a buffer in SRAM into flash memory.

        // Syntax: y[DST],[SRC]#
        // Set the starting address of the SRAM buffer.

        // Syntax: Y[ROM_ADDR],[SIZE]#
        // Write the first SIZE bytes from the SRAM buffer (previously set) into
        // flash memory starting from address ROM_ADDR
        uint32_t src = current_number;
				uint32_t *dst = (uint32_t *)ptr_data;
				
				uint8_t *ptrt = (uint8_t *)src;

				
				status_t status = flash_program_buffer((uint32_t)dst+APP_START_ADDRESS, (void *)ptrt, flash_size);
			  if(kStatus_Success != status)
	      {
			    imrxt_ba_putdata( ptr_monitor_if, "y\n\r", 3);
					break;
	      }
			
				imrxt_ba_putdata( ptr_monitor_if, "Y\n\r", 3);
				
				break;
        // Set automatic page write
      }
      else if (command == 'Z')
      {
        // This command calculate CRC for a given area of memory.
        // It's useful to quickly check if a transfer has been done
        // successfully.

        // Syntax: Z[START_ADDR],[SIZE]#
        // Returns: Z[CRC]#
				
				uint8_t *data;
        uint32_t size = current_number;
        uint16_t crc = 0;
        uint32_t i = 0;
				
				data = (uint8_t *)(ptr_data + APP_START_ADDRESS);
				for (i=0; i<size; i++)
          crc = serial_add_crc(*data++, crc);

        // Send response
        imrxt_ba_putdata( ptr_monitor_if, "Z", 1);
        put_uint32(crc);
        imrxt_ba_putdata( ptr_monitor_if, "#\n\r", 3);
      }

      command = 'z';
      current_number = 0;

      if (b_terminal_mode)
      {
        imrxt_ba_putdata( ptr_monitor_if, ">", 1);
      }
    }
    else
    {
      if (('0' <= *ptr) && (*ptr <= '9'))
      {
        current_number = (current_number << 4) | (*ptr - '0');
      }
      else if (('A' <= *ptr) && (*ptr <= 'F'))
      {
        current_number = (current_number << 4) | (*ptr - 'A' + 0xa);
      }
      else if (('a' <= *ptr) && (*ptr <= 'f'))
      {
        current_number = (current_number << 4) | (*ptr - 'a' + 0xa);
      }
      else if (*ptr == ',')
      {
        ptr_data = (uint8_t *) current_number;
        current_number = 0;
      }
      else
      {
        command = *ptr;
        current_number = 0;
      }
    }
  }
}

void imrxt_ba_monitor_sys_tick(void)
{
	/* Check whether the TX or RX LED one-shot period has elapsed.  if so, turn off the LED */
	if (txLEDPulse && !(--txLEDPulse))
		LEDTX_off();
	if (rxLEDPulse && !(--rxLEDPulse))
		LEDRX_off();
}

/**
 * \brief This function starts the IMRXT_BA monitor.
 */
void imrxt_ba_monitor_run(void)
{

  ptr_data = NULL;
  command = 'z';
	LED_on();
  while (1)
  {
   imrxt_ba_monitor_loop();
  }

}



