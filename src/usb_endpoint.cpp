/*********************************************************************\
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
#include "usb_endpoint.h"
#include "usb_control_endpoint.h"
#include "cdc_control_endpoint.h"
#include "cdc_data_endpoint.h"
#include "usb_phy.h"

USBEndpoint::USBEndpoint(BYTE epnum) :
    m_epnum(epnum)
{
}

USBEndpoint::~USBEndpoint() {
}

void USBEndpoint::operator delete(void* __attribute__((unused))) {
    /// Shut up stupid linker - there are no dynamic objects!!!
}

void USBEndpoint::Enable(BYTE dir) {
    if(dir & DIR_IN)
        USBPhy::EnableInEndpoint(m_epnum);
    if(dir & DIR_OUT)
        USBPhy::EnableOutEndpoint(m_epnum);
}

void USBEndpoint::Transmit(const WORD* pData, USHORT len) {
    USBPhy::TransmitData(m_epnum, pData, len);
}

void USBEndpoint::Receive(WORD* pData, USHORT len) {
}

BYTE USBEndpoint::TXFIFOEmpty() {
    USBPhy::TXFIFOEmpty(m_epnum);
}

void USBEndpoint::OnReceive(USHORT len) {
}

void USBEndpoint::OnSetup(USHORT len) {
}

void USBEndpoint::OnTransmit() {
}

void USBEndpoint::OnRxData(WORD data) {
}

USBEndpoint* eps[] = {&ep0, &ep1, &ep2};
