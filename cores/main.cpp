#include "Arduino.h"
#include "pins_arduino.h"

int main(void)
{
    init();
    pinMode(USER_LED_G,OUTPUT);

    while(1){
        digitalWrite(USER_LED_G, 0);
        delay(1000);
        digitalWrite(USER_LED_G, 1);
        delay(1000);
    }
    return 0;
}