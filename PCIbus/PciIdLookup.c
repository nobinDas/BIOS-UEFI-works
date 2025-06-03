#include <Uefi.h>
#include "pci_ids.h"

CHAR16* GetVendorName(UINT16 vendor_id) {
    for (int i = 0; i < vendor_table_size; i++) {
        if (vendor_table[i].vendor_id == vendor_id)
            return vendor_table[i].name;
    }
    return L"Unknown Vendor";
}

CHAR16* GetDeviceName(UINT16 vendor_id, UINT16 device_id) {
    for (int i = 0; i < device_table_size; i++) {
        if (device_table[i].vendor_id == vendor_id && device_table[i].device_id == device_id)
            return device_table[i].name;
    }
    return L"Unknown Device";
}
