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

#include "HardwareSerial.h"

HardwareSerial Serial(LPUART1, LPUART1_IRQn, RX0, TX0);

#define LPUART_CLK_FREQ BOARD_DebugConsoleSrcFreq()

extern "C"{  // IRQHandler must be compiled in C

void LPUART1_IRQHandler(void)
{
    Serial.LPUART_IRQHandel();
}

}

/**
 * @description:
 * @param {type}
 * @return:
 */
void HardwareSerial::LPUART_IRQHandel(void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(_lpuart_num))
    {
        data = LPUART_ReadByte(_lpuart_num);
        _rb->store_char(data);
    }

}

/**
 * @description:
 * @param {type}
 * @return:
 */
HardwareSerial::HardwareSerial(LPUART_Type * uart_num_ , IRQn_Type uart_num_IRQ, int rx_pin_, int tx_pin_)
{
    _lpuart_num = uart_num_;
    _lpuart_num_IRQ = uart_num_IRQ;
    _rx_pin = rx_pin_;
    _tx_pin = tx_pin_;
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void HardwareSerial::begin(unsigned long baud, uint16_t config)
{
    init(baud, config);

    _rb = new RingBuffer();
    _rb->clear();
}

void HardwareSerial::init(unsigned long baud, uint16_t config)
{
     // IO MUX
  CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */

  IOMUXC_SetPinMux(
      g_APinDescription[_rx_pin].FUN_UART.muxRegister,
      g_APinDescription[_rx_pin].FUN_UART.muxMode,           
      g_APinDescription[_rx_pin].FUN_UART.inputRegister, 
      g_APinDescription[_rx_pin].FUN_UART.inputDaisy,   
      g_APinDescription[_rx_pin].FUN_UART.configRegister, 
      0U);                                   /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      g_APinDescription[_tx_pin].FUN_UART.muxRegister,
      g_APinDescription[_tx_pin].FUN_UART.muxMode,           
      g_APinDescription[_tx_pin].FUN_UART.inputRegister, 
      g_APinDescription[_tx_pin].FUN_UART.inputDaisy,   
      g_APinDescription[_tx_pin].FUN_UART.configRegister,       
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinConfig(
      g_APinDescription[_rx_pin].FUN_UART.muxRegister,
      g_APinDescription[_rx_pin].FUN_UART.muxMode,           
      g_APinDescription[_rx_pin].FUN_UART.inputRegister, 
      g_APinDescription[_rx_pin].FUN_UART.inputDaisy,   
      g_APinDescription[_rx_pin].FUN_UART.configRegister,        
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */
  IOMUXC_SetPinConfig(
      g_APinDescription[_tx_pin].FUN_UART.muxRegister,
      g_APinDescription[_tx_pin].FUN_UART.muxMode,           
      g_APinDescription[_tx_pin].FUN_UART.inputRegister, 
      g_APinDescription[_tx_pin].FUN_UART.inputDaisy,   
      g_APinDescription[_tx_pin].FUN_UART.configRegister,         
      0x10B0u);                               /* Slew Rate Field: Slow Slew Rate
                                                 Drive Strength Field: R0/6
                                                 Speed Field: medium(100MHz)
                                                 Open Drain Enable Field: Open Drain Disabled
                                                 Pull / Keep Enable Field: Pull/Keeper Enabled
                                                 Pull / Keep Select Field: Keeper
                                                 Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                 Hyst. Enable Field: Hysteresis Disabled */

    lpuart_config_t _config;
    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&_config);
    _config.baudRate_Bps  = baud;
    _config.parityMode    = (lpuart_parity_mode_t)(config & 0x0F);
    _config.dataBitsCount = (lpuart_data_bits_t)(config & 0x30);
    _config.stopBitCount  = (lpuart_stop_bit_count_t)(config & 0xC0);
    _config.enableTx      = true;
    _config.enableRx      = true;
    

    LPUART_Init(_lpuart_num, &_config, LPUART_CLK_FREQ);

    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(_lpuart_num, kLPUART_RxDataRegFullInterruptEnable);
    EnableIRQ(_lpuart_num_IRQ);
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void HardwareSerial::end() { delete _rb; }

/**
 * @description:
 * @param {type}
 * @return:
 */
int HardwareSerial::available(void) { return _rb->available(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int HardwareSerial::peek(void) { return _rb->peek(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int HardwareSerial::read(void) {
    while (true) {
        if (0 != _rb->available()) {
            return _rb->read_char();
        } else {
            return -1;
        }
    }
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void HardwareSerial::flush(void) { _rb->clear(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
size_t HardwareSerial::write(uint8_t c) { LPUART_WriteBlocking(_lpuart_num, &c, 1); return 1; }

/**
 * @description:
 * @param {type}
 * @return:
 */
HardwareSerial::operator bool() const { return true; }





