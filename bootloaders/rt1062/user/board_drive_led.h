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

#ifndef _BOARD_DRIVER_LED_H
#define _BOARD_DRIVER_LED_H

#include "app.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"

#define IMRXT_BA_LED_GPIO  GPIO1
#define IMRXT_BA_LED_GPIO_PIN (9U)
#define IMRXT_BA_LEDRX_GPIO  GPIO1
#define IMRXT_BA_LEDRX_GPIO_PIN (10U)
#define IMRXT_BA_LEDTX_GPIO  GPIO1
#define IMRXT_BA_LEDTX_GPIO_PIN (11U)



void LED_init(void);
void LED_on(void);
void LED_off(void);
void LED_toggle(void);


void LEDRX_init(void);
void LEDRX_on(void);
void LEDRX_off(void);
void LEDRX_toggle(void);


void LEDTX_init(void);
void LEDTX_on(void);
void LEDTX_off(void);
void LEDTX_toggle(void);


#endif // _BOARD_DRIVER_LED_
