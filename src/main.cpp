/**********************************************************************\
 * USB-Mux
 *
 * C++ main function
 *  - please note that the real program starts in startup.cpp
 **********************************************************************
 * Copyright (C) 2019-2021 - Max Maisel
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
\**********************************************************************/
/// @file
#include <string.h>
#include <stdlib.h>

#include "dev/rcc.h"
#include "dev/gpio.h"

void sleep_ms(ULONG ms) {
    volatile ULONG cycles = 48000 * ms / 4; // Sysclock: 48 MHz
    asm (
        "1:\n"
        "  subs %0, %0, #1\n" // 1 cycle
        "  nop\n"             // 1 cycle
        "  bne 1b\n"          // 1 + P cycles (here: pipeline == 1)
        : : "r"(cycles) :
    );
}

void led_init() {
    dev::RCC->AHB1ENR |= dev::rcc::GPIOCEN;
    dev::GPIOC->MODER |= (1 << (2*0)) | (1 << (2*1)) | (1 << (2*2)) | (1 << (2*3)) | (1 << (2*10));
    dev::GPIOC->clear_odr((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 10));
}

/// C++ main function, program starts here.
void main() __attribute__((noreturn));
void main() {
    led_init();
    for(;;) {
        dev::GPIOC->clear_odr((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 10));
        sleep_ms(500);
        dev::GPIOC->set_odr((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 10));
        sleep_ms(500);
    }
}
