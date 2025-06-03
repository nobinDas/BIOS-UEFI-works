#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include "pci_ids.h"
#include "PciIdLookup.h" 

EFI_STATUS EFIAPI UefiEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
    gST = SystemTable;
    gBS = SystemTable->BootServices;

    //variables
    UINTN HandleCount;
    EFI_HANDLE *HandleBuffer;
    EFI_PCI_IO_PROTOCOL *PciIo;
    PCI_TYPE00 PCIConfig;
    UINTN Segment, Bus, Device, Function;

    //registers the number of handle and keeps the values in Buffer according the given protocol
    gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiPciIoProtocolGuid,
        NULL,
        &HandleCount,
        &HandleBuffer
    );

    //loop to go over each handle strored in HandleBuffer
    for(int i=0; i<HandleCount; i++){
        //checks if the buffer value matches the protocol and registers the value in the protocol type EFI_PCI_IO_PROTOCOL
        gBS->HandleProtocol(
            HandleBuffer[i],
            &gEfiPciIoProtocolGuid,
            (VOID **)&PciIo
        );

        //PCi function to dig the value of segment, bus, device and function according to the given PCI
        PciIo->GetLocation(PciIo, &Segment, &Bus, &Device, &Function);

        //function to read and store the configuration of PCI according to the PCI
        PciIo->Pci.Read(
            PciIo,
            EfiPciIoWidthUint32,
            0,
            sizeof(PCIConfig)/sizeof(UINT32),
            &PCIConfig
        );

        //cheks if the vendor id is acceptable and the prints the device
        if (PCIConfig.Hdr.VendorId != 0xffff) {
            Print(L"Bus: %02x, Device: %02x, Function: %02x:\n", Bus, Device, Function);

            Print(L"VendorID:%04x (%s), DeviceID:%04x (%s), ",
                PCIConfig.Hdr.VendorId,
                GetVendorName(PCIConfig.Hdr.VendorId),//function from the PciLookup.c file to print the vendor name
                PCIConfig.Hdr.DeviceId,
                GetDeviceName(PCIConfig.Hdr.VendorId, PCIConfig.Hdr.DeviceId));//function from the PciLookup.c file to print the device name

            //it checks the capability segment of the pci table to know if it is PCI or PCIe
            if (PCIConfig.Device.CapabilityPtr != 0) {
                UINT8 CapId;
                PciIo->Pci.Read(
                    PciIo,
                    EfiPciIoWidthUint8,
                    PCIConfig.Device.CapabilityPtr,
                    1,
                    &CapId
                );
                Print(L" Capability ID at 0x%02x: 0x%02x\n", PCIConfig.Device.CapabilityPtr, CapId);

                //prints if PCI or PCIe
                if (CapId == 0x00) {
                    Print(L"-----Null or reserved capability.\n");
                } else if (CapId == 0x01) {
                    Print(L"-----(PCI).\n");
                } else if (CapId == 0x02) {
                    Print(L"-----(PCI).\n");
                } else if (CapId == 0x05) {
                    Print(L"-----(PCI/PCIe).\n");
                } else if (CapId == 0x09) {
                    Print(L"-----(PCI/PCIe).\n");
                } else if (CapId == 0x10) {
                    Print(L"-----(PCIe).\n");
                } else if (CapId == 0x11) {
                    Print(L"-----(PCIe).\n");
                } else {
                    Print(L"Unknown or unhandled capability: 0x%02x\n", CapId);
                }
            }
        }
        Print(L"\n\n");//prints a line
    }


    return EFI_SUCCESS;
}

