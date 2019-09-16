/*
  Copyright (c) 2016 Arduino LLC.  All right reserved.

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

#pragma once
#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"
#include "RingBuffer.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "system_MIMXRT1052.h"


/*  
MSB (STOPBit[][])(DATA[][])(PARIRT[][][][])LSB
 */

#define SERIAL_PARITY_EVEN    (kLPUART_ParityEven & 0x0F)
#define SERIAL_PARITY_ODD     (kLPUART_ParityOdd & 0x0F)
#define SERIAL_PARITY_NONE    (kLPUART_ParityDisabled & 0x0F)


#define SERIAL_STOP_BIT_1    ((kLPUART_OneStopBit << 4) & 0x30)
#define SERIAL_STOP_BIT_2    ((kLPUART_TwoStopBit << 4) & 0x30)


#define SERIAL_DATA_7        (kLPUART_SevenDataBits << 6) & 0xC0)
#define SERIAL_DATA_8        ((kLPUART_EightDataBits << 6) & 0xC0)


#define SERIAL_7N1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_7N2          (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_7)
#define SERIAL_8N1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)
#define SERIAL_8N2          (SERIAL_STOP_BIT_2 | SERIAL_PARITY_NONE  | SERIAL_DATA_8)

#define SERIAL_7E1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_7E2          (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_7)
#define SERIAL_8E1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)
#define SERIAL_8E2          (SERIAL_STOP_BIT_2 | SERIAL_PARITY_EVEN  | SERIAL_DATA_8)

#define SERIAL_7O1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_7O2          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_7)
#define SERIAL_8O1          (SERIAL_STOP_BIT_1 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)
#define SERIAL_8O2          (SERIAL_STOP_BIT_2 | SERIAL_PARITY_ODD   | SERIAL_DATA_8)


class HardwareSerial : public Stream
{
  public:
    void begin(unsigned long baudrate, uint16_t config = SERIAL_8N1);
    void end();
    int available(void);
    int peek(void);
    int read(void);
    void flush(void);
    size_t write(uint8_t);
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() const;
    void LPUART_IRQHandel(void);
    HardwareSerial(LPUART_Type * uart_num_ , IRQn_Type uart_num_IRQ, int rx_pin_, int tx_pin_);
    ~HardwareSerial(){};
  private:
    void init(unsigned long baud, uint16_t config);
    RingBuffer *_rb;
    size_t read_timeout_ = 1000;
    LPUART_Type * _lpuart_num;
    IRQn_Type _lpuart_num_IRQ;
    lpuart_handle_t g_lpuartHandle;
    int _rx_pin;
    int _tx_pin;

};

// XXX: Are we keeping the serialEvent API?
extern void serialEventRun(void) __attribute__((weak));

extern HardwareSerial Serial;