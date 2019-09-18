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

extern "C" {
#include <string.h>
}

#include "Arduino.h"
#include "wiring_private.h"
#include "Wire.h"



TwoWire::TwoWire(LPI2C_Type * i2c, uint8_t pinSDA, uint8_t pinSCL)
{
  this->lpi2c = i2c;
  this->_uc_pinSDA=pinSDA;
  this->_uc_pinSCL=pinSCL;
  transmissionBegun = false;
}

void TwoWire::begin(void) {
  //Master Mode
  lpi2c_master_config_t lpi2c_config;
  

  CLOCK_SetMux(kCLOCK_Lpi2cMux,0);   //LPI2C clock source is PLL3=60Mhz
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv,5);   //Frequency division setting, frequency division value is 5+1, so LPI2C CLK=10Mhz
    
  uint32_t i2cclk=((CLOCK_GetFreq(kCLOCK_Usb1PllClk)/8)/(5+1)); 
    
  LPI2C_MasterGetDefaultConfig(&lpi2c_config);      
  lpi2c_config.baudRate_Hz = TWI_CLOCK;             //Default 

  LPI2C_MasterInit(lpi2c,&lpi2c_config,i2cclk);     

  // IO MUX
  pinPeripheral(_uc_pinSDA, 1U, FUN_I2C, 0xD8B0u);
  pinPeripheral(_uc_pinSCL, 1U, FUN_I2C, 0xD8B0u);

}


void TwoWire::begin(uint8_t address, bool enableGeneralCall) {
  //Slave mode
  lpi2c_slave_config_t slaveConfig   = {0};

  CLOCK_SetMux(kCLOCK_Lpi2cMux,0);   //LPI2C clock source is PLL3=60Mhz
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv,5);   //Frequency division setting, frequency division value is 5+1, so LPI2C CLK=10Mhz
    
  /* 1.Set up lpi2c slave first */
  /*
  * slaveConfig.address0 = 0U;
  * slaveConfig.address1 = 0U;
  * slaveConfig.addressMatchMode = kLPI2C_MatchAddress0;
  * slaveConfig.filterDozeEnable = true;
  * slaveConfig.filterEnable = true;
  * slaveConfig.enableGeneralCall = false;
  * slaveConfig.ignoreAck = false;
  * slaveConfig.enableReceivedAddressRead = false;
  * slaveConfig.sdaGlitchFilterWidth_ns = 0;
  * slaveConfig.sclGlitchFilterWidth_ns = 0;
  * slaveConfig.dataValidDelay_ns = 0;
  * slaveConfig.clockHoldTime_ns = 0;
  */
  LPI2C_SlaveGetDefaultConfig(&slaveConfig);
  slaveConfig.address0 = address;

  uint32_t i2cclk=((CLOCK_GetFreq(kCLOCK_Usb1PllClk)/8)/(5+1)); 

  LPI2C_SlaveInit(lpi2c, &slaveConfig, i2cclk);  

   // IO MUX
  pinPeripheral(_uc_pinSDA, 1U, FUN_I2C, 0xD8B0u);
  pinPeripheral(_uc_pinSCL, 1U, FUN_I2C, 0xD8B0u);

   if(LPI2C1 == lpi2c){
      EnableIRQ(LPI2C1_IRQn);
      NVIC_SetPriority(LPI2C1_IRQn, 5);
    }else if (LPI2C2 == lpi2c)
    {
      EnableIRQ(LPI2C2_IRQn);
      NVIC_SetPriority(LPI2C2_IRQn, 5);
    }else if (LPI2C3 == lpi2c)
    {
      EnableIRQ(LPI2C3_IRQn);
      NVIC_SetPriority(LPI2C3_IRQn, 5);
    }else if (LPI2C4 == lpi2c)
    {
      EnableIRQ(LPI2C4_IRQn);
      NVIC_SetPriority(LPI2C4_IRQn, 5);
    }

    LPI2C_SlaveEnableInterrupts(lpi2c, kLPI2C_SlaveRxReadyFlag | 
                                kLPI2C_SlaveRepeatedStartDetectFlag |
                                kLPI2C_SlaveAddressValidFlag | 
                                kLPI2C_SlaveStopDetectFlag |
                                kLPI2C_SlaveTxReadyFlag 
                                );                                             
}

void TwoWire::end() {
 
}

