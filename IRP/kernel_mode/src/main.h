#pragma once

#include <ntddk.h>

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject, 
	_In_ PUNICODE_STRING RegistryPath
);

NTSTATUS UnloadDriver(
	_In_ PDRIVER_OBJECT DriverObject
);

NTSTATUS IrpCreateClose(
	_In_ PDEVICE_OBJECT DeviceObject,
	_In_ PIRP Irp
);

NTSTATUS IrpWrite(
	_In_ PDEVICE_OBJECT DeviceObject,
	_In_ PIRP Irp
);