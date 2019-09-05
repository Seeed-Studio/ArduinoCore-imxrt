/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _BOARD_DRIVER_LED_H
#define _BOARD_DRIVER_LED_H


#if defined(BOARD_LED_PORT)
inline void LED_init(void) {  }
inline void LED_on(void) { }
inline void LED_off(void) { }
inline void LED_toggle(void) {  }
#else
inline void LED_init(void) { }
inline void LED_on(void) { }
inline void LED_off(void) { }
inline void LED_toggle(void) { }
#endif
void LED_pulse();

#if defined(BOARD_LEDRX_PORT)
inline void LEDRX_init(void) {  }
inline void LEDRX_on(void) {  }
inline void LEDRX_off(void) {  }
inline void LEDRX_toggle(void) {  }
#else
inline void LEDRX_init(void) { }
inline void LEDRX_on(void) { }
inline void LEDRX_off(void) { }
inline void LEDRX_toggle(void) { }
#endif

#if defined(BOARD_LEDTX_PORT)
inline void LEDTX_init(void) {  }
inline void LEDTX_on(void) {  }
inline void LEDTX_off(void) { }
inline void LEDTX_toggle(void) { }
#else
inline void LEDTX_init(void) { }
inline void LEDTX_on(void) { }
inline void LEDTX_off(void) { }
inline void LEDTX_toggle(void) { }
#endif

#endif // _BOARD_DRIVER_LED_