void TwoWire::setClock(uint32_t clock)
{
  uint32_t i2cclk=((CLOCK_GetFreq(kCLOCK_Usb1PllClk)/8)/(5+1)); 
    
  LPI2C_MasterEnable(lpi2c, false);
  LPI2C_MasterSetBaudRate(lpi2c, i2cclk, clock);
  LPI2C_MasterEnable(lpi2c, true);
  
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
  if(quantity == 0)
  {
    return 0;
  }

  rxBuffer.clear();

  /* Master start and send address to slave. */
  if(LPI2C_MasterStart(lpi2c, address, kLPI2C_Read) != kStatus_Success)
  {
    return 0;
  };

  if(LPI2C_MasterReceive(lpi2c, rxBuffer._aucBuffer, quantity) != kStatus_Success)
  {
    return 0;
  }
  if (stopBit)
  {
    LPI2C_MasterStop(lpi2c);
  }    

  rxBuffer._iHead = quantity;
  return rxBuffer._iHead;
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address) {
  // save address of target and clear buffer
  txAddress = address;
  txBuffer.clear();

  transmissionBegun = true;
}


uint8_t TwoWire::endTransmission(bool stopBit)
{
  transmissionBegun = false ;
 
  LPI2C_MasterClearStatusFlags(lpi2c, kLPI2C_MasterNackDetectFlag);
    
  if(LPI2C_MasterStart(lpi2c, txAddress, kLPI2C_Write) != kStatus_Success)
  {
    LPI2C_MasterStop(lpi2c);
    return 2;
  }

  delayMicroseconds(400); // wait 400 us for get a ack flag
    
  if(lpi2c->MSR & kLPI2C_MasterNackDetectFlag)
  {
    return 2;
  } 


  if(LPI2C_MasterSend(lpi2c, txBuffer._aucBuffer, txBuffer.available()) != kStatus_Success)
  {
    return 3;
  }

  if (stopBit)
  {
    LPI2C_MasterStop(lpi2c);
  }   

  return 0;
  
}

uint8_t TwoWire::endTransmission()
{
  return endTransmission(true);
}

size_t TwoWire::write(uint8_t ucData)
{
  // No writing, without begun transmission or a full buffer
  if ( !transmissionBegun || txBuffer.isFull() )
  {
    return 0 ;
  }

  txBuffer.store_char( ucData ) ;
  
  return 1 ;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  //Try to store all data
  for(size_t i = 0; i < quantity; ++i)
  {
    //Return the number of data stored, when the buffer is full (if write return 0)
    if(!write(data[i]))
      return i;
  }

  //All data stored
  return quantity;
}

int TwoWire::available(void)
{
  return rxBuffer.available();
}

int TwoWire::read(void)
{
  return rxBuffer.read_char();
}

int TwoWire::peek(void)
{
  return rxBuffer.peek();
}

void TwoWire::flush(void)
{
  // Do nothing, use endTransmission(..) to force
  // data transfer.
}

void TwoWire::onReceive(void(*function)(int))
{
  onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void))
{
  onRequestCallback = function;
}


void TwoWire::onService(void)
{
  size_t flags = 0U;
  flags = LPI2C_SlaveGetStatusFlags(lpi2c);

  if((flags & kLPI2C_SlaveStopDetectFlag) || 
     ((flags & kLPI2C_SlaveAddressValidFlag) && 
      (flags & kLPI2C_SlaveRepeatedStartDetectFlag) && 
      (flags & kLPI2C_SlaveTxReadyFlag))){

      LPI2C_SlaveTransmitAck(lpi2c, true);
      //Calling onReceiveCallback, if exists
      if(onReceiveCallback)
      {
        onReceiveCallback(available());
      }

      rxBuffer.clear();
    }else if(flags & kLPI2C_SlaveAddressValidFlag)
    {
      LPI2C_SlaveTransmitAck(lpi2c, true);

      transmissionBegun = true;

    //Calling onRequestCallback, if exists
      if(onRequestCallback)
      {
        onRequestCallback();
      }

    } else if(flags & kLPI2C_SlaveTxReadyFlag)
    {
        uint8_t c = 0xff;

        if( txBuffer.available() ) {
          c = txBuffer.read_char();
        }
        if(LPI2C_SlaveSend(lpi2c, &c, 1, 0) == kStatus_Success){
           transmissionBegun = true;
        }
    }else { //Received data
        if (rxBuffer.isFull()) {
          LPI2C_SlaveTransmitAck(lpi2c, false);
        } else {
          //Store data
          rxBuffer.store_char(lpi2c->SRDR);
          LPI2C_SlaveTransmitAck(lpi2c, true);
        }
      }
}

TwoWire Wire1(I2C1, I2C1_SDA, I2C1_SCL);
TwoWire Wire3(I2C3, I2C3_SDA, I2C3_SCL);

extern "C"{
void LPI2C1_IRQHandler(void){
  Wire1.onService();
}
void LPI2C3_IRQHandler(void){
  Wire3.onService();
}
}