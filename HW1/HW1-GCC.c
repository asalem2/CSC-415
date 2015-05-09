#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define name "Eric Gonzalez\n"

int main(void) {

    char str[29];

    sprintf(str, "Hello 415, I am %s", name);

    write(1, str, 29);

    return 0;
}
