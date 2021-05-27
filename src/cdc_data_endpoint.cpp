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
#include "usb_endpoint.h"
#include "cdc_data_endpoint.h"

CdcDataEndpoint ep2;

CdcDataEndpoint::CdcDataEndpoint() :
    USBEndpoint(2, DIR_IN | DIR_OUT)
{
}

CdcDataEndpoint::~CdcDataEndpoint() {
}

void CdcDataEndpoint::OnReceive(USHORT len) {
    //SimpleUart::Write('R');
}

void CdcDataEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}

void CdcDataEndpoint::OnRxData(WORD data) {
    //SimpleUart::Write('r');
}
