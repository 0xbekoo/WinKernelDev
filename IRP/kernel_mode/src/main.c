#include "main.h"

NTSTATUS IrpCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    DbgPrintEx(0, 0, "\n\nMJ_Create Received!\n");

    Irp->IoStatus.Status = STATUS_SUCCESS;;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Irp->IoStatus.Status;
}

NTSTATUS IrpWrite(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    PIO_STACK_LOCATION  Stack   = IoGetCurrentIrpStackLocation(Irp);
    PCHAR               Buffer  = Irp->UserBuffer;
    ULONG               Length  = Stack->Parameters.Write.Length;

    DbgPrintEx(0, 0, "\n\nMJ_WRITE Received!\n");

    if (NULL == Buffer || 0 == Length) {
        DbgPrintEx(0, 0, "Failed to Received Data!\n");
        Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
        Irp->IoStatus.Information = 0;

        return Irp->IoStatus.Status;
    }
    DbgPrintEx(0, 0, "Data to be written received: %.*s\n", Length, Buffer);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = Length;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

NTSTATUS IrpRead(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    UNREFERENCED_PARAMETER(DeviceObject);
    PIO_STACK_LOCATION  Stack   = IoGetCurrentIrpStackLocation(Irp);
    PCHAR               Buffer  = Irp->UserBuffer;
    ULONG               Length  = Stack->Parameters.Write.Length;

    DbgPrintEx(0, 0, "MJ_READ Received!\n");
    if (NULL == Buffer || 0 == Length) {
        DbgPrintEx(0, 0, "Failed to Received Data!\n");
        Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
        Irp->IoStatus.Information = 0;
        
        return Irp->IoStatus.Status;
    }
    DbgPrintEx(0, 0, "Data to be read received: %.*s\n", Length, Buffer);
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = Length;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Irp->IoStatus.Status;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT PDrvObj, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\MyDevice");
    UNICODE_STRING SymName = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
    PDEVICE_OBJECT DeviceObject;
    NTSTATUS Status;

    Status = IoCreateDevice(
        PDrvObj,
        0,
        &DeviceName,
        FILE_DEVICE_UNKNOWN,
        0,
        FALSE,
        &DeviceObject
    );
    if (!NT_SUCCESS(Status)) {
        DbgPrintEx(0, 0, "Failed to Create IO Device!\n");
        return Status;
    }

    Status = IoCreateSymbolicLink(
        &SymName,
        &DeviceName
    );
    if (!NT_SUCCESS(Status)) {
        DbgPrintEx(0, 0, "Failed to Create Smybolic Link!\n");
        return Status;
    }
    PDrvObj->MajorFunction[IRP_MJ_CREATE]   = IrpCreate;
    PDrvObj->MajorFunction[IRP_MJ_WRITE]    = IrpWrite;
    PDrvObj->MajorFunction[IRP_MJ_READ]     = IrpRead;

    //IoCreateDevice
    //IOCreateSym

    return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT PDrvObj) {
    UNICODE_STRING SymName = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
    DbgPrintEx(0, 0, "Unloading Driver...\n");

    IoDeleteSymbolicLink(&SymName);
    IoDeleteDevice(PDrvObj->DeviceObject);

    return STATUS_SUCCESS;
}
