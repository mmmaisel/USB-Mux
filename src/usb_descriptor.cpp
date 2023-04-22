/**********************************************************************\
 * USB-Mux
 *
 * USB descriptors
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
#include "usb_descriptor.h"

// XXX: add PROGMEM support

DeviceDescriptor DEVICE_DESCRIPTOR = {
    sizeof(DeviceDescriptor),
    DESCRIPTOR_TYPE_DEVICE,
    0x200,
    CLASS_COMM,
    SUBCLASS_COMM_ACM,
    PROTOCOL_ACM,
    64,
    0x1209,
    0x554D,
    0x100,
    1,
    2,
    3,
    1
};

ConfigDescriptor0 CONFIG_DESCRIPTOR = {
    sizeof(ConfigDescriptor),
    DESCRIPTOR_TYPE_CONFIG,
    sizeof(ConfigDescriptor0),
    2,
    1,
    0,
    0x80,
    250,
    {
        sizeof(InterfaceDescriptor),
        DESCRIPTOR_TYPE_INTERFACE,
        0,
        0,
        1,
        CLASS_COMM,
        SUBCLASS_COMM_ACM,
        PROTOCOL_ACM,
        0
    },
    {
        sizeof(CdcHeaderDescriptor),
        DESCRIPTOR_TYPE_CS_INTERFACE,
        0, // Header
        0x0110
    },
    {
        sizeof(CdcAcmDescriptor),
        DESCRIPTOR_TYPE_CS_INTERFACE,
        0x02, // ACM
        0x00
    },
    {
        sizeof(CdcUnionDescriptor),
        DESCRIPTOR_TYPE_CS_INTERFACE,
        0x06, // Union
        0,
        1
    },
    {
        sizeof(CdcManagementDescriptor),
        DESCRIPTOR_TYPE_CS_INTERFACE,
        0x01, // Management
        0x00,
        1
    },
    {
        sizeof(EndpointDescriptor),
        DESCRIPTOR_TYPE_ENDPOINT,
        1 | ENDPOINT_DIR_IN,
        ENDPOINT_MODE_INTERRUPT,
        64,
        255
    },
    {
        sizeof(InterfaceDescriptor),
        DESCRIPTOR_TYPE_INTERFACE,
        1,
        0,
        2,
        CLASS_CDC_DATA,
        0,
        0,
        0,
    },
    {
        {
            sizeof(EndpointDescriptor),
            DESCRIPTOR_TYPE_ENDPOINT,
            2,
            ENDPOINT_MODE_BULK,
            64,
            10
        },
        {
            sizeof(EndpointDescriptor),
            DESCRIPTOR_TYPE_ENDPOINT,
            2 | ENDPOINT_DIR_IN,
            ENDPOINT_MODE_BULK,
            64,
            10
        }
    }
};

StringDescriptorLang STR_LANG_DESCRIPTOR = {
    sizeof(StringDescriptorLang),
    DESCRIPTOR_TYPE_STRING,
    {
        0x0407  // German (Default)
    }
};

StringDescriptorManf STR_MANF_DESCRIPTOR = {
    sizeof(StringDescriptorManf),
    DESCRIPTOR_TYPE_STRING,
    { 'm', 'm', 'm', 'a', 'i', 's', 'e', 'l' }
};

StringDescriptorProduct STR_PRODUCT_DESCRIPTOR = {
    sizeof(StringDescriptorProduct),
    DESCRIPTOR_TYPE_STRING,
    { 'U', 'S', 'B', '-', 'M', 'u', 'x', }
};

StringDescriptorSerial STR_SERIAL_DESCRIPTOR = {
    sizeof(StringDescriptorSerial),
    DESCRIPTOR_TYPE_STRING,
    { '0', '0', '0', '0', '0', '0', '0', '0' }
};

// XXX: pad descriptors to word boundaries to avoid leaking memory
#define DESCRIPTOR_ENTRY(descriptor, type) \
    { sizeof(descriptor), DESCRIPTOR_TYPE_ ## type, (BYTE*)&descriptor }

static const Descriptor DESCRIPTORS[] = {
    // Descriptors of the same type must be ordered as they are
    // indexed like an array.
    DESCRIPTOR_ENTRY(DEVICE_DESCRIPTOR, DEVICE),
    DESCRIPTOR_ENTRY(CONFIG_DESCRIPTOR, CONFIG),
    DESCRIPTOR_ENTRY(STR_LANG_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_MANF_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_PRODUCT_DESCRIPTOR, STRING),
    DESCRIPTOR_ENTRY(STR_SERIAL_DESCRIPTOR, STRING)
};

const Descriptor* get_usb_descriptor(BYTE type, BYTE index) {
    BYTE i = 0;
    for(const auto& descriptor : DESCRIPTORS) {
        if(descriptor.type == type) {
            if(i++ == index)
                return &descriptor;
        }
        else
            i = 0;
    }
    return nullptr;
}
