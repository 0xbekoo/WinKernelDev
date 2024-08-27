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

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);
	UNICODE_STRING Buffer = RTL_CONSTANT_STRING(L"Hello, Kernel!");
	UNICODE_STRING MemoryAddress = { 0 };
	ULONG Length = Buffer.Length;

	MemoryAddress.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, Length, Tag);
	if (NULL == MemoryAddress.Buffer) {
		DbgPrintEx(0, 0, "Failed to Allocate Memory!\n");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	RtlCopyMemory(MemoryAddress.Buffer, Buffer.Buffer, Length);

	DbgPrintEx(0, 0, "'%wZ' string in 0x%p Address \r\n", &Buffer, MemoryAddress.Buffer);

	ExFreePool(MemoryAddress.Buffer);
	DriverObject->DriverUnload = UnloadDriver;

	return STATUS_SUCCESS;
}
NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject) {
	DbgPrintEx(0, 0, "Unloading Driver...\n");
	return STATUS_SUCCESS;
}