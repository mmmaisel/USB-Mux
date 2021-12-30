/**********************************************************************\
 * USB-Mux
 *
 * System devices and error handlers
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
#include <stdint.h>

#include "dev/gpio.h"
#include "dev/rcc.h"
#include "pinout.h"

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

/**********************************************************************\
 * Fatal software errors
\**********************************************************************/

void error_handler() {
    using namespace dev;
    using namespace dev::rcc;

    RCC->AHB1ENR |= GPIOCEN;
    GPIOC->MODER |= MODE_LED_PWR;
    for(;;) {
        sleep_ms(500);
        GPIOC->set_odr(LED_PWR);
        sleep_ms(500);
        GPIOC->clear_odr(LED_PWR);
    }
}

/// @internal
/// Pure virtual function call error hook
extern "C" void __cxa_pure_virtual() {
    error_handler();
}

/**********************************************************************\
 * Error interrupts
\**********************************************************************/

extern "C" void fault_vector() __attribute__((naked));
extern "C" void fault_vector() {
    error_handler();
}

extern "C" void nmi_vector() __attribute__((naked));
extern "C" void nmi_vector() {
    error_handler();
}

extern "C" void default_vector() __attribute__((naked));
extern "C" void default_vector() {
}

