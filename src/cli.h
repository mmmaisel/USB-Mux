/*********************************************************************\
 * USB-Mux
 *
 * Command Line Interface
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

class Cli {
    // Static class
    Cli() = delete;
    Cli(const Cli&) = delete;
    Cli(Cli&&) = delete;
    ~Cli() = delete;

    public:
        static USHORT Parse(BYTE* pData, USHORT len);

    private:
        static const int BUFFER_SIZE = 32;
        static BYTE m_buffer[BUFFER_SIZE];
        static BYTE m_pos;
};
