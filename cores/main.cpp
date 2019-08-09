#include "Arduino.h"
#include "pins_arduino.h"
#include "Wire.h"


int main(void)
{
    init();
    Serial.begin(115200);
    Serial.println("begin");
    Wire.begin();
    Wire3.begin(0x18);
    uint8_t data = 0;
    while (1)
    {
    Wire.beginTransmission(0x18);
    Wire.write(0x0F);
    Wire.endTransmission();

    Wire.requestFrom((int16_t)0x18, 1);
    while(Wire.available())
    {
        data =Wire.read();
    }
      Serial.print("data: ");
      Serial.println(data);
    }
    
    return 0;
}