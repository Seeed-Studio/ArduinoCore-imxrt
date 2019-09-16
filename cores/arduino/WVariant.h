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

#pragma once

#include <stdint.h>
#include <limits.h>

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_adc.h"
#include "fsl_pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MuxConfig 
{
    uint32_t muxRegister;
    uint32_t muxMode;
    uint32_t inputRegister;
    uint32_t inputDaisy;
    uint32_t configRegister;
} MuxConfig;

typedef enum _EPioType
{
  PIO_NOT_A_PIN=-1,          
  PIO_ANALOG,                    
  PIO_UART,
  PIO_PWM,
  PIO_TIMER,
  PIO_I2C,
  PIO_SPI,
  PIO_CAN,
  PIO_SAI,
  PIO_ENET,
  PIO_CSI,
  PIO_SPDIF,
      
} EPioType ;

typedef struct _PinDescription {
  GPIO_Type*  GROUP;
  uint32_t PIN;
  ADC_Type *  ADC;
  uint32_t adcChannel;
  PWM_Type * PWM;
  pwm_submodule_t pwm_submodule;
  pwm_channels_t pwm_channel; 
  MuxConfig FUN_GPIO;
  MuxConfig FUN_UART;
  MuxConfig FUN_PWM;
  MuxConfig FUN_TIMER;
  MuxConfig FUN_I2C;
  MuxConfig FUN_SPI;
  MuxConfig FUN_CAN;
  MuxConfig FUN_SAI;
  MuxConfig FUN_ENET;
  MuxConfig FUN_CSI;
  MuxConfig FUN_SPDIF;
} PinDescription;


/* Pins table to be instantiated into variant.cpp */
extern const PinDescription g_APinDescription[] ;

#ifdef __cplusplus
} // extern "C"
#endif