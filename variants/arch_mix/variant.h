/*
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
#ifndef PINS_ARDUINO_ARCH_MIX
#define PINS_ARDUINO_ARCH_MIX

#include "WVariant.h"
#include "fsl_lpuart.h"
#include "fsl_common.h"
#include "fsl_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplu


#define J3_4    0U
#define J3_5    1U
#define J3_6    2U
#define J3_7    3U
#define J3_8    4U
#define J3_9    5U
#define J3_10    6U
#define J3_11    7U
#define J3_12    8U
#define J3_13    9U
#define J3_14    10U
#define J3_15    11U
#define J3_16    12U
#define J3_17    13U
#define J3_19    14U
#define J3_20    15U

#define J4_4    16U
#define J4_5    17U
#define J4_6    18U
#define J4_7    19U
#define J4_8    20U
#define J4_9    21U
#define J4_10    22U
#define J4_11    23U
#define J4_12    24U
#define J4_13    25U
#define J4_14    26U
#define J4_15    27U
#define J4_16    28U
#define J4_17    29U


#define USER_LED_R 30U
#define USER_LED_G 31U
#define USER_LED_B 32U

#define LED_BUILTIN USER_LED_G

#define UART_INTERFACES_COUNT 3

#define PIN_UART1_RX      J3_19
#define PIN_UART1_TX      J3_20
#define UART1_NUM         LPUART1
#define UART1_IRQn        LPUART1_IRQn
#define UART1_HANDLER     LPUART1_IRQHandler

#define PIN_UART2_RX      J4_11
#define PIN_UART2_TX      J4_10
#define UART2_NUM         LPUART8
#define UART2_IRQn        LPUART8_IRQn
#define UART2_HANDLER     LPUART8_IRQHandler

#define PIN_UART3_RX      J4_17
#define PIN_UART3_TX      J4_16
#define UART3_NUM         LPUART2
#define UART3_IRQn        LPUART2_IRQn
#define UART3_HANDLER     LPUART2_IRQHandler

#define AD2_9  J3_9
#define AD2_10  J3_8
#define AD1_3  J3_14
#define AD1_4  J3_15

#define PWM4_0  J4_8
#define PWM4_1  J4_9

#define I2C1    LPI2C1
#define I2C1_SCL    J3_16
#define I2C1_SDA    J3_17

// default I2C PORT
#define SCL I2C1_SCL
#define SDA I2C1_SDA

#define I2C3    LPI2C3
#define I2C3_SCL    J4_7
#define I2C3_SDA    J4_6

// default I2C PORT
#define SPI3_MOSI    J4_13
#define SPI3_MISO    J4_14
#define SPI3_SCK     J4_15

#define MOSI SPI3_MOSI
#define MISO SPI3_MISO
#define SCK  SPI3_SCK


#ifdef __cplusplus
}
#endif


#endif /*end of  PINS_ARDUINO_ARCH_MIX*/