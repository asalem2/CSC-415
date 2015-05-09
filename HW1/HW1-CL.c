#include <stdio.h>
#include <windows.h>

#define name "Eric Gonzalez"

int main(void){
    DWORD written;

    char str[29];

    sprintf(str, "Hello 415, I am %s\n", name);

    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), str, 29, &written, NULL);

    return 0;
}
