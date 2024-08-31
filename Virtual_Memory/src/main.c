// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: Virtual Memory                                 ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#pragma warning(disable: 4996)

#include "main.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);
	SIZE_T MemorySize = 0x1000;
	PVOID VirtualAddress = NULL;
	PVOID MappedAddress = NULL;
	PMDL MDL = NULL;

	VirtualAddress = ExAllocatePool(NonPagedPool, MemorySize);
	if (NULL == VirtualAddress) {
		DbgPrintEx(0, 0, "Failed to Allocate Memory!\n");
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	DbgPrintEx(0, 0, "ALlocated Memory at 0x%p\n", VirtualAddress);

	MDL = IoAllocateMdl(VirtualAddress, (ULONG)MemorySize, FALSE, FALSE, NULL);
	if (NULL == MDL) {
		DbgPrintEx(0, 0, "Failed to MDL Creation!\n");
		ExFreePool(VirtualAddress);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	
	MmBuildMdlForNonPagedPool(MDL);

	MappedAddress = MmMapLockedPagesSpecifyCache(MDL, KernelMode, MmCached, NULL, FALSE, NormalPagePriority);
	if (NULL == MappedAddress) {
		DbgPrintEx(0, 0, "Failed to Virtual Address Mapping!\n");
		IoFreeMdl(MDL);
		ExFreePool(VirtualAddress);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	DbgPrintEx(0, 0, "Virtual Address mapping is successful! Address: 0x%p\n", MappedAddress);
	
	MmUnmapLockedPages(MappedAddress, MDL);
	IoFreeMdl(MDL);
	ExFreePool(VirtualAddress);

	DriverObject->DriverUnload = UnloadDriver;
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrintEx(0, 0, "Driver Unloaded\n");
	return STATUS_SUCCESS;
}
