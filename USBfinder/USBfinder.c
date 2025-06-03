#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Usb.h>
#include <Protocol/UsbIo.h>

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    gST = SystemTable;
    gBS = SystemTable->BootServices;

    UINTN HandleCount;
    EFI_HANDLE *HandleBuffer;
    EFI_USB_IO_PROTOCOL *UsbIo;
    EFI_USB_DEVICE_DESCRIPTOR DeviceDes;
    UINT16 *Lang;
    UINT16 tableSize;
    CHAR16 *Name;
    CHAR16 *Type;
    CHAR16 *Number;

    //registers the number of handle and keeps the values in Buffer according the given protocol
    gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiUsbIoProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
    );

    //loop to go over each handle strored in HandleBuffer
    for(int i=0; i<HandleCount; i++){
        //checks if the buffer value matches the protocol and registers the value in the protocol type EFI_USB_IO_PROTOCOL
        gBS->HandleProtocol(
            HandleBuffer[i],
            &gEfiUsbIoProtocolGuid,
            (VOID **)&UsbIo
        );

        //this function outputs the Device descriptor value in variable DeviceDes
        UsbIo->UsbGetDeviceDescriptor(
            UsbIo,
            &DeviceDes
        );

        //this function gets all the languags avaliable in the Usb
        UsbIo->UsbGetSupportedLanguages(
            UsbIo,
            &Lang,
            &tableSize
        );
        
        //according to the given language it registers the name of the product according to the index in StrProduct
        UsbIo->UsbGetStringDescriptor(
            UsbIo,
            Lang[0],
            DeviceDes.StrManufacturer,
            &Name
        );

        UsbIo->UsbGetStringDescriptor(
            UsbIo,
            Lang[0],
            DeviceDes.StrProduct,
            &Type
        );

        EFI_STATUS status = UsbIo->UsbGetStringDescriptor(
            UsbIo,
            0x0409,
            DeviceDes.StrSerialNumber,
            &Number
        );

        if (status == EFI_NOT_FOUND) {
            Number = L"Not found";
        }

        //prints the name, type and number of the usb products
        Print(L"Name:%s \nType:%s \nNumber:%s\n\n", 
                                                    Name, 
                                                    Type, 
                                                    Number);
    }
    return EFI_SUCCESS;
}

