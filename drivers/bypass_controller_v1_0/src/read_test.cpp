#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define RESERVED_MEMORY_OFFSET  0x1B5800000      /* Offset is 7000M */

int main() {
    int fd;
    char *reserved_memory;
    char buffer[13];

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    /* Returns a pointer to the 7000M point in /dev/mem - the start of my reserved memory. Only mapping 4096 bytes. */
    reserved_memory = (char *) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fd, RESERVED_MEMORY_OFFSET);
    if (reserved_memory == MAP_FAILED) {
        printf("Failed to creating mapping.\n");
        printf("ERRNO: %s\n", strerror(errno));
        return -1;
    }
    snprintf(buffer, 13, "%s", reserved_memory);
    printf("%s\n", buffer);
    return 0;
}
