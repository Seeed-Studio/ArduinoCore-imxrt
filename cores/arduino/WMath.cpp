#include "Common.h"
extern "C" {
  #include "stdlib.h"
  #include "stdint.h"
}

void randomSeed( uint32_t dwSeed )
{
  if ( dwSeed != 0 )
  {
    srand( dwSeed ) ;
  }
}

long random( long howbig )
{
  if ( howbig == 0 )
  {
    return 0 ;
  }

  return rand() % howbig;
}

long random( long howsmall, long howbig )
{
  if (howsmall >= howbig)
  {
    return howsmall;
  }

  long diff = howbig - howsmall;

  return random(diff) + howsmall;
}

/* C++ prototypes */
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }