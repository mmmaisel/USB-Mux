/**********************************************************************\
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
#pragma once

#include "types.h"

class CdcDataEndpoint : public USBEndpoint {
    public:
        CdcDataEndpoint();
        CdcDataEndpoint(const CdcDataEndpoint&) = delete;
        CdcDataEndpoint(CdcDataEndpoint&&) = delete;
        virtual ~CdcDataEndpoint();

    protected:
        virtual void OnReceive() override;
        virtual void OnTransmit() override;
};
extern CdcDataEndpoint ep2;
