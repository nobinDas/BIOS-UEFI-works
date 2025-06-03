#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>

//display the resgisters value
void display(){
    UINT8 CmosAddress = 0;
    UINT8 value = 0;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 16; j++){
            IoWrite8(0x70, CmosAddress);
            value = IoRead8(0x71);
            Print(L"%02x ", value);
            CmosAddress++;
        }
        Print(L"\n");
    }
}

// Convert a single hex character to a value between 0x0 and 0xF
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

    EFI_INPUT_KEY Key;
    UINTN Index;

    while (TRUE) {
        Print(L"\nPress 'r' to read, 'w' to write CMOS, or Enter to exit:\n");

        gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
        gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

        if (Key.UnicodeChar == L'\r') {
            Print(L"\nProgram shutting down\n");
            break;
        }

        //this is for reading the value
        if (Key.UnicodeChar == L'r') {
            UINT8 CmosAddress = 0;
            UINT8 value = 0;
            UINT8 digit;
            CHAR16 ch;
            
            //display start
            display();
            //display end

            Print(L"Enter 2-digit CMOS address: ");

            CmosAddress = 0;
            for (int i = 0; i < 2; i++) {
                gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
                gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
                ch = Key.UnicodeChar;
                Print(L"%c", ch);

                digit = ConvertHexChar(ch);
                if (digit == 0xFF) {
                    Print(L"\nInvalid hex digit\n");
                    goto End;
                }

                CmosAddress = (CmosAddress << 4) | digit;
            }

            IoWrite8(0x70, CmosAddress);
            value = IoRead8(0x71);

            Print(L"\nCMOS[0x%02x] = 0x%02x\n", CmosAddress, value);
        }

        //this is for writting a value at an address
        else if (Key.UnicodeChar == L'w') {
            UINT8 CmosAddress = 0;
            UINT8 CmosValue = 0;
            UINT8 digit;
            CHAR16 ch;
            UINT8 PreCmosValue = 0;

            //display start
            display();
            //display end

            Print(L"Enter 2-digit CMOS address and 2-digit value: ");

            // Read address
            CmosAddress = 0;
            for (int i = 0; i < 2; i++) {
                gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
                gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
                ch = Key.UnicodeChar;
                Print(L"%c", ch);

                digit = ConvertHexChar(ch);
                if (digit == 0xFF) {
                    Print(L"\nInvalid hex digit\n");
                    goto End;
                }

                CmosAddress = (CmosAddress << 4) | digit;
            }

            Print(L" : ");

            // Read value
            CmosValue = 0;
            for (int i = 0; i < 2; i++) {
                gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, &Index);
                gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
                ch = Key.UnicodeChar;
                Print(L"%c", ch);

                digit = ConvertHexChar(ch);
                if (digit == 0xFF) {
                    Print(L"\nInvalid hex digit\n");
                    goto End;
                }

                CmosValue = (CmosValue << 4) | digit;
            }

            IoWrite8(0x70, CmosAddress);
            PreCmosValue = IoRead8(0x71);
            IoWrite8(0x71, CmosValue);

            // Confirm write
            UINT8 Confirm = IoRead8(0x71);
            Print(L"\nCMOS[0x%02x] = 0x%02x (written)\n", CmosAddress, Confirm);

            //display start
            display();
            //display end

            //restoring the value
            IoWrite8(0x70, CmosAddress);
            IoWrite8(0x71, PreCmosValue);
            Print(L"\nCMOS[0x%02x] = 0x%02x (Restored)\n", CmosAddress, PreCmosValue);
        }

        End:
        continue;
    }

    return EFI_SUCCESS;
}
