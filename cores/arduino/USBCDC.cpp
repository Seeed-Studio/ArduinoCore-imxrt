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

#include "USBCDC.h"
#include "wiring_private.h"
#include "virtual_com.h"

USBCDC SerialUSB;


void CDC_IRQHandel()
{
    SerialUSB.CDC_IRQHandel();
}

uint32_t CDC_available()
{
    return SerialUSB.available();
}


void USBCDC::CDC_IRQHandel(void)
{
   uint8_t data[64];
   uint32_t length; 
   length = vcom_get_recBuf(data);
   for(uint8_t i = 0; i < length; i++)
   {
       Serial.write(data[i]);
       _rb->store_char(data[i]);
   }
}

/**
 * @description:
 * @param {type}
 * @return:
 */
USBCDC::USBCDC()
{

}

/**
 * @description:
 * @param {type}
 * @return:
 */
void USBCDC::begin(unsigned long baud, uint16_t config)
{
    _rb = new RingBufferN<512>();
    _rb->clear();
    init(baud, config);
}

void USBCDC::init(unsigned long baud, uint16_t config)
{
     vcom_read(_rb->availableForStore());
}

/**
 * @description:
 * @param {type}
 * @return:
 */
void USBCDC::end() { delete _rb; }

/**
 * @description:
 * @param {type}
 * @return:
 */
int USBCDC::available(void) { return _rb->available(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int USBCDC::peek(void) { return _rb->peek(); }

/**
 * @description:
 * @param {type}
 * @return:
 */
int USBCDC::read(void) {
    int temp;
    while (true) {
        if (0 != _rb->available()) {
            temp = _rb->read_char();
            vcom_read(_rb->availableForStore());
            return temp;
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
void USBCDC::flush(void) { _rb->clear(); vcom_read(_rb->availableForStore());}

/**
 * @description:
 * @param {type}
 * @return:
 */
size_t USBCDC::write(uint8_t c) { vcom_write_buf(&c, 1); return 1;}






