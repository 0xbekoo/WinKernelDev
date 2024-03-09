#pragma warning (disable: 4100 4996 4047)
#pragma once 
#include "allocatememory.h"
#define Tag 'beko'

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegistry) {
	pDrvObj->DriverUnload = UnloadDriver;
	UNICODE_STRING srcStr = RTL_CONSTANT_STRING(L"bekoo");
	UNICODE_STRING rBuffer = { 0 };
	ULONG lenght;
	lenght = srcStr.Length;

	/*
		PVOID NTAPI ExAllocatePoolWithTag (
			_In_ __drv_strictTypeMatch(__drv_typeExpr) POOL_TYPE PoolType,
			_In_ SIZE_T NumberOfBytes,
			_In_ ULONG Tag
		);
	*/
	rBuffer.Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, lenght, Tag);
	if (rBuffer.Buffer == NULL) {
		DbgPrintEx(0, 0, "Failed to Allocate Memory!");
		return -1;
	}
	DbgPrintEx(0, 0, "Allocated (%d-byte) Memory", lenght);

	RtlCopyMemory(rBuffer.Buffer, srcStr.Buffer, lenght);
	DbgPrintEx(0, 0, "'%wZ' string in 0x%p Address \r\n", &srcStr, rBuffer.Buffer);
	ExFreePool(rBuffer.Buffer);
	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDrvObj) {
	DbgPrintEx(0, 0, "Now Exit!");
	return STATUS_SUCCESS;
}