/*********************************************************************\
 * USB-Mux
 *
 * USB CDC Control Endpoint class
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
#include "cdc_control_endpoint.h"

CdcControlEndpoint ep1;

CdcControlEndpoint::CdcControlEndpoint() :
    USBEndpoint(1)
{
}

CdcControlEndpoint::~CdcControlEndpoint() {
}

void CdcControlEndpoint::OnReceive() {
    //SimpleUart::Write('R');
}

void CdcControlEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}
