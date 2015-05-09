/* Created on 2/19/15 by Eric Gonzalez */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFERSIZE 1024

/* main function starts here */
int main(int argc, char *argv[])
{
    int sourceFileDesc, destFileDesc;
    ssize_t bytesRead = 0, bytesWritten = 0, totalBytesRead = 0;

    char readBuffer[BUFFERSIZE] = {0};

    sourceFileDesc = open(argv[1], O_RDONLY);

    if (sourceFileDesc < 0)
    {
        perror("Error opening source file");
        close(sourceFileDesc);
        return -1;
    }


    destFileDesc = open(argv[2], O_RDWR|O_EXCL|O_CREAT, 00644);

    if (destFileDesc < 0)
    {
        perror("Error creating destination file");
        close(sourceFileDesc);
        close(destFileDesc);
        return -1;
    }

    do {
        /* ssize_t read(int fd, void *buf, size_t count) */
        bytesRead = read(sourceFileDesc, readBuffer, BUFFERSIZE-1);
        if (bytesRead < 0)
        {
            perror("Error reading from file");
            close(sourceFileDesc);
            close(destFileDesc);
            return -1;
        }

        /* ssize_t write(int fd, const void *buf, size_t count) */
        bytesWritten = write(destFileDesc, readBuffer, bytesRead);
        if (bytesWritten < 0)
        {
            perror("Error writing to file");
            close(sourceFileDesc);
            close(destFileDesc);
            return -1;
        }

        /* update the total number bytes read from file */
        totalBytesRead += bytesWritten;

    } while (bytesRead != 0);

    printf("File Copy successful, %u bytes copied.\n\n", totalBytesRead);

    close(sourceFileDesc);
    close(destFileDesc);

    return 0;
}

