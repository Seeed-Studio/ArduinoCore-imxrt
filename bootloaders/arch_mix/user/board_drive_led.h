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



void LED_init(void) 
{  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        /* GPIO_AD_B0_09 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	GPIO_PinInit(IMRXT_BA_LED_GPIO, IMRXT_BA_LED_GPIO_PIN, &led_config);
}
void LED_on(void) {GPIO_PinWrite(IMRXT_BA_LED_GPIO, IMRXT_BA_LED_GPIO_PIN, 1U); }
void LED_off(void) { GPIO_PinWrite(IMRXT_BA_LED_GPIO, IMRXT_BA_LED_GPIO_PIN, 0U);}
void LED_toggle(void) { }


void LEDRX_init(void) 
{ 
	IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 is configured as GPIO1_IO10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_10_GPIO1_IO10,        /* GPIO_AD_B0_10 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	GPIO_PinInit(IMRXT_BA_LEDRX_GPIO, IMRXT_BA_LEDRX_GPIO_PIN, &led_config); 
}
void LEDRX_on(void) { GPIO_PinWrite(IMRXT_BA_LEDRX_GPIO, IMRXT_BA_LEDRX_GPIO_PIN, 1U); }
void LEDRX_off(void) { GPIO_PinWrite(IMRXT_BA_LEDRX_GPIO, IMRXT_BA_LEDRX_GPIO_PIN, 0U); }
void LEDRX_toggle(void) { }


void LEDTX_init(void) 
{
	
	IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,        /* GPIO_AD_B0_11is configured as GPIO1_IO11 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_11_GPIO1_IO11,        /* GPIO_AD_B0_011 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
	gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
	GPIO_PinInit(IMRXT_BA_LEDTX_GPIO, IMRXT_BA_LEDTX_GPIO_PIN, &led_config);
}
void LEDTX_on(void) { GPIO_PinWrite(IMRXT_BA_LEDTX_GPIO, IMRXT_BA_LEDTX_GPIO_PIN, 1U); }
void LEDTX_off(void) { GPIO_PinWrite(IMRXT_BA_LEDTX_GPIO, IMRXT_BA_LEDTX_GPIO_PIN, 0U);}
void LEDTX_toggle(void) { }


#endif // _BOARD_DRIVER_LED_
