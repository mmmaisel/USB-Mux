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
#include "usb_phy.h"
//#include "usb_descriptor.h"

USBEndpoint ep1(1, USBEndpoint::DIR_IN);

USBEndpoint::USBEndpoint(BYTE epnum, BYTE dir) :
    m_epnum(epnum)
{
/*    if(dir & DIR_IN)
        USBPhy::EnableInEndpoint(m_epnum);
    if(dir & DIR_OUT)
        USBPhy::EnableOutEndpoint(m_epnum);*/
}

USBEndpoint::~USBEndpoint() {
}

void USBEndpoint::operator delete(void* __attribute__((unused))) {
    /// Shut up stupid linker - there are no dynamic objects!!!
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

ControlEndpoint ep0;

ControlEndpoint::ControlEndpoint() :
    USBEndpoint(0, 0)
{
    // Control Endpoint is enabled in USB reset interrupt
    // -> initialize with both directions disabled
    // TODO: move ep setup here
    // TODO: keep platform dep code in phy.cpp
}

ControlEndpoint::~ControlEndpoint() {
}

void ControlEndpoint::OnReceive(USHORT len) {
    //SimpleUart::Write('R');
}

void ControlEndpoint::OnSetup(USHORT len) {
    //SimpleUart::Write('S');

    BYTE bmRequestType = m_buffer.b[0];
    BYTE bRequest = m_buffer.b[1];
    SHORT wValue = m_buffer.h[1];
    SHORT wIndex = m_buffer.h[2];
    SHORT wLength = m_buffer.h[3];

    // TODO: handle 3 back-to-back setup packets
    // TODO: get len from m_bufferPos
    // TODO: refactor buffer handling
    // TODO: refactor state machine, use switch, remove magic numbers

    /*SimpleUart::Write(0xFF);
    SimpleUart::Write(bmRequestType);
    SimpleUart::Write(bRequest);
    SimpleUart::Write(type);
    SimpleUart::Write(index);
    SimpleUart::Write(wIndex >> 8);
    SimpleUart::Write(wIndex);
    SimpleUart::Write(wLength >> 8);
    SimpleUart::Write(wLength);
    SimpleUart::Write(0x7F);*/
    if(bmRequestType == 0 && bRequest == 5) {
        // set address
        BYTE addr = wValue & 0x7F;
        USBPhy::SetAddress(addr);
        USBPhy::TransmitData(m_epnum, 0, 0);
        //SimpleUart::Write('a');
    } else if(bmRequestType == 0 && bRequest == 9) {
        // set configuration
        BYTE index = wValue;
        if(index == 0) {
            // XXX: just reset here, send ACK
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else if(index == 1) {
            // There is only one configuration, just reset DATA0 and ACK
            USBPhy::EnableInEndpoint(1);
            USBPhy::EndpointSetDATA0(0xFF);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            // NAK all other configurations
            USBPhy::TransmitStall(m_epnum);
        }
    } else if((bmRequestType == 0x80 || bmRequestType == 0x81) && bRequest == 6) {
        // XXX: ignore set/clear feature
        // get descriptor or hid descriptor
        BYTE type = wValue >> 8;
        BYTE index = wValue;
        // TODO: implement without <array>
        //const Descriptor* pDescriptor = getUsbDescriptor(type, index);
        /*if(pDescriptor)
        {
            SHORT length = pDescriptor->length;
            if(wLength < length)
                length = wLength;
            USBPhy::TransmitData(m_epnum,
                reinterpret_cast<const WORD*>(pDescriptor->data), length);
            //SimpleUart::Write('D');
        }
        else*/ {
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == 0x81 && bRequest == 10) {
        // get interface
        BYTE index = wValue;
        if(wValue == 0 && index == 0 && wLength == 1) {
            // There is only one interface, just send 0
            WORD buffer[1] = {0};
            USBPhy::TransmitData(m_epnum, buffer, 1);
        } else {
            // NAK all other interfaces
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == 0x01 && bRequest == 11) {
        // set interface
        BYTE index = wValue;
        if(wValue == 0 && index == 0 && wLength == 0) {
            // There is only one interface, just send ACK
            USBPhy::EnableInEndpoint(1);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else {
            // NAK all other interfaces
            //SimpleUart::Write('z');
            USBPhy::TransmitStall(m_epnum);
        }
    } else if(bmRequestType == 0x80 && bRequest == 0) {
        // get status
        WORD status = 0x00000001; // self-powered, no-wakeup
        USBPhy::TransmitData(m_epnum, &status, 2);
        //SimpleUart::Write('S');
    } else if(bmRequestType == 0xA1 && bRequest == 1) {
        // TODO: move hid stuff to own class
        // TODO: implement HID commands on EP0, usb_hid_core.c line 256 and following
        // get report
        BYTE type = wValue >> 8;
        BYTE index = wValue;
        WORD buffer[2];
        BYTE length = 8;
        //HidKeyboard::MakeReport((BYTE*)buffer);
        if(wLength < length)
            length = wLength;
        USBPhy::TransmitData(m_epnum, buffer, length);
    } else if(bmRequestType == 0xA1 && bRequest == 2) {
        // get idle
        BYTE index = wValue;
        WORD buffer[1] = { 0 };
        //buffer[0] = HidKeyboard::GetIdle();
        USBPhy::TransmitData(m_epnum, buffer, 1);
    } else if(bmRequestType == 0x21 && bRequest == 10) {
        // set idle
        BYTE duration = wValue >> 8;
        BYTE index = wValue;
        if(wIndex == 0) {
            //HidKeyboard::SetIdle(duration);
            USBPhy::TransmitData(m_epnum, 0, 0);
        } else
            USBPhy::TransmitStall(m_epnum);
    } else if(bmRequestType == 0xA1 && bRequest == 3) {
        // get protocol
        WORD buffer[1] = { 0 };
        // TODO: implement action
        USBPhy::TransmitData(m_epnum, buffer, 1);
    } else if(bmRequestType == 0x21 && bRequest == 11) {
        // set protocol
        // TODO: implement action
        USBPhy::TransmitData(m_epnum, 0, 0);
    } else {
        //SimpleUart::Write('x');
        USBPhy::TransmitStall(m_epnum);
    }
    m_bufferPos = 0;
    // XXX: just in case
    m_buffer.w[0] = 0;
    m_buffer.w[1] = 0;
    m_buffer.w[2] = 0;
    m_buffer.w[3] = 0;

    USBPhy::PrepareRX(m_epnum);
    //SimpleUart::Write('0');
}

void ControlEndpoint::OnTransmit() {
    //SimpleUart::Write('T');
}

void ControlEndpoint::OnRxData(WORD data) {
    m_buffer.w[m_bufferPos++] = data;
    //SimpleUart::Write('r');
}

USBEndpoint* eps[2] = {&ep0, &ep1};

