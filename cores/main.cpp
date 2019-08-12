#include "Arduino.h"
#include "pins_arduino.h"
#include "SPI.h"


int main(void)
{
    init();
    Serial.begin(115200);
    Serial.println("begin");
    SPI.begin();
    SPI.beginTransaction(SPISettings(50000000, MSBFIRST, SPI_MODE0));
    while(1){
      SPI.transfer(0x74);
    }
    
    return 0;
}