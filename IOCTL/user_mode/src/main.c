// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: IOCTL - User Mode  						       ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#include "main.h"

int main(int argc, char* argv[]) {
	HANDLE HandleDevice = NULL;
	CHAR InBuffer[] = "DAK DAK WHO'S IT?";
	CHAR OutBuffer[sizeof(InBuffer)] = { 0 };
	DWORD InputBytesReturned = 0;
	DWORD OutputBytesReturned = 0;
	BOOL Result = 0;

	HandleDevice = CreateFile(DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == HandleDevice) {
		printf("Failed to connect Driver! Error Code: 0x%lx\n", GetLastError());
		return -1;
	}

	Result = DeviceIoControl(HandleDevice, IOCTL_MEM_ALLOCATE, InBuffer, sizeof(InBuffer), NULL, 0, &InputBytesReturned, NULL);
	if (!Result) {
		printf("Failed to Allocate Memory! Error Code: 0x%lx\n", GetLastError());
		return -1;
	}

	Result = DeviceIoControl(HandleDevice, IOCTL_MEM_READ, NULL, 0, OutBuffer, sizeof(OutBuffer), &OutputBytesReturned, 0);
	if (!Result) {
		printf("Failed to get Data!\n");
		return -1;
	}
	printf("Output Buffer: %s\n", OutBuffer);
	
	return 0;
}