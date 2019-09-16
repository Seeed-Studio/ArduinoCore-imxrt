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
#include "wiring_private.h"
#include "wiring_analog.h"
#include "fsl_adc.h"
#include "fsl_pwm.h"

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

uint32_t analogRead(uint32_t pin)
{
    PinDescription const *g_pinDes = &g_APinDescription[pin];

    if(g_pinDes->ADC == NULL){ /*does not a AD Pin */
        return 0;
    }

    //IO MUX
    pinPeripheral(pin, 0U, FUN_GPIO, 0xB0u);


    adc_channel_config_t adcChannelConfigStruct;

    /* Configure the user channel and interrupt. */
    adcChannelConfigStruct.channelNumber                        = g_pinDes->adcChannel;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;

     /*
         When in software trigger mode, each conversion would be launched once calling the "ADC_ChannelConfigure()"
         function, which works like writing a conversion command and executing it. For another channel's conversion,
         just to change the "channelNumber" field in channel's configuration structure, and call the
         "ADC_ChannelConfigure() again.
        */
    ADC_SetChannelConfig(g_pinDes->ADC, 0, &adcChannelConfigStruct);

   while (0U == ADC_GetChannelStatusFlags(g_pinDes->ADC, 0)){}

   return ADC_GetChannelConversionValue(g_pinDes->ADC, 0);
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint32_t pin, uint32_t value)
{
    
    if (value == 0) // No need to turn on PWM
	{
        pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW); return;
	}
	else if (value >= 255)
	{
        pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH); return;
	}

    PinDescription const *g_pinDes = &g_APinDescription[pin];
     if(g_pinDes->PWM == NULL){ /*does not a PWM Pin */
         if (value < 128)
	    {
            pinMode(pin, OUTPUT);
		    digitalWrite(pin, LOW); return;
	    }
	    else 
	    {
            pinMode(pin, OUTPUT);
		    digitalWrite(pin, HIGH); return;
	    }
        return;
    }

    uint8_t  pwmVal = 100*value / 255; // map 0-255 to 0-100

    if(!(g_pinDes->PWM->MCTRL & (PWM_MCTRL_RUN((1U << g_pinDes->pwm_submodule))))){ // if first use pin as pwm

     /*Initialize the flexPWM  PWM4_A0  PWM4_A1 */
    pwm_signal_param_t pwmSignal[1];
    pwm_config_t pwmConfig;
   
  

    PWM_GetDefaultConfig(&pwmConfig);                    
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle; 
    pwmConfig.pairOperation = kPWM_Independent;      
    PWM_Init(g_pinDes->PWM, kPWM_Module_0, &pwmConfig);    

    pwmSignal[0].pwmChannel = kPWM_PwmA;             
    pwmSignal[0].level = kPWM_HighTrue;              
    pwmSignal[0].dutyCyclePercent = 50;             
        
   
    PWM_SetupPwm(g_pinDes->PWM, g_pinDes->pwm_submodule, pwmSignal, 1, kPWM_SignedCenterAligned, 1000, CLOCK_GetFreq(kCLOCK_IpgClk));        


    PWM_SetPwmLdok(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule) , true);    
    PWM_StartTimer(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule));        

    //turn off falut detection
    g_pinDes->PWM->SM[g_pinDes->pwm_submodule].DISMAP[0]=0;       
                           
    }

    
   CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03U */

   IOMUXC_SetPinMux(
      g_pinDes->FUN_PWM.muxRegister,       
      g_pinDes->FUN_PWM.muxMode,
      g_pinDes->FUN_PWM.inputRegister,
      g_pinDes->FUN_PWM.inputDaisy,
      g_pinDes->FUN_PWM.configRegister,
      0U);                                          /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      g_pinDes->FUN_PWM.muxRegister,       
      g_pinDes->FUN_PWM.muxMode,
      g_pinDes->FUN_PWM.inputRegister,
      g_pinDes->FUN_PWM.inputDaisy,
      g_pinDes->FUN_PWM.configRegister,      
      0x10B0u);                                 /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper */

    // update the dutyCyclePercent
    PWM_UpdatePwmDutycycle(g_pinDes->PWM,g_pinDes->pwm_submodule, g_pinDes->pwm_channel, kPWM_SignedCenterAligned, pwmVal); //更新占空比
    PWM_SetPwmLdok(g_pinDes->PWM, (1U << g_pinDes->pwm_submodule), true);   

}

#ifdef __cplusplus
}
#endif