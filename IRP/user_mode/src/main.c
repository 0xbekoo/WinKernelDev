// ╔═══════════════════════════════════════════════════════════╗
// ║                                                           ║
// ║            ⚠️  Windows Kernel Development!  ⚠️           ║
// ║                                                           ║
// ║  @author: bekoo                                           ║
// ║  @website: 0xbekoo.github.io                              ║
// ║  @Project: I/O Request Packets (IRP) - User Mode          ║
// ║                                                           ║
// ╚═══════════════════════════════════════════════════════════╝

#include "main.h"

int main(int argc, char* argv[]) {
	HANDLE HandleDevice	= NULL;
	CHAR Buffer[] = "Hello, kernel!";
	DWORD BytesWritten = 0;
	DWORD BytesRead	= 0;
	BOOL Status	= 0;

	HandleDevice = CreateFile(DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == HandleDevice) {
		printf("Failed to Open Device! Error Code: 0x%lx\n", GetLastError());
		return -1;
	}

	Status = WriteFile(HandleDevice, Buffer, (DWORD)sizeof(Buffer), &BytesWritten, NULL);
	if (!Status) {
		printf("Failed to Write Data!\n");
		CloseHandle(HandleDevice);
		return -1;
	}

	return 0;
}