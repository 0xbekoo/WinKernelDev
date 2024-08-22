#pragma once


#include <ntddk.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT PDrvObj, PUNICODE_STRING RegistryPath);
NTSTATUS UnloadDriver(PDRIVER_OBJECT PDrvObj);