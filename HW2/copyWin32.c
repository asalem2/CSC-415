//Created 2/19/15 by Eric Gonzalez

#include <stdio.h>
#include <windows.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]){

	HANDLE sourceFileHandle, destFileHandle;
	DWORD bytesRead = 0, bytesWritten = 0, totalBytesRead = 0;
	char readBuffer[BUFFERSIZE] = { 0 };

	sourceFileHandle = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (sourceFileHandle == INVALID_HANDLE_VALUE){
		printf("Error opening source file: %s\nTerminating Program.",argv[1]);
		CloseHandle(sourceFileHandle);
		return -1;
	}

	destFileHandle = CreateFile(argv[2], GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

	if (destFileHandle == INVALID_HANDLE_VALUE){
		printf("Error opening source file: %s\nTerminating Program.",argv[2]);
		CloseHandle(sourceFileHandle);
		CloseHandle(destFileHandle);
		return -1;
	}

	do{
		/* ReadFile(fileHandle, readBuffer pointer, max number of bytes to read,
		pointer to variable that counts bytesRead, OverLapped pointer */
		if (ReadFile(sourceFileHandle, readBuffer, BUFFERSIZE - 1, &bytesRead, NULL) == FALSE){
			printf("Error reading source file.\nTerminating program.\n");
			CloseHandle(sourceFileHandle);
			CloseHandle(destFileHandle);
			return -1;
		}

		/* WriteFile(fileHandle, writeBuffer pointer, number of bytes to write,
		pointer to variable that counts bytesWritten, OverLapped pointer */
		if (WriteFile(destFileHandle, readBuffer, bytesRead, &bytesWritten, NULL) == FALSE){
			printf("Error writing file.\nTerminating program.\n");
			CloseHandle(sourceFileHandle);
			CloseHandle(destFileHandle);
			return -1;
		}

		/* update the total number bytes read from file */
		totalBytesRead += bytesWritten;

	}
	while (bytesRead != 0);

	printf("File Copy successful, %u bytes copied.\n\n", totalBytesRead);

	/* although handles will be closed at the end of program, it is recommended
	good practice to close them in code anyway */
	CloseHandle(sourceFileHandle);
	CloseHandle(destFileHandle);

	//system("pause");
	return 0;
}
