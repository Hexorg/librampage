#include "os_specific.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void readMemFile(FILE *mem, ProcessMemory_t *memory) {
    if (fseek(mem, (long int) memory->address, SEEK_SET) != 0) {
        printf("%s:%d %s(): Faliure while reading mem file", __FILE__, __LINE__, __func__);
        if (ferror(mem)) {
            printf(": error %s\n", strerror(errno));
        } else {
            printf("\n");
        }
        return;
    }
    if (fflush(mem)) {
        printf("%s:%d %s() Couldn't flush the mem file\n", __FILE__, __LINE__, __func__);
    }
    memory->bytes_read = fread(memory->buffer, 1, memory->buffer_length, mem);
}