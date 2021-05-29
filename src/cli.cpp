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

BYTE Cli::m_lastChar = '\r';

static const char MUX[] = "Mux: ";
static const char SELECTED[] = "Selected: x\r\n";
static const char ERROR[] = "Error!\r\n";

USHORT Cli::Process(
    BYTE* pDataIn, USHORT len_in, BYTE* pDataOut, USHORT len_out)
{
    TINY mux;
    USHORT resp_len = 0;
    USHORT available_len = len_out;
    for(USHORT i = 0; i < len_in; ++i) {
        if(pDataIn[i] == '\r') {
            if(available_len >= 2) {
                pDataOut[resp_len++] = '\r';
                pDataOut[resp_len++] = '\n';
                available_len -= 2;
            }

            mux = m_lastChar - '0';
            if(mux >= 0 && mux <= 4 && available_len >= sizeof(SELECTED)) {
                UsbMux::mux(mux);
                memcpy(pDataOut + resp_len, SELECTED, sizeof(SELECTED));
                pDataOut[sizeof(SELECTED) + resp_len - 4] = m_lastChar;
                resp_len += sizeof(SELECTED);
                available_len -= sizeof(SELECTED);
            } else if(m_lastChar != '\r' && available_len >= sizeof(ERROR)) {
                memcpy(pDataOut + resp_len, ERROR, sizeof(ERROR));
                resp_len += sizeof(ERROR);
                available_len -= sizeof(ERROR);
            }

            if(available_len >= sizeof(MUX)) {
                memcpy(pDataOut + resp_len, MUX, sizeof(MUX));
                resp_len += sizeof(MUX);
                available_len -= sizeof(MUX);
            }
            m_lastChar = '\r';
        } else if(available_len >= 1) {
            m_lastChar = pDataIn[i];
            pDataOut[resp_len++] = pDataIn[i];
            --available_len;
        }
    }
    return resp_len;
}
