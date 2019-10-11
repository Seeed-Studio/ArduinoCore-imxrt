
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

#include "Arduino.h"
#include <inttypes.h>
#include "Stream.h"
#include "RingBuffer.h"
#include "fsl_lpuart.h"
#include "board.h"
#include "system_MIMXRT1052.h"
#include "HardwareSerial.h"

class USBCDC : public HardwareSerial
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
    operator bool() { return true; }
    void CDC_IRQHandel(void);
    USBCDC();
    ~USBCDC(){};
  private:
    void init(unsigned long baud, uint16_t config);
    RingBufferN<512> *_rb;
};

extern USBCDC SerialUSB;