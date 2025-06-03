#pragma once

#include <Uefi.h>

CHAR16* GetVendorName(UINT16 vendor_id);
CHAR16* GetDeviceName(UINT16 vendor_id, UINT16 device_id);
