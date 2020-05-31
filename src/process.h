#ifndef __CSCAN_PROCESS_H__
#define __CSCAN_PROCESS_H__

#include <sys/types.h>
#include <stdio.h>
#include <inttypes.h>

#include "memmap/procmap.h"

typedef struct Process_s {
    pid_t pid;
    ProcMapFile_t memmap;
    FILE *mem;
} Process_t;

typedef struct ProcessMemory_s {
    void *address;
    uint8_t *buffer;
    size_t buffer_length;
    size_t bytes_read;
} ProcessMemory_t;

// typedef enum ProcessFlags_s {
//     ATTACHED = 1,
//     RUNNING = 2,
//     EXITED = 4
// } ProcessFlag_t;

// #define ISATTACHED(process) (process->flags & ATTACHED)
// #define SETATTACHED(process) (process->flags |= ATTACHED)
// #define SETDETTACHED(process) (process->flags &= ~ATTACHED)

// #define ISRUNNING(process) (process->flags & RUNNING)
// #define SETRUNNING(process) (process->flags |= RUNNING)
// #define SETSTOPPED(process) (process->flags &= ~RUNNING)

extern Process_t *Process_new(const int pid);
// extern void Process_attach(Process_t *process);
// extern void Process_detach(Process_t *process);
// extern int Process_wait(Process_t *process, int is_blocking);
// extern void Process_stop(Process_t *process);
// extern void Process_continue(Process_t *process);
// extern long Process_get_word(Process_t *process, void *address);
// extern void Process_set_word(Process_t *process, void *address, long data);
extern ProcessMemory_t *Process_get_bytes(Process_t *process, void *const address, const size_t size);
extern void Process_free_bytes(ProcessMemory_t *bytes);
extern void Process_free(Process_t *process);
#endif