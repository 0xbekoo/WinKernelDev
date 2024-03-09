#pragma once
#include <ntifs.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegistry);
NTSTATUS UnloadDriver(PDRIVER_OBJECT pDrvObj);