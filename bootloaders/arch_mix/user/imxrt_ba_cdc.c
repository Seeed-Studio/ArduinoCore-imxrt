/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.
	Copyright (c) 2019 Seeedstudio Corporation/Thibaut VIARD.  All right reserved.

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

#include "imxrt_ba_cdc.h"



int cdc_putc(int value)
{
	if(!cdc_is_rx_ready()){
		return 0;
	}

  return vcom_write_buf(&value, 1);

}

int cdc_getc(void)
{
	if(!cdc_is_rx_ready()){
		return 0;
	}
	uint8_t rx_char;

  vcom_read_buf(&rx_char, 1);	
		
  return (int)rx_char;
}

bool cdc_is_rx_ready(void)
{
	if ((1 == s_cdcVcom.attach) && (1 == s_cdcVcom.startTransactions))
		return true;
	else
		return false;
}

uint32_t cdc_write_buf(void const* data, uint32_t length)
{
	if(!cdc_is_rx_ready()){
		return 0;
	}
	
	vcom_write_buf((void *)data, length);

  return length;
}

uint32_t cdc_read_buf(void* data, uint32_t length)
{
   return vcom_read_buf(data, length);		
	
}

uint32_t cdc_read_buf_xmd(void* data, uint32_t length)
{
	if(!cdc_is_rx_ready()){
		return 0;
	}

  char *dst = (char *)data;
  int32_t remaining = length;
  while (remaining > 0)
  {
		uint32_t readed = vcom_read_buf(dst, length);	
  
    remaining -= readed;
    dst += readed;
  }

  return length;
}


