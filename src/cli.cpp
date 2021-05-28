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
#include "cli.h"
#include "usb_mux.h"

#include <cstring>

BYTE Cli::m_buffer[Cli::BUFFER_SIZE] = {0};
BYTE Cli::m_pos = 0;

USHORT Cli::Parse(BYTE* pData, USHORT src_len) {
    USHORT available_size = BUFFER_SIZE - m_pos;
    USHORT len = src_len > available_size ? available_size : src_len;

    memcpy(m_buffer + m_pos, pData, len);
    m_pos += len;

    if(m_pos == BUFFER_SIZE) {
        m_pos = 0;
        return 0;
    } else if(m_pos > 1 && m_buffer[m_pos-1] == '\r') {
        // XXX: suplication and magic number (strlen) hell
        // XXX: Correctly send VT100 control sequences
        if(m_buffer[m_pos-2] == '0') {
            UsbMux::mux(0);
            memcpy(pData, "\n\rSelected: 0\n", 14);
            m_pos = 0;
            return 14;
        } else if(m_buffer[m_pos-2] == '1') {
            UsbMux::mux(1);
            memcpy(pData, "\n\rSelected: 1\n", 14);
            m_pos = 0;
            return 14;
        } else if(m_buffer[m_pos-2] == '2') {
            UsbMux::mux(2);
            memcpy(pData, "\n\rSelected: 2\n", 14);
            m_pos = 0;
            return 14;
        } else if(m_buffer[m_pos-2] == '3') {
            UsbMux::mux(3);
            memcpy(pData, "\n\rSelected: 3\n", 14);
            m_pos = 0;
            return 14;
        } else if(m_buffer[m_pos-2] == '4') {
            UsbMux::mux(4);
            memcpy(pData, "\n\rSelected: 4\n", 14);
            m_pos = 0;
            return 14;
        } else {
            memcpy(pData, "\n\rMux: ", 7);
            m_pos = 0;
            return 7;
        }
    }
    return 0;
}
