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

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PinMux(x, y, z) IOMUXC_SetPinMux(g_APinDescription[x].y.muxRegister,\
                     g_APinDescription[x].y.muxMode,\
                     g_APinDescription[x].y.inputRegister,\
                     g_APinDescription[x].y.inputDaisy,\
                     g_APinDescription[x].y.configRegister,\
                     0U)
#define SetPinConfig(x, y, z) IOMUXC_SetPinConfig(g_APinDescription[x].y.muxRegister,\
                     g_APinDescription[x].y.muxMode,\
                     g_APinDescription[x].y.inputRegister,\
                     g_APinDescription[x].y.inputDaisy,\
                     g_APinDescription[x].y.configRegister,\
                     z)

#define pinPeripheral(pin, val, fun, config) PinMux(pin, fun, val); SetPinConfig(pin, fun, config)


#ifdef __cplusplus
} // extern "C"

#include "HardwareSerial.h"

#endif
