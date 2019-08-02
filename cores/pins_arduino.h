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
#pragma once
#ifndef PINS_ARDUINO_ARCH_MIX
#define PINS_ARDUINO_ARCH_MIX

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_common.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

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

#define RX0  J3_19
#define TX0  J3_20

typedef struct _MuxConfig
{
    uint32_t muxRegister;
    uint32_t muxMode;
    uint32_t inputRegister;
    uint32_t inputDaisy;
    uint32_t configRegister;
} MuxConfig;



typedef struct _PinDescription {
  GPIO_Type*  GROUP;
  uint32_t PIN;
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


/*
 * Pins descriptions
 */
const PinDescription g_APinDescription[]=
{
  {   //ENET_RXER GPIO_B1_11 J3_4 #0
      .GROUP = GPIO2,
      .PIN = 27,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27
      },
      .FUN_ENET = {
          IOMUXC_GPIO_B1_11_ENET_RX_ER
      }
  },
  {   //ENET_CRSDV GPIO_B1_06 J3_5 #1
      .GROUP = GPIO2,
      .PIN = 22,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_06_GPIO2_IO22
      },
      .FUN_ENET = {
          IOMUXC_GPIO_B1_06_ENET_RX_EN
      }
  },
  {   //ENET_MDIO GPIO_EMC_41 J3_6 #2
      .GROUP = GPIO3,
      .PIN = 27,
      .FUN_GPIO = {
          IOMUXC_GPIO_EMC_41_GPIO3_IO27
      },
      .FUN_ENET= {
          IOMUXC_GPIO_EMC_41_ENET_MDIO
      }
  },
  {   //ENET_MDC GPIO_EMC_40 J3_7 #3
      .GROUP = GPIO3,
      .PIN = 26,
      .FUN_GPIO = {
          IOMUXC_GPIO_EMC_40_GPIO3_IO26
      },
      .FUN_ENET= {
          IOMUXC_GPIO_EMC_40_ENET_MDC
      }
  },
   {   //ENET_RXD1 GPIO_B1_05 J3_8 #4
      .GROUP = GPIO2,
      .PIN = 21,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_05_ENET_RX_DATA01
      }
  },
  {   //ENET_RXD0 GPIO_B1_04 J3_9 #5
      .GROUP = GPIO2,
      .PIN = 21,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_04_GPIO2_IO20
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_04_ENET_RX_DATA00
      }
  },
   {   //ENET_TXD1 GPIO_B1_08 J3_10 #6
      .GROUP = GPIO2,
      .PIN = 24,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_08_GPIO2_IO24
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_08_ENET_TX_DATA01
      }
  },
   {   //ENET_TXD0 GPIO_B1_07 J3_11 #7
      .GROUP = GPIO2,
      .PIN = 23,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_07_GPIO2_IO23
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_07_ENET_TX_DATA00
      }
  },
   {   //ENET_TXEN GPIO_B1_09 J3_12 #8
      .GROUP = GPIO2,
      .PIN = 25,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_09_GPIO2_IO25
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_09_ENET_TX_EN
      }
  },
  {   //ENET_TX_CLK GPIO_B1_10 J3_13 #9
      .GROUP = GPIO2,
      .PIN = 26,
      .FUN_GPIO = {
          IOMUXC_GPIO_B1_10_FLEXIO2_FLEXIO26
      },
      .FUN_ENET= {
          IOMUXC_GPIO_B1_10_ENET_TX_CLK
      }
  },
  {   //CAN2_TX GPIO_AD_B0_14 J3_14 #10
      .GROUP = GPIO1,
      .PIN = 14,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_14_GPIO1_IO14
      },
      .FUN_CAN = {
          IOMUXC_GPIO_AD_B0_14_FLEXCAN2_TX
      }
  },
   {   //CAN2_RX GPIO_AD_B0_15 J3_15 #11
      .GROUP = GPIO1,
      .PIN = 15,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_15_GPIO1_IO15
      },
      .FUN_CAN = {
          IOMUXC_GPIO_AD_B0_15_FLEXCAN2_RX
      }
  },
   {   //I2C1_SCL GPIO_AD_B1_00 J3_16 #12
      .GROUP = GPIO1,
      .PIN = 16,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_00_GPIO1_IO16
      },
      .FUN_I2C = {
          IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
      }
  },
   {   //I2C1_SDL GPIO_AD_B1_01 J3_17 #13
      .GROUP = GPIO1,
      .PIN = 17,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_01_GPIO1_IO17
      },
      .FUN_I2C = {
          IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA
      }
  },
   {   //DBG_UART1_RXD GPIO_AD_B0_13 J3_19 #14
      .GROUP = GPIO1,
      .PIN = 13,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_13_GPIO1_IO13
      },
      .FUN_UART = {
          IOMUXC_GPIO_AD_B0_13_LPUART1_RX
      },
  },
   {   //DBG_UART1_TXD GPIO_AD_B0_12 J3_20 #15
      .GROUP = GPIO1,
      .PIN = 12,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_12_GPIO1_IO12
      },
      .FUN_UART = {
          IOMUXC_GPIO_AD_B0_12_LPUART1_TX
      }
  },
  {   //CSI_PIXCLK GPIO_AD_B1_04 J4_04 #16
      .GROUP = GPIO1,
      .PIN = 20,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_04_GPIO1_IO20
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_04_CSI_PIXCLK
      }
  },
   {   //CSI_MCLK GPIO_AD_B1_05 J4_05 #17
      .GROUP = GPIO1,
      .PIN = 21,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_05_GPIO1_IO21
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_05_CSI_MCLK
      }
  },
   {   //CSI_VSYNC GPIO_AD_B1_06 J4_06 #18
      .GROUP = GPIO1,
      .PIN = 22,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_06_GPIO1_IO22
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_06_CSI_VSYNC
      }
  },
  {   //CSI_HSYNC GPIO_AD_B1_07 J4_07 #19
      .GROUP = GPIO1,
      .PIN = 23,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_07_GPIO1_IO23
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_07_CSI_HSYNC
      }
  },
   {   //CSI_DATA08 GPIO_AD_B1_08 J4_08 #20
      .GROUP = GPIO1,
      .PIN = 24,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_08_GPIO1_IO24
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_08_CSI_DATA09
      }
  },
   {   //SAI1_MCLK CSI_DATA09 GPIO_AD_B1_09 J4_09 #21
      .GROUP = GPIO1,
      .PIN = 25,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_09_GPIO1_IO25
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_10_SAI1_RX_SYNC
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_09_CSI_DATA08
      }
  },
   {   //SAI1_RX_SYNC CSI_DATA07 GPIO_AD_B1_10 J4_10 #22
      .GROUP = GPIO1,
      .PIN = 26,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_10_GPIO1_IO26
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_10_SAI1_RX_SYNC
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_10_CSI_DATA07
      }
  },
   {   //SAI1_RX_BCLK CSI_DATA06 GPIO_AD_B1_11 J4_11 #23
      .GROUP = GPIO1,
      .PIN = 27,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_11_GPIO1_IO27
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_11_SAI1_RX_BCLK
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_11_CSI_DATA06
      }
  },
   {   //SAI1_RX_DATA00 CSI_DATA05 GPIO_AD_B1_12 J4_12 #24
      .GROUP = GPIO1,
      .PIN = 28,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_12_GPIO1_IO28
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_12_SAI1_RX_DATA00
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_12_CSI_DATA05
      }
  },
  {   //SAI1_TX_DATA00 CSI_DATA04 GPIO_AD_B1_13 J4_13 #25
      .GROUP = GPIO1,
      .PIN = 29,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_13_GPIO1_IO29
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_13_SAI1_TX_DATA00
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_13_CSI_DATA04
      }
  },
   {   //SAI1_TX_BCLK CSI_DATA03 GPIO_AD_B1_14 J4_14 #26
      .GROUP = GPIO1,
      .PIN = 30,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_14_GPIO1_IO30
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_14_SAI1_TX_BCLK
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_14_CSI_DATA03
      }
  },
  {   //SAI1_TX_SYNC CSI_DATA02 GPIO_AD_B1_15 J4_15 #27
      .GROUP = GPIO1,
      .PIN = 31,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_15_GPIO1_IO31
      },
      .FUN_SAI = {
          IOMUXC_GPIO_AD_B1_15_SAI1_TX_SYNC
      },
      .FUN_CSI= {
          IOMUXC_GPIO_AD_B1_15_CSI_DATA02
      }
  }, 
   {   //SPDIF_OUT GPIO_AD_B1_02 J4_16 #28
      .GROUP = GPIO1,
      .PIN = 18,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_02_GPIO1_IO18
      },
      .FUN_SPDIF = {
          IOMUXC_GPIO_AD_B1_02_SPDIF_OUT
      }
  },
   {   //SPDIF_IN GPIO_AD_B1_03 J4_17 #29
      .GROUP = GPIO1,
      .PIN = 19,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B1_03_GPIO1_IO19
      },
      .FUN_SPDIF = {
          IOMUXC_GPIO_AD_B1_03_SPDIF_IN
      }
  },
   {   //USER_LED_R GPIO_AD_B0_08 #30
      .GROUP = GPIO1,
      .PIN = 8,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_08_GPIO1_IO08
      }
  },
   {   //USER_LED_G GPIO_AD_B0_09  #31
      .GROUP = GPIO1,
      .PIN = 9,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_09_GPIO1_IO09
      }
  },
   {   //USER_LED_B GPIO_AD_B0_10  #32
      .GROUP = GPIO1,
      .PIN = 10,
      .FUN_GPIO = {
          IOMUXC_GPIO_AD_B0_10_GPIO1_IO10
      }
  }
};

#ifdef __cplusplus
}
#endif

#endif /*end of  PINS_ARDUINO_ARCH_MIX*/