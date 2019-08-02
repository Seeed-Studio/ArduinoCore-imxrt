#include "Arduino.h"
#include "pins_arduino.h"

int main(void)
{
    init();
    pinMode(USER_LED_G,OUTPUT);
    Serial.begin(115200);

    while(1){
      while(Serial.available()){
          Serial.write(Serial.read());
      }
    }
    return 0;
}