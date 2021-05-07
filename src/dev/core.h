/**********************************************************************\
 * USB-Mux
 *
 * ARM M4 core register map
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

#include "../types.h"

namespace dev {
    struct ScbStruct {
        WORD CPUID;
        WORD ICSR;
        WORD VTOR;
        WORD AIRCR;
        WORD SCR;
        WORD CCR;
        BYTE SHP[12];
        WORD SHCSR;
        WORD CFSR;
        WORD HFSR;
        WORD DFSR;
        WORD MMFAR;
        WORD BFAR;
        WORD AFSR;
        WORD PFR[2];
        WORD DFR;
        WORD ADR;
        WORD MMFR[4];
        WORD ISAR[5];
        WORD _reserved0[5];
        WORD CPACR;
    };

    volatile ScbStruct* const SCB = (volatile ScbStruct*)0xE000ED00;

    enum : WORD {
        CP10_ACC_FULL = 0x00300000,
        CP11_ACC_FULL = 0x00C00000
    };
}
