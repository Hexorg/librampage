#include "rampage.h"
#include "scan.h"
#include "memmap/procmap.h"
#include <time.h>
#include <stdlib.h>

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

double rampage_benchmark(Process_t *proc, int size) {
    // MatchConditions_t search;
    procmap_update(&proc->memmap);
    uint64_t data = rand(); 
    clock_t start = clock();
    size_t read_size = 0;
    while (read_size < 10000000000) {
        for (size_t i=0; i<proc->memmap.segment_count; i++) {
            if (likely(PROCMAP_IS_WRITE(proc->memmap.segments[i]))) {
                size_t len = proc->memmap.segments[i].end - proc->memmap.segments[i].start;
                ProcessMemory_t *memory = Process_get_bytes(proc, proc->memmap.segments[i].start, len);    
                if (memory->bytes_read > 0) {
                    if (memory->buffer == NULL) {
                        printf("WTF?!\n");
                    }
                    read_size += memory->bytes_read;
                    MatchedOffsets_t * result = scan_exact(memory->buffer, memory->bytes_read, (uint8_t*) &data, size, 1);
                    // MatchedOffsets_t * result = scan_exact((uint8_t*)proc.memmap.segments, proc.memmap.segment_count * sizeof(ProcMapEntry_t), (uint8_t*) &data, size, 1);
                    free_offsets(result);
                } else {
                    printf("Couldn't read %s segment: perms: %x\n", proc->memmap.segments[i].pathname, proc->memmap.segments[i].perms);
                }
                Process_free_bytes(memory);
            }
        }
    }
    clock_t end = clock();
    return (double) read_size / (((double) (end - start)) / CLOCKS_PER_SEC);
}