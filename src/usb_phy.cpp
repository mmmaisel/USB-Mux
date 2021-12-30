/**********************************************************************\
 * USB-Mux
 *
 * USB driver class
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
#include "usb_phy.h"

#include "dev/core.h"
#include "dev/gpio.h"
#include "dev/rcc.h"
#include "dev/usb.h"
#include "usb_endpoint.h"

using namespace dev;
using namespace dev::rcc;
using namespace dev::usb;

void USBPhy::Initialize() {
    // Enable power
    RCC->AHB1ENR |= GPIOAEN;
    RCC->AHB2ENR |= USBEN;

    // Configure pins: PA9: VBUS, PA11: DM, PA12: DP
    GPIOA->MODER  |=  (2 << (2*9)) | (2 << (2*11)) | (2 << (2*12));
    GPIOA->AFRH |= (10 << (4*1)) | (10 << (4*3)) | (10 << (4*4));

    // Enable USB interrupt, ISR number 67
    NVIC->EN[2] = (1 << 3);
    NVIC->PRI[67]  = 0x09;

    // Programming model see RefMan, p. 764
    // Core init
    USB->GAHBCFG |= GINTMSK | PTXFELVL;
    USB->GINTSTS |= RXFLVL;
    USB->GUSBCFG |= HNPCAP | SRPCAP |
        (15 << TRDT_POS) | (3 << TOCAL_POS) | FDMOD;
    USB->GINTMSK |= OTGINT | MMIS;

    // Device init
    USBDEV->DCFG |= DSPD_FULL;
    USB->GINTMSK |= USBRST | ENUMDNE | ESUSP | USBSUSP;
    USB->GCCFG |= VBUSBSEN;

    // Enable USB
    USBDEV->DCTL &= ~SDIS;
    USB->GCCFG |= PWRDWN;
}

void USBPhy::GetRXData(WORD* pBuffer, USHORT wcnt) {
    for(USHORT i = 0; i < wcnt; i++) {
        // RX FIFO is always 0
        pBuffer[i] = USB_FIFO[0].data;
    }
}

void USBPhy::PrepareRX(BYTE epnum) {
    // XXX: setup packets are handled separately
    // XXX: this must be done after each transfer
    USB_OUTEP[epnum].DOEPTSIZ &= ~0x0000007F;
    USB_OUTEP[epnum].DOEPTSIZ |= (1 << PKTCNT_POS) | (64 << XFRSIZ_POS) | RXPID_MDATA;
    USB_OUTEP[epnum].DOEPCTL |= EPENA | CNAK;
}

void USBPhy::TransmitData(BYTE epnum, const WORD* pBuffer, USHORT len) {
    WORD wcnt = (len + 3) / 4;
    while((USB_INEP[epnum].DTXFSTS & 0xFFFF) < wcnt)
        ;

    USB_INEP[epnum].DIEPTSIZ = (USB_INEP[epnum].DIEPTSIZ
        & ~(XFRSIZ_MASK | PKTCNT_MASK))
    // TODO. handle split packets (zero len packets have a pkt count of 1!)
    // TODO: XFRSIZ register is length in bytes(?) of all packets
        | (1 << PKTCNT_POS) | (len << XFRSIZ_POS);
    USB_INEP[epnum].DIEPCTL &= ~STALL;
    USB_INEP[epnum].DIEPCTL |= CNAK | EPENA;
    // XXX: TXFE interrupt mask
    USBDEV->DIEPEMPMSK |= (1 << epnum);

    for(USHORT i = 0; i < wcnt; i++) {
        USB_FIFO[epnum].data = pBuffer[i];
    }
}

BYTE USBPhy::TXFIFOEmpty(BYTE epnum) {
    // TODO: use TXFE interrupt flag instead
    return (USB_INEP[epnum].DTXFSTS & 0xFFFF) == 128 / 4;
}

void USBPhy::TransmitStall(BYTE epnum) {
    USB_INEP[epnum].DIEPTSIZ &= ~(XFRSIZ_MASK | PKTCNT_MASK);
    USB_INEP[epnum].DIEPCTL |= STALL | EPENA;
}

void USBPhy::SetAddress(BYTE adr) {
    USBDEV->DCFG = (USBDEV->DCFG & DADR_MASK) | (adr << DADR_POS);
}

void USBPhy::ResetAllEndpoints() {
    // Flush all FIFOs
    USB->GRSTCTL = TXFFLSH | (0x10 << TXFNUM_POS) | RXFFLSH;
    while(USB->GRSTCTL & (RXFFLSH | TXFFLSH));

    // Clear all device endpoint interrupts
    USBDEV->DIEPMSK = 0;
    USBDEV->DOEPMSK = 0;
    USBDEV->DAINT = 0xFFFFFFFF;
    USBDEV->DAINTMSK = 0;

    // Reset all endpoints
    for(BYTE i = 0; i < 4; ++i) {
        if(USB_INEP[i].DIEPCTL & EPENA) {
            USB_INEP[i].DIEPCTL = EPDIS | SNAK;
        } else {
            USB_INEP[i].DIEPCTL = 0;
        }
        USB_INEP[i].DIEPTSIZ = 0;
        USB_INEP[i].DIEPINT = 0xFF;

        if(USB_OUTEP[i].DOEPCTL & EPENA) {
            USB_OUTEP[i].DOEPCTL = EPDIS | SNAK;
        } else {
            USB_OUTEP[i].DOEPCTL = 0;
        }
        USB_OUTEP[i].DOEPTSIZ = 0;
        USB_OUTEP[i].DOEPINT = 0xFF;
    }
    // TODO: wait until all endpoints are disabled

    USB->GINTMSK |= IEPINT | OEPINT;
}

void USBPhy::EnableInEndpoint(BYTE num) {
    USBDEV->DAINTMSK |= (1 << num) << IEPINT_POS;
    USBDEV->DIEPMSK  |= XFRC;
    // | USB_OTG_DIEPMSK_ITTXFEMSK;// | USB_OTG_DIEPMSK_TOM;

    // TODO: ep0 TX FF is special, in GLOBAL register
    // TODO: calc FIFO num from size
    // XXX: FIFO: (size in words << 16) | (sum size from FF0 in words, == pos)
    // TODO: TXFF0 does not fit into addressing scheme
    // TODO: observe EP enable disable timing
    if(num == 0) {
        USB->DIEPTXF0_HNPTXFSIZ =
            ((128 / 4) << TX0FD_POS) | ((256 / 4) << TX0FSA_POS);
        USB_INEP[num].DIEPCTL |= USBAEP | (num << TXFNUM_POS);
    } else {
        USB->DIEPTXF[num-1] =
            ((128 / 4) << INEPTXFD_POS) | (((256 + 128*num) / 4) << INEPTXSA_POS);
        USB_INEP[num].DIEPCTL |= (64 << MPSIZ_POS) | EPTYP_INT |
            USBAEP | (num << TXFNUM_POS) | SD0PID;
    }
}

void USBPhy::EnableOutEndpoint(BYTE num) {
    USBDEV->DAINTMSK |= ((1 << num) << OEPINT_POS);
    USBDEV->DOEPMSK  |= XFRC | STUP;

    // TODO: this is global and must be >= 256 ?
    USB->GRXFSIZ = ((256 / 4) << RXFD_POS); // 256 bytes RX FIFO

    // TODO: implement other endpoints > 0

    USB_OUTEP[num].DOEPTSIZ = (3 << STUPCNT_POS); // 3 setup packets
    USB_OUTEP[num].DOEPCTL |= USBAEP | CNAK;

    USB->GINTMSK |= RXFLVL;
}

void USBPhy::EndpointSetDATA0(BYTE num) {
    if(num == 0xFF) {
        for(BYTE x = 1; x < 4; ++x) {
            USB_INEP[x].DIEPCTL |= SD0PID;
            USB_OUTEP[x].DOEPCTL |= SD0PID;
        }
    } else if(num >= 1 && num <= 4) {
        USB_INEP[num].DIEPCTL |= SD0PID;
        USB_OUTEP[num].DOEPCTL |= SD0PID;
    }
}

void USBPhy::ISR() {
    WORD cause = USB->GINTSTS & USB->GINTMSK;

    if(cause & MMIS) {
        // Mode mismatch error
        for(;;);
    }
    if(cause & USBRST) {
        ResetAllEndpoints();
        EnableOutEndpoint(0);
        EnableInEndpoint(0);

        cause &= ~USBRST;
        USB->GINTSTS = USBRST;
    }
    if(cause & ENUMDNE) {
        WORD status = USBDEV->DSTS;

        // XXX: The hardware only supports full-speed mode
        //      => fixed max packet size of 64
        USB_INEP[0].DIEPCTL &= ~(3 << 0);

        cause &= ~ENUMDNE;
        USB->GINTSTS = ENUMDNE;
    }
    // TODO: disable this interrupt
    if(cause & SOF) {
        // XXX: this is just for timesync and maybe high-speed devices
        WORD status = USBDEV->DSTS;
        cause &= ~SOF;
        USB->GINTSTS = SOF;
    }
    if(cause & OTGINT) {
        WORD status = USB->GOTGINT;
        cause &= ~OTGINT;
        // VBUS went low, cable disconnected
        if(status & SEDET)
            USB->GOTGINT = SEDET;
    }
    if(cause & USBSUSP) {
        cause &= ~USBSUSP;
        USB->GINTSTS = USBSUSP;
    }
    if(cause & ESUSP) {
        // TODO: implement sleep mode
        cause &= ~ESUSP;
        USB->GINTSTS = ESUSP;
    }
    if(cause & RXFLVL) {
        //SimpleUart::Write('x');
        // XXX: setup packet must be decoded in OEP STUP interrupt,
        //   FIFO POP must stay here
        // Read and pop status
        WORD rxstat = USB->GRXSTSP;

        USHORT size = (rxstat & BCNT_MASK) >> BCNT_POS;
        // XXX: FRMNUM definition is missing!!!
        BYTE frmnum = (rxstat & FRMNUM_MASK) >> FRMNUM_POS;
        BYTE pktsts = (rxstat & PKTSTS_MASK) >> PKTSTS_POS;
        BYTE dpid   = (rxstat & DPID_MASK) >> DPID_POS;
        BYTE epnum  = (rxstat & EPNUM_MASK) >> EPNUM_POS;

        if(epnum != 0) {
            asm volatile(" nop");
        } else {
            int wcnt = (size + 3) / 4;
            for(int i = 0; i < wcnt; ++i)
                eps[epnum]->OnRxData(USB_FIFO[0].data);
        }

        cause &= ~RXFLVL;
    }
    if(cause & IEPINT) {
        WORD ep_bits = USBDEV->DAINT;
        // get endpoint from ep_bits
        for(WORD num = 0, mask = (1 << 0); mask < (1 << 4); ++num, mask <<= 1) {
            if(ep_bits & mask) {
                WORD cause2 = USB_INEP[num].DIEPINT & USBDEV->DIEPMSK;
                USBDEV->DIEPEMPMSK &= ~(1 << num);
                if(cause2 & XFRC)
                    eps[num]->OnTransmit();
                // TODO: relevant: XFRC (for empty ack packet)
                // TODO: TXFE ISR is optional here
                //else if(cause2 & USB_OTG_DIEPINT_ITTXFE)
                //    ;
                USB_INEP[num].DIEPINT = cause2;
            }
        }
        cause &= ~IEPINT;
    }
    if(cause & OEPINT) {
        WORD ep_bits = USBDEV->DAINT;
        // get endpoint from ep_bits
        for(WORD num = 0, mask = (1 << 16); mask < (1 << 20); ++num, mask <<= 1) {
            // TODO: XFRC interrupts are triggered when special values
            // are popped from FIFO in RXFLVL interrupt
            if(ep_bits & mask) {
                WORD cause2 = USB_OUTEP[num].DOEPINT & USBDEV->DOEPMSK;
                if(cause2 & STUP)
                    eps[num]->OnSetup(0 /* dummy */);
                else if(cause2 & XFRC)
                    eps[num]->OnReceive(0 /* dummy */);
                // STSPHSRX ISR is broken, it never occurs
                //else if(cause2 & USB_OTG_DOEPINT_STSPHSRX)
                //    eps[num]->OnStatus();
                USB_OUTEP[num].DOEPINT = cause2;
            }
        }
        cause &= ~OEPINT;
    }
}

extern "C" void usb_vector() {
    USBPhy::ISR();
}
