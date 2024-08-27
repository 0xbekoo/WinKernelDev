#include <ntddk.h>

#define Tag 'beko'

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
);

NTSTATUS UnloadDriver(
	_In_ PDRIVER_OBJECT DriverObject
);