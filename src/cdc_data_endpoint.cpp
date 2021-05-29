/*********************************************************************\
 * USB-Mux
 *
 * USB CDC Data Endpoint class
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
#include "dev/usb.h"
#include "usb_endpoint.h"
#include "usb_phy.h"
#include "cdc_data_endpoint.h"

#include "cli.h"

CdcDataEndpoint ep2;

CdcDataEndpoint::CdcDataEndpoint() :
    USBEndpoint(2)
{
}

CdcDataEndpoint::~CdcDataEndpoint() {
}

void CdcDataEndpoint::OnReceive() {
    Buffer<BUFFER_SIZE*2> resp_buf;
    USHORT resp_len =
        Cli::Process(m_buffer.b, m_length, resp_buf.b, BUFFER_SIZE*2);
    USBPhy::TransmitData(m_epnum, resp_buf.w, resp_len);

    m_bufferPos = 0;
    m_length = 0;
    USBPhy::PrepareRX(m_epnum);
}

void CdcDataEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}
