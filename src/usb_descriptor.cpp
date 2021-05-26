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

DeviceDescriptor DEVICE_DESCRIPTOR = {
    sizeof(DeviceDescriptor),
    DESCRIPTOR_TYPE_DEVICE,
    0x200,
    0x00,
    0x00,
    0x00,
    64,
    0xDEAD,
    0xBEEF,
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
    1,
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
        0x03,
        0x01,
        0x01,
        0,
        {
            sizeof(HidDescriptor),
            DESCRIPTOR_TYPE_HID,
            0x0110,
            0,
            1,
            DESCRIPTOR_TYPE_HID_REPORT,
            sizeof(HidReportDescriptor0)
        },
        {
            sizeof(EndpointDescriptor),
            DESCRIPTOR_TYPE_ENDPOINT,
            1 | ENDPOINT_DIR_IN,
            3,
            8,
            2
        }
    }
};

HidReportDescriptor0 HID_REPORT_DESCRIPTOR = {
    0x05, 0x01,         // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,         // USAGE (Keyboard)
    0xa1, 0x01,         // COLLECTION (Application)
    0x05, 0x07,         //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,         //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,         //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x25, 0x01,         //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,         //   REPORT_SIZE (1)
    0x95, 0x08,         //   REPORT_COUNT (8)
    0x81, 0x02,         //   INPUT (Data,Var,Abs)
    0x75, 0x08,         //   REPORT_SIZE (8)
    0x95, 0x01,         //   REPORT_COUNT (1)
    0x81, 0x01,         //   INPUT (Cnst,Ary,Abs)
    0x05, 0x07,         //   USAGE_PAGE (Keyboard)
    0x19, 0x00,         //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xe7,         //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,         //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,   //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,         //   REPORT_SIZE (8)
    0x95, 0x06,         //   REPORT_COUNT (6)
    0x81, 0x00,         //   INPUT (Data,Ary,Abs)
    0xc0                // END_COLLECTION
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
    { 'A', 'C', 'M', 'E', ' ', 'C', 'o', 'r', 'p', '.' }
};

StringDescriptorProduct STR_PRODUCT_DESCRIPTOR = {
    sizeof(StringDescriptorProduct),
    DESCRIPTOR_TYPE_STRING,
    { 'F', 'l', 'u', 'x', 'k', 'o', 'm', 'p', 'e', 'n', 's', 'a', 't', 'o', 'r' }
};

StringDescriptorSerial STR_SERIAL_DESCRIPTOR = {
    sizeof(StringDescriptorSerial),
    DESCRIPTOR_TYPE_STRING,
    { '4', '2', '-', '1', '3', '3', '7', '-', '4', '7', '/', '1', '1' }
};

// XXX: pad descriptors to word boundaries to avoid leaking memory
#define DESCRIPTOR_ENTRY(descriptor, type) \
    { sizeof(descriptor), DESCRIPTOR_TYPE_ ## type, (BYTE*)&descriptor }

static const Descriptor DESCRIPTORS[] = {
    // Descriptors of the same type must be ordered as they are
    // indexed like an array.
    DESCRIPTOR_ENTRY(DEVICE_DESCRIPTOR, DEVICE),
    DESCRIPTOR_ENTRY(CONFIG_DESCRIPTOR, CONFIG),
    DESCRIPTOR_ENTRY(HID_REPORT_DESCRIPTOR, HID_REPORT),
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
