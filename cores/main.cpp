#include "Arduino.h"


int main(void)
{
    init();
    pinMode(1,OUTPUT);

    while(1){
        digitalRead(0);
        delay(1000);
        digitalWrite(0,0);
        delay(1000);
    }
    return 0;
}