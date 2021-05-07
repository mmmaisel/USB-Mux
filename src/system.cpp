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

/**********************************************************************\
 * Fatal software errors
\**********************************************************************/

/// @internal
/// Pure virtual function call error hook
extern "C" void __cxa_pure_virtual() {
    for(;;);
}

/**********************************************************************\
 * Error interrupts
\**********************************************************************/

extern "C" void fault_vector() __attribute__((naked));
extern "C" void fault_vector() {
    for(;;);
}

extern "C" void nmi_vector() __attribute__((naked));
extern "C" void nmi_vector() {
    for(;;);
}

extern "C" void default_vector() __attribute__((naked));
extern "C" void default_vector() {
    for(;;);
}

