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

#ifndef _MONITOR_IMRXT_UF2_H_
#define _MONITOR_IMRXT_UF2_H_

#define IMRXT_UF2_VERSION              "1.0"


#include <stdio.h>
#include <stdint.h>

#if !defined(IMRXT_BA_BOTH_INTERFACES) &&  !defined(IMRXT_BA_UART_ONLY) && !defined(IMRXT_BA_USBCDC_ONLY)
#define IMRXT_UF2_BOTH_INTERFACES
#endif

/* Selects USB as the communication interface of the monitor */
#define IMRXT_BA_INTERFACE_USBCDC     0
/* Selects USART as the communication interface of the monitor */
#define IMRXT_BA_INTERFACE_USART      1

/* Selects USB as the communication interface of the monitor */
#define SIZEBUFMAX                  64

/**
 * \brief Initialize the monitor
 *
 */
void imrxt_ba_monitor_init(uint8_t com_interface);

/**
 * \brief System tick function of the IMRXT_UF2 Monitor
 *
 */
void imrxt_ba_monitor_sys_tick(void);

/**
 * \brief Main function of the IMRXT_UF2 Monitor
 *
 */
void imrxt_ba_monitor_run(void);

/**
 * \brief
 *
 */
void imrxt_ba_putdata_term(uint8_t* data, uint32_t length);

/**
 * \brief
 *
 */
void call_applet(uint32_t address);

#endif // _MONITOR_IMRXT_UF2_H_
