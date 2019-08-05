/**
 * The MIT License (MIT)
 * 
 * Author: Hongtai Liu (lht856@fox.com)
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

#include "Arduino.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "board.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @description: Configures the specified pin to behave either as an input or an
 * output. See the description of (digital pins) for details on the functionality
 *  of the pins.
 * @param: the number of the pin whose mode you wish to set
 * @param: INPUT, OUTPUT, or INPUT_PULLUP. (see the (digital pins) page for a more
 *   complete description of the functionality.)
 * @return: Nothing
 */
void pinMode(pin_size_t pinNumber, uint8_t pinMode) {
   CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

   IOMUXC_SetPinMux(
      g_APinDescription[pinNumber].FUN_GPIO.muxRegister,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
      g_APinDescription[pinNumber].FUN_GPIO.muxMode,
      g_APinDescription[pinNumber].FUN_GPIO.inputRegister,
      g_APinDescription[pinNumber].FUN_GPIO.inputDaisy,
      g_APinDescription[pinNumber].FUN_GPIO.configRegister,
      0U);                                          /* Software Input On Field: Input Path is determined by functionality */
   IOMUXC_SetPinConfig(
      g_APinDescription[pinNumber].FUN_GPIO.muxRegister,        /* GPIO_AD_B0_09 is configured as GPIO1_IO09 */
      g_APinDescription[pinNumber].FUN_GPIO.muxMode,
      g_APinDescription[pinNumber].FUN_GPIO.inputRegister,
      g_APinDescription[pinNumber].FUN_GPIO.inputDaisy,
      g_APinDescription[pinNumber].FUN_GPIO.configRegister,        /* GPIO_AD_B0_09 PAD functional properties : */
      0x10B0U);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */

    gpio_pin_config_t  led_config = {kGPIO_DigitalInput, 0, kGPIO_NoIntmode};  
    
    switch (pinMode)
    {
    case OUTPUT:
        led_config.direction = kGPIO_DigitalOutput;
        break;
    case INPUT:
        led_config.direction = kGPIO_DigitalInput;
        break;
    default:
        led_config.direction = kGPIO_DigitalInput;
        break;
    }

    GPIO_PinInit(g_APinDescription[pinNumber].GROUP, g_APinDescription[pinNumber].PIN, &led_config);
}

/**
* @description: Write a HIGH or a LOW value to a digital pin.
* If the pin has been configured as an OUTPUT with pinMode(), its voltage will be
*  set to the corresponding value: 5V (or 3.3V on 3.3V boards) for HIGH, 0V
*  (ground) for LOW.
* If you do not set the pinMode() to OUTPUT, and connect an LED to a pin, when calling
* digitalWrite(HIGH), the LED may appear dim. Without explicitly setting pinMode(),
* digitalWrite() will have enabled the internal pull-up resistor, which acts like a
* large current-limiting resistor.
* @param: The pin number
* @param: HIGH or LOW
* @return: Nothing
*/
void digitalWrite(pin_size_t pinNumber, uint8_t status) {

    if(status != 0 && status !=1){
        return;
    }
    GPIO_PinWrite(g_APinDescription[pinNumber].GROUP, g_APinDescription[pinNumber].PIN, status);
}

/**
 * @description: Reads define
 * @param: The number odefine
 * @return: HIGH or LOWdefine
 */
uint8_t digitalRead(pin_size_t pinNumber) {
    return GPIO_PinRead(g_APinDescription[pinNumber].GROUP, g_APinDescription[pinNumber].PIN);
}

#ifdef __cplusplus
}
#endif
