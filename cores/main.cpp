#include "Arduino.h"
#include "pins_arduino.h"

int main(void)
{
    init();
    pinMode(USER_LED_G,OUTPUT);
    pinMode(J3_4,OUTPUT);
    Serial.begin(115200);
    analogReadResolution(10);
    while(1){
      while(Serial.available()){
          Serial.write(Serial.read());
      }
      Serial.println("AD2_9");
      Serial.println(analogRead(AD2_9));
       Serial.println("AD1_3");
      Serial.println(analogRead(AD1_3));
      delay(100);
    }
    return 0;
}