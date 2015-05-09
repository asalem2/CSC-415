/*Author: Eric Gonzalez
  Created: 3/6/15
  Shell Program  */

#include <stdio.h>
#include <windows.h>
#include <string.h>

#define BUFFERSIZE 1024

int main() {
    int exit = 0;
    int childID = 0;
    int status = 0;
    char rawInput[BUFFERSIZE];
    char* commandFull = NULL;
    char* nextToken = NULL;

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));


    while (1) {
        printf("Myshell> ");
        fgets(rawInput, BUFFERSIZE, stdin);
        commandFull = strtok_s(rawInput, "\n", &nextToken);
        if (strcmp(commandFull, "exit") == 0) {
            return 0;
        }

        if (!CreateProcess(NULL, commandFull, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            printf("Error creating new process\n");
        }
        else {
            WaitForSingleObject(pi.hProcess, INFINITE);
        }
    }

return 0;
}