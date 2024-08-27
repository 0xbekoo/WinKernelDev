#include <ntddk.h>

#define TAG 'beko'

#define IOCTL_MEM_ALLOCATE \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_MEM_READ \
	CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS DriverEntry(
		_In_ PDRIVER_OBJECT DriverObject, 
		_In_ PUNICODE_STRING RegistryPath
);

NTSTATUS UnloadDriver(
		_In_ PDRIVER_OBJECT DriverObject
);

NTSTATUS IoCreateClose(
		_In_ PDEVICE_OBJECT DeviceObject, 
		_In_ PIRP Irp
);

NTSTATUS IoControl(
		_In_ PDEVICE_OBJECT DeviceObject, 
		_In_ PIRP Irp
);