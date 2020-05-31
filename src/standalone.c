#include "process.h"
#include "rampage.h"
#include <stdio.h>
#include <stdlib.h>

// int test_ptrace(int argc, char *argv[]) {
//     int pid = atoi(argv[1]);
//     void *block_start = (void *)0x7fff61a43000;
//     void *block_end = (void *)0x7fff61a64000;
//     Process_t* process = Process_new(pid);
//     uint8_t *data = Process_get_bytes(process, block_start, block_end-block_start);

//     int value = 12;
//     MatchConditions_t match;
//     match.data = (uint8_t *)&value;
//     match.data_length = 4;
//     match.alignment = 1;
//     match.is_float = 0;
//     match.precision = 1;
//     MatchedOffsets_t *result = scan(data, 135168, &match);
//     printf("Found %ld matches\n", result->size);
//     for (size_t i=0; i<result->size; i++) {
//         printf("\t0x%lx\n", result->matchbuffer[i]+0x7ffbfffdd000);
//     }

//     // printf("Read value is %d\n", value);
//     Process_free(&process);
//     return 0;
// }

#include <stdlib.h>

void benchmark(Process_t *proc) {
    printf("\"commit\", \"8-bit exact\", \"16-bit exact\", \"32-bit exact\", \"64-bit exact\"\n");
    printf("TBF, %lf, %lf, %lf, %lf\n", rampage_benchmark(proc, 1), rampage_benchmark(proc, 2), rampage_benchmark(proc, 4), rampage_benchmark(proc, 8));
}

int main(int argc, char *argv[]) {
    int target_pid = atoi(argv[1]);
    Process_t *proc = Process_new(target_pid);
    benchmark(proc);
    Process_free(proc);
    return 0;
}