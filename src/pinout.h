/**********************************************************************\
 * USB-Mux
 *
 * Pinout difinitions
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
#pragma once

#include "types.h"

enum : WORD {
    // PORTA
    MUXDIS1 = (1 << 0),
    MUX1 = (1 << 1),
    MUXDIS0 = (1 << 2),
    MUX0 = (1 << 3),
    PWREN0 = (1 << 4),
    PWREN1 = (1 << 5),
    PWREN2 = (1 << 6),
    PWREN3 = (1 << 7),

    // PORTC
    LED_OUT0 = (1 << 0),
    LED_OUT1 = (1 << 1),
    LED_IN1 = (1 << 2),
    LED_IN0 = (1 << 3),
    LED_PWR = (1 << 10)
};

enum : WORD {
    // PORTA
    MODE_MUXDIS1 = (1 << (2*0)),
    MODE_MUX1 = (1 << (2*1)),
    MODE_MUXDIS0 = (1 << (2*2)),
    MODE_MUX0 = (1 << (2*3)),
    MODE_PWREN0 = (1 << (2*4)),
    MODE_PWREN1 = (1 << (2*5)),
    MODE_PWREN2 = (1 << (2*6)),
    MODE_PWREN3 = (1 << (2*7)),

    MODE_VBUS = (2 << (2*9)),
    MODE_DM = (2 << (2*11)),
    MODE_DP = (2 << (2*12)),

    // PORTC
    MODE_LED_OUT0 = (1 << (2*0)),
    MODE_LED_OUT1 = (1 << (2*1)),
    MODE_LED_IN1 = (1 << (2*2)),
    MODE_LED_IN0 = (1 << (2*3)),
    MODE_LED_PWR = (1 << (2*10))
};

enum : WORD {
    AFRH_VBUS = (10 << (4*(9-8))),
    AFRH_DM = (10 << (4*(11-8))),
    AFRH_DP = (10 << (4*(12-8)))
};

enum : WORD {
    PUPD_MUXDIS1 = (1 << (2*0)),
    PUPD_MUXDIS0 = (1 << (2*1))
};
