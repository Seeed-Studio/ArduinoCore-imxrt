#ifdef ARDUINO_MAIN

#include "Arduino.h"

void  __attribute__((weak)) setup() {

}

void  __attribute__((weak)) loop() {

}

int  __attribute__((weak)) main()
{
    init();
    setup();
    for (;;)
    {
        loop();
    }
}

#endif
