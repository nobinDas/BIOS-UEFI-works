#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>

UINT8 ConvertHexChar(CHAR16 ch) {
    if (ch == L'0') {
        return 0x0;
    } else if (ch == L'1') {
        return 0x1;
    } else if (ch == L'2') {
        return 0x2;
    } else if (ch == L'3') {
        return 0x3;
    } else if (ch == L'4') {
        return 0x4;
    } else if (ch == L'5') {
        return 0x5;
    } else if (ch == L'6') {
        return 0x6;
    } else if (ch == L'7') {
        return 0x7;
    } else if (ch == L'8') {
        return 0x8;
    } else if (ch == L'9') {
        return 0x9;
    } else if (ch == L'a' || ch == L'A') {
        return 0xA;
    } else if (ch == L'b' || ch == L'B') {
        return 0xB;
    } else if (ch == L'c' || ch == L'C') {
        return 0xC;
    } else if (ch == L'd' || ch == L'D') {
        return 0xD;
    } else if (ch == L'e' || ch == L'E') {
        return 0xE;
    } else if (ch == L'f' || ch == L'F') {
        return 0xF;
    } else {
        return 0xFF;  // Invalid hex character
    }

}

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    gST = SystemTable;
    gBS = SystemTable->BootServices;

    UINT64 value = AsmReadMsr64(0x34);
    Print(L"%lu\n", value);

    IoWrite8(0x80, 0x00);
    IoWrite8(0xB2, 0x34);

    value = AsmReadMsr64(0x34);
    Print(L"%lu\n", value);



    return EFI_SUCCESS;
}