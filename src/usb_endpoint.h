/**********************************************************************\
 * USB-Mux
 *
 * USB Endpoint class
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
#pragma once

#include "types.h"
class USBPhy;

class USBEndpoint {
    friend class USBPhy;
    public:
        enum { DIR_IN = 1, DIR_OUT = 2};

        USBEndpoint(BYTE epnum);
        USBEndpoint(const USBEndpoint&) = delete;
        USBEndpoint(USBEndpoint&&) = delete;
        virtual ~USBEndpoint();

        void Enable(BYTE dir);

        void Transmit(const WORD* pData, USHORT len);
        void Receive(WORD* pData, USHORT len);

        BYTE TXFIFOEmpty();

    protected:
        virtual void OnReceive(USHORT len);
        virtual void OnSetup(USHORT len);
        virtual void OnTransmit();
        virtual void OnRxData(WORD data);

        static void operator delete(void* __attribute__((unused)));

        BYTE m_epnum;
        // TODO: RX buffer

    private:
};

extern USBEndpoint* eps[];
