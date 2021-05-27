/**********************************************************************\
 * USB-Mux
 *
 * USB Control Endpoint class
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

class ControlEndpoint : public USBEndpoint {
    public:
        ControlEndpoint();
        ControlEndpoint(const ControlEndpoint&) = delete;
        ControlEndpoint(ControlEndpoint&&) = delete;
        virtual ~ControlEndpoint();

    protected:
        virtual void OnReceive(USHORT len) override;
        virtual void OnSetup(USHORT len) override;
        virtual void OnTransmit() override;
        virtual void OnRxData(WORD data) override; // TODO: this is inefficient and bad

    private:
        static const int BUFFER_SIZE = 32;
        union
        {
            BYTE  b[BUFFER_SIZE];
            HWORD h[BUFFER_SIZE/2];
            WORD  w[BUFFER_SIZE/4];
        } m_buffer;
        BYTE m_bufferPos;
};
extern ControlEndpoint ep0;
