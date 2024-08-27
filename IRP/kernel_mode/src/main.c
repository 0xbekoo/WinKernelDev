// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: I/O Request Packets (IRP) - Kernel Mode        ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#include "main.h"

NTSTATUS IrpCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    
    PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);

    switch (Stack->MajorFunction) {

        case IRP_MJ_CREATE:
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;

        case IRP_MJ_CLOSE:
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;
    
        default:
            Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Irp->IoStatus.Status;
}

NTSTATUS IrpWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    PIO_STACK_LOCATION  Stack   = IoGetCurrentIrpStackLocation(Irp);
    PCHAR Buffer  = Irp->UserBuffer;
    ULONG Length  = Stack->Parameters.Write.Length;

    if (NULL == Buffer || 0 == Length) {
        DbgPrintEx(0, 0, "Failed to Received Data!\n");
        Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
        Irp->IoStatus.Information = 0;

        return Irp->IoStatus.Status;
    }
    DbgPrintEx(0, 0, "Received Data: %.*s\n", Length, Buffer);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = Length;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\MyDevice");
    UNICODE_STRING SymName = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
    PDEVICE_OBJECT DeviceObject;
    NTSTATUS Status;

    Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
    if (!NT_SUCCESS(Status)) {
        DbgPrintEx(0, 0, "Failed to Create IO Device!\n");
        return Status;
    }

    Status = IoCreateSymbolicLink(&SymName, &DeviceName);
    if (!NT_SUCCESS(Status)) {
        DbgPrintEx(0, 0, "Failed to Create Smybolic Link!\n");
        return Status;
    }
    DriverObject->MajorFunction[IRP_MJ_CREATE] = IrpCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = IrpCreateClose;
    DriverObject->MajorFunction[IRP_MJ_WRITE] = IrpWrite;
    DriverObject->DriverUnload = UnloadDriver;

    return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT PDrvObj) {
    UNICODE_STRING SymName = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
    DbgPrintEx(0, 0, "Unloading Driver...\n");

    IoDeleteSymbolicLink(&SymName);
    IoDeleteDevice(PDrvObj->DeviceObject);

    return STATUS_SUCCESS;
}
