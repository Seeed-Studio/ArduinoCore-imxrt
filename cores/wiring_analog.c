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
 
#include "Arduino.h"
#include "wiring_analog.h"
#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_adc.h"
#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void analogReadResolution(int res)
{
    // Initialize Analog Controller
    adc_config_t adcConfigStrcut;
     /*
     *  config->enableAsynchronousClockOutput = true;
     *  config->enableOverWrite =               false;
     *  config->enableContinuousConversion =    false;
     *  config->enableHighSpeed =               false;
     *  config->enableLowPower =                false;
     *  config->enableLongSample =              false;
     *  config->referenceVoltageSource =        kADC_ReferenceVoltageSourceVref;
     *  config->samplePeriodMode =              kADC_SamplePeriod2or12Clocks;
     *  config->clockSource =                   kADC_ClockSourceAD;
     *  config->clockDriver =                   kADC_ClockDriver1;
     *  config->resolution =                    kADC_Resolution12Bit;
     */
      ADC_GetDefaultConfig(&adcConfigStrcut);

    /*change the resolutino of ADC*/
    switch (res)
    {
    case 8:
        adcConfigStrcut.resolution = kADC_Resolution12Bit;
        break;
    case 10:
        adcConfigStrcut.resolution = kADC_Resolution10Bit;
        break;
    case 12:        
        adcConfigStrcut.resolution = kADC_Resolution12Bit;
    default: /*if another num, set the resolution to default which mean 12Bit */
        adcConfigStrcut.resolution = kADC_Resolution12Bit;
        break;
    }

    ADC_Init(ADC1, &adcConfigStrcut);
    ADC_Init(ADC2, &adcConfigStrcut);
#if !(defined(FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE) && FSL_FEATURE_ADC_SUPPORT_HARDWARE_TRIGGER_REMOVE)
    ADC_EnableHardwareTrigger(ADC1, false);
    ADC_EnableHardwareTrigger(ADC2, false);
#endif
    
    // waitting for ADC Auto Calibiration
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC1))){}
    while (!(kStatus_Success == ADC_DoAutoCalibration(ADC2))){}
}

void analogWriteResolution(int res)
{
  
}


uint32_t analogRead(uint32_t pin)
{
    if(g_APinDescription[pin].ADC == NULL){ /*does not a AD Pin */
        return 0;
    }

    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

   IOMUXC_SetPinMux(
      g_APinDescription[pin].FUN_GPIO.muxRegister,       
      g_APinDescription[pin].FUN_GPIO.muxMode,
      g_APinDescription[pin].FUN_GPIO.inputRegister,
      g_APinDescription[pin].FUN_GPIO.inputDaisy,
      g_APinDescription[pin].FUN_GPIO.configRegister,
      0U);                                          /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      g_APinDescription[pin].FUN_GPIO.muxRegister,       
      g_APinDescription[pin].FUN_GPIO.muxMode,
      g_APinDescription[pin].FUN_GPIO.inputRegister,
      g_APinDescription[pin].FUN_GPIO.inputDaisy,
      g_APinDescription[pin].FUN_GPIO.configRegister,      /* GPIO_AD_B1_11 PAD functional properties : */
      0xB0u);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Disabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */


    adc_channel_config_t adcChannelConfigStruct;

    /* Configure the user channel and interrupt. */
    adcChannelConfigStruct.channelNumber                        = g_APinDescription[pin].channelNumber;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;

     /*
         When in software trigger mode, each conversion would be launched once calling the "ADC_ChannelConfigure()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC_ChannelConfigure() again.
        */
    ADC_SetChannelConfig(g_APinDescription[pin].ADC, 0, &adcChannelConfigStruct);

   while (0U == ADC_GetChannelStatusFlags(g_APinDescription[pin].ADC, 0)){}

   return ADC_GetChannelConversionValue(g_APinDescription[pin].ADC, 0);
}


// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t pin, uint32_t value)
{

 
}

#ifdef __cplusplus
}
#endif