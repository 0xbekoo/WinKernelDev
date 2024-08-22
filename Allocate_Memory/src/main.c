// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: Allocate Memory in Kernel Pool                 ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#pragma warning (disable: 4100 4996 4047)
#include "main.h"

#define Tag 'beko'

NTSTATUS DriverEntry(PDRIVER_OBJECT PDrvObj, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);
	UNICODE_STRING Buffer		= RTL_CONSTANT_STRING(L"Hello, Kernel!");
	UNICODE_STRING RemoteBuffer = { 0 };
	ULONG Length = Buffer.Length;

	RemoteBuffer.Buffer = (PWCHAR)ExAllocatePoolWithTag(
		NonPagedPool,
		Length,
		Tag
	);
	if (NULL == RemoteBuffer.Buffer) {
		DbgPrintEx(0, 0, "Failed to Allocate Pool!\n");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlCopyMemory(RemoteBuffer.Buffer, Buffer.Buffer, Length);

	DbgPrintEx(0, 0, "'%wZ' string in 0x%p Address \r\n", &Buffer, RemoteBuffer.Buffer);

	ExFreePool(RemoteBuffer.Buffer);
	PDrvObj->DriverUnload = UnloadDriver;

	return STATUS_SUCCESS;
}
NTSTATUS UnloadDriver(PDRIVER_OBJECT PDrvObj) {
	DbgPrintEx(0, 0, "Unloading Driver...\n");
	return STATUS_SUCCESS;
}