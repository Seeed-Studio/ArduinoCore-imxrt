#include "Arduino.h"
#include "pins_arduino.h"


int main(void)
{
    init();
    Serial.begin(115200);
    uint16_t value = 0;
    uint8_t status = 0;
    Serial.println("begin");
    
     
    while (1)
    {
      
      analogWrite(PWM4_0, value);
      if(status){
        analogWrite(PWM4_1, 255);
      }else
      {
        analogWrite(PWM4_1, 0);
      }
      
      if(value > 255){
        value = 0;
        status ^= 1;

      }
      value += 1;
      delay(10);
      Serial.print(analogRead(AD1_3));Serial.print("\t");
      Serial.print(analogRead(AD2_9));Serial.print("\n");
    }
    
    return 0;
  
}