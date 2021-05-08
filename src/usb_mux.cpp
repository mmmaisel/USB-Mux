/**********************************************************************\
 * USB-Mux
 *
 * USB-Mux class
 **********************************************************************
 * Copyright (C) 2021 - Max Maisel
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
#include "usb_mux.h"
#include "pinout.h"
#include "system.h"

#include "dev/gpio.h"
#include "dev/rcc.h"

using namespace dev;
using namespace dev::rcc;

void UsbMux::initialize() {
    RCC->AHB1ENR |= GPIOAEN | GPIOCEN;

    GPIOA->PUPDR |= PUPD_MUXDIS0 | PUPD_MUXDIS1;
    GPIOA->MODER |= MODE_MUX0 | MODE_MUX1 | MODE_MUXDIS0 | MODE_MUXDIS1 |
        MODE_PWREN0 | MODE_PWREN1 | MODE_PWREN2 | MODE_PWREN3;
    GPIOC->MODER |= MODE_LED_OUT0 | MODE_LED_OUT1 |
        MODE_LED_IN0 | MODE_LED_IN1 | MODE_LED_PWR;

    disable_mux();
    GPIOC->set_odr(LED_PWR);
}

void UsbMux::mux(BYTE pos) {
    disable_mux();
    switch(pos) {
        case 1:
            GPIOA->set_odr(PWREN0 | PWREN2);
            sleep_ms(1);
            GPIOA->clear_odr(MUX0);
            GPIOA->set_odr(MUX1);
            sleep_ms(1);
            GPIOA->clear_odr(MUXDIS0 | MUXDIS1);
            GPIOC->set_odr(LED_IN0 | LED_OUT0);
            break;
        case 2:
            GPIOA->set_odr(PWREN0 | PWREN3);
            sleep_ms(1);
            GPIOA->clear_odr(MUX0 | MUX1);
            sleep_ms(1);
            GPIOA->clear_odr(MUXDIS0 | MUXDIS1);
            GPIOC->set_odr(LED_IN0 | LED_OUT1);
            break;
        case 3:
            GPIOA->set_odr(PWREN1 | PWREN2);
            sleep_ms(1);
            GPIOA->set_odr(MUX0 | MUX1);
            sleep_ms(1);
            GPIOA->clear_odr(MUXDIS0 | MUXDIS1);
            GPIOC->set_odr(LED_IN1 | LED_OUT0);
            break;
        case 4:
            GPIOA->set_odr(PWREN1 | PWREN3);
            sleep_ms(1);
            GPIOA->set_odr(MUX1);
            GPIOA->clear_odr(MUX0);
            sleep_ms(1);
            GPIOA->clear_odr(MUXDIS0 | MUXDIS1);
            GPIOC->set_odr(LED_IN1 | LED_OUT1);
            break;
        default:
            break;
    };
}

void UsbMux::disable_mux() {
    GPIOA->set_odr(MUXDIS0 | MUXDIS1);
    sleep_ms(1);
    GPIOA->clear_odr(
        MUX0 | MUX1 | PWREN0 | PWREN1 | PWREN2 | PWREN3);
    GPIOC->clear_odr(LED_OUT0 | LED_OUT1 | LED_IN0 | LED_IN1);
}
