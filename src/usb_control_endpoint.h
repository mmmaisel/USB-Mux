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

        enum {
            REQUEST_DIR_OUT = 0x00,
            REQUEST_DIR_IN = 0x80,
            REQUEST_TYPE_STANDARD = 0x00,
            REQUEST_TYPE_CLASS = 0x20,
            REQUEST_TYPE_VENDOR = 0x40,
            REQUEST_TO_DEVICE = 0x00,
            REQUEST_TO_INTERFACE = 0x01,
            REQUEST_TO_ENDPOINT = 0x02,
            REQUEST_TO_OTHER = 0x03,

            SET_STANDARD_DEVICE =
                REQUEST_DIR_OUT | REQUEST_TYPE_STANDARD | REQUEST_TO_DEVICE,
            GET_STANDARD_DEVICE =
                REQUEST_DIR_IN | REQUEST_TYPE_STANDARD | REQUEST_TO_DEVICE,
            SET_STANDARD_INTERFACE =
                REQUEST_DIR_OUT | REQUEST_TYPE_STANDARD | REQUEST_TO_INTERFACE,
            GET_STANDARD_INTERFACE =
                REQUEST_DIR_IN | REQUEST_TYPE_STANDARD | REQUEST_TO_INTERFACE,
            SET_CLASS_INTERFACE =
                REQUEST_DIR_OUT | REQUEST_TYPE_CLASS | REQUEST_TO_INTERFACE,
            GET_CLASS_INTERFACE =
                REQUEST_DIR_IN | REQUEST_TYPE_CLASS | REQUEST_TO_INTERFACE,

            REQUEST_GET_STATUS = 0,
            REQUEST_SET_ADDRESS = 5,
            REQUEST_GET_DESCRIPTOR = 6,
            REQUEST_SET_CONFIG = 9,
            REQUEST_GET_INTERFACE = 10,
            REQUEST_SET_INTERFACE = 17,

            // Application specific requests below
            REQUEST_SET_LINE_CODING = 32
        };

        enum {
            WAKEUP = (1 << 1),
            SELF_POWERED = (1 << 0),
            APP_STATUS = 0x00000000 // no wakeup, bus powered
        };

    protected:
        virtual void OnReceive() override;
        virtual void OnSetup() override;
        virtual void OnTransmit() override;

    private:
        static void EnableAppEndpoints();
};
extern ControlEndpoint ep0;
