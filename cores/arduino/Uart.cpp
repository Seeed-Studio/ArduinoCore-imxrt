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

#include "Uart.h"
#include "wiring_private.h"

UART Serial(LPUART1, LPUART1_IRQn, RX0, TX0);

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
void UART::LPUART_IRQHandel(void)
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
UART::UART(LPUART_Type * uart_num_ , IRQn_Type uart_num_IRQ, int rx_pin_, int tx_pin_)
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
void UART::begin(unsigned long baud, uint16_t config)
{
    init(baud, config);

    _rb = new RingBuffer();
    _rb->clear();
}

void UART::init(unsigned long baud, uint16_t config)
{
     // IO MUX
    pinPeripheral(_tx_pin, 0U, FUN_UART, 0x10B0U);
    pinPeripheral(_rx_pin, 0U, FUN_UART, 0x10B0U);
  
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
void UART::end() { delete _rb; }

/**
 * @description:
 * @param {type}
 * @return:
 */
int UART::available(void) { return _rb->available(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int UART::peek(void) { return _rb->peek(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int UART::read(void) {
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
void UART::flush(void) { _rb->clear(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
size_t UART::write(uint8_t c) { LPUART_WriteBlocking(_lpuart_num, &c, 1); return 1; }







