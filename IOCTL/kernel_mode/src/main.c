// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: IOCTL - Kernel Mode                            ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#include "main.h"

PVOID GlobalMemoryAddr = NULL;

NTSTATUS IoCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
	NTSTATUS status = STATUS_SUCCESS;

	switch (Stack->MajorFunction) {

	case IRP_MJ_CREATE:
		Irp->IoStatus.Status = STATUS_SUCCESS;
		break;

	case IRP_MJ_CLOSE:
		Irp->IoStatus.Status = STATUS_SUCCESS;
		break;

	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return Irp->IoStatus.Status;
}

NTSTATUS IoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
	UNREFERENCED_PARAMETER(DeviceObject);

	PIO_STACK_LOCATION Stack = IoGetCurrentIrpStackLocation(Irp);
	PVOID Data = Irp->AssociatedIrp.SystemBuffer;
	PCHAR UserBuffer = (PCHAR)Data;
	ULONG OutLength = Stack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG Length = Stack->Parameters.DeviceIoControl.InputBufferLength;

	ULONG Tag = TAG;

	switch (Stack->Parameters.DeviceIoControl.IoControlCode) {

	case IOCTL_MEM_ALLOCATE:
		if (NULL == UserBuffer || 0 == Length) {
			Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
			Irp->IoStatus.Information = 0;
			break;
		}
		GlobalMemoryAddr = ExAllocatePool2(POOL_FLAG_NON_PAGED, Length, Tag);
		if (NULL == GlobalMemoryAddr) {
			Irp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
			Irp->IoStatus.Information = 0;
			break;
		}
		RtlCopyMemory(GlobalMemoryAddr, UserBuffer, Length);

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = Length;
		break;

	case IOCTL_MEM_READ:
		RtlCopyMemory(UserBuffer, GlobalMemoryAddr, OutLength);

		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = OutLength;
		break;

	default:
		Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
		Irp->IoStatus.Information = 0;
		break;
	}
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
		DbgPrintEx(0, 0, "Failed to Create I/O Device!\n");
		return Status;
	}

	Status = IoCreateSymbolicLink(&SymName, &DeviceName);
	if (!NT_SUCCESS(Status)) {
		DbgPrintEx(0, 0, "Failed to Create Symbolic Link!\n");
		return Status;
	}
	DriverObject->MajorFunction[IRP_MJ_CREATE] = IoCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = IoCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IoControl;
	DriverObject->DriverUnload = UnloadDriver;

	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING SymName = RTL_CONSTANT_STRING(L"\\??\\MyDevice");
	DbgPrintEx(0, 0, "Unloading the Driver...\n");

	IoDeleteSymbolicLink(&SymName);
	IoDeleteDevice(DriverObject->DeviceObject);

	return STATUS_SUCCESS;
}