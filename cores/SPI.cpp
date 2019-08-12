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
#include "SPI.h"
#include <Arduino.h>


SPIClass::SPIClass(LPSPI_Type *lpspi, uint8_t uc_pinMISO, uint8_t uc_pinSCK, uint8_t uc_pinMOSI) : _settings(SPISettings(0, MSBFIRST, SPI_MODE0))
{
  initialized = false;
  assert(lpspi != NULL);
  _lpspi = lpspi;

  // pins
  _uc_pinMiso = uc_pinMISO;
  _uc_pinSCK = uc_pinSCK;
  _uc_pinMosi = uc_pinMOSI;
}

void SPIClass::begin()
{
  init();

  CLOCK_SetMux(kCLOCK_LpspiMux,1);   
  CLOCK_SetDiv(kCLOCK_LpspiDiv,5);    

    
  PinDescription const *g_MISOPinDes = &g_APinDescription[_uc_pinMiso];
  PinDescription const *g_MOSIPinDes = &g_APinDescription[_uc_pinMosi];
  PinDescription const *g_SCKPinDes =  &g_APinDescription[_uc_pinSCK];
    
  IOMUXC_SetPinMux( 
    g_MISOPinDes->FUN_SPI.muxRegister,       
    g_MISOPinDes->FUN_SPI.muxMode,
    g_MISOPinDes->FUN_SPI.inputRegister,
    g_MISOPinDes->FUN_SPI.inputDaisy,
    g_MISOPinDes->FUN_SPI.configRegister, 0);                                    
  IOMUXC_SetPinMux(
    g_MOSIPinDes->FUN_SPI.muxRegister,       
    g_MOSIPinDes->FUN_SPI.muxMode,
    g_MOSIPinDes->FUN_SPI.inputRegister,
    g_MOSIPinDes->FUN_SPI.inputDaisy,
    g_MOSIPinDes->FUN_SPI.configRegister, 0);                                    
  IOMUXC_SetPinMux(
    g_SCKPinDes->FUN_SPI.muxRegister,       
    g_SCKPinDes->FUN_SPI.muxMode,
    g_SCKPinDes->FUN_SPI.inputRegister,
    g_SCKPinDes->FUN_SPI.inputDaisy,
    g_SCKPinDes->FUN_SPI.configRegister, 0);        

  IOMUXC_SetPinConfig( 
    g_MISOPinDes->FUN_SPI.muxRegister,       
    g_MISOPinDes->FUN_SPI.muxMode,
    g_MISOPinDes->FUN_SPI.inputRegister,
    g_MISOPinDes->FUN_SPI.inputDaisy,
    g_MISOPinDes->FUN_SPI.configRegister, 0x10B0);                                    
  IOMUXC_SetPinConfig(
    g_MOSIPinDes->FUN_SPI.muxRegister,       
    g_MOSIPinDes->FUN_SPI.muxMode,
    g_MOSIPinDes->FUN_SPI.inputRegister,
    g_MOSIPinDes->FUN_SPI.inputDaisy,
    g_MOSIPinDes->FUN_SPI.configRegister, 0x10B0);                                    
  IOMUXC_SetPinConfig(
    g_SCKPinDes->FUN_SPI.muxRegister,       
    g_SCKPinDes->FUN_SPI.muxMode,
    g_SCKPinDes->FUN_SPI.inputRegister,
    g_SCKPinDes->FUN_SPI.inputDaisy,
    g_SCKPinDes->FUN_SPI.configRegister, 0x10B0);                                      

    config(DEFAULT_SPI_SETTINGS);
}

void SPIClass::init()
{
  if (initialized)
    return;
 
  initialized = true;
}

void SPIClass::config(SPISettings settings)
{
    this->_settings = settings;
    Serial.println(_settings.clockFreq);
    lpspi_master_config_t lpspi_config; 

    LPSPI_Deinit(_lpspi);
  
    uint32_t lpspiclk=(CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk)/(5+1));

    lpspi_config.baudRate=_settings.clockFreq;              
    lpspi_config.whichPcs=kLPSPI_Pcs1;                
    lpspi_config.pcsActiveHighOrLow=kLPSPI_PcsActiveLow;
    lpspi_config.bitsPerFrame=8;                      
    lpspi_config.cpol=spiMode[_settings.dataMode]._CPOL;   
    lpspi_config.cpha=spiMode[_settings.dataMode]._CPHA;;  
    lpspi_config.direction=_settings.bitOrder;          
    
    lpspi_config.pinCfg=kLPSPI_SdiInSdoOut;            
    lpspi_config.dataOutConfig=kLpspiDataOutRetained; 

    lpspi_config.pcsToSckDelayInNanoSec        = 10;
    lpspi_config.lastSckToPcsDelayInNanoSec    = 10;
    lpspi_config.betweenTransferDelayInNanoSec = 10;

    LPSPI_MasterInit(_lpspi, &lpspi_config, lpspiclk);
  
    LPSPI_Enable(_lpspi, true);                        
}

void SPIClass::end()
{
 
}

void SPIClass::beginTransaction(SPISettings settings)
{
    this->_settings = settings;
    
    config(this->_settings);
}

void SPIClass::endTransaction(void)
{

}

void SPIClass::setBitOrder(BitOrder order)
{
    this->_settings.bitOrder = (order == MSBFIRST ? kLPSPI_MsbFirst : kLPSPI_LsbFirst);
    config(this->_settings);
    
}

void SPIClass::setDataMode(uint8_t mode)
{
    this->_settings.dataMode = mode;
    config(this->_settings);
}

void SPIClass::setClock(uint32_t clock)
{
   this->_settings.clockFreq = clock;
   config(this->_settings);
}

byte SPIClass::transfer(uint8_t data)
{
    uint8_t spirxdata=0;
    uint8_t spitxdata=data;
    lpspi_transfer_t spi_tranxfer;
    
    spi_tranxfer.configFlags=kLPSPI_MasterPcs1|kLPSPI_MasterPcsContinuous;   
    spi_tranxfer.txData=&spitxdata;               
    spi_tranxfer.rxData=&spirxdata;                
    spi_tranxfer.dataSize=1;                        
    LPSPI_MasterTransferBlocking(_lpspi, &spi_tranxfer);	   					    
    return spirxdata;
}

uint16_t SPIClass::transfer16(uint16_t data) {
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;

  t.val = data;

  if (_settings.bitOrder == kLPSPI_LsbFirst) {
    t.lsb = transfer(t.lsb);
    t.msb = transfer(t.msb);
  } else {
    t.msb = transfer(t.msb);
    t.lsb = transfer(t.lsb);
  }

  return t.val;
}

void SPIClass::transfer(void *buf, size_t count)
{
  uint8_t *buffer = reinterpret_cast<uint8_t *>(buf);
  for (size_t i=0; i<count; i++) {
    *buffer = transfer(*buffer);
    buffer++;
  }
}

void SPIClass::attachInterrupt() {
  // Should be enableInterrupt()
}

void SPIClass::detachInterrupt() {
  // Should be disableInterrupt()
}

SPIClass SPI3(LPSPI3, SPI3_MOSI, SPI3_SCK, SPI3_MISO);
