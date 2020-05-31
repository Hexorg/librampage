#ifndef __LIBRAMPAGE_PROCMAP_H__
#define __LIBRAMPAGE_PROCMAP_H__

#include <stdio.h>

#define PROCMAP_READ    0b00001
#define PROCMAP_WRITE   0b00010
#define PROCMAP_EXECUTE 0b00100
#define PROCMAP_PRIVATE 0b01000
#define PROCMAP_SHARED  0x10000

#define PROCMAP_IS_READ(segment) ((segment).perms & PROCMAP_READ)
#define PROCMAP_IS_WRITE(segment) ((segment).perms & PROCMAP_WRITE)
#define PROCMAP_IS_EXECUTE(segment) ((segment).perms & PROCMAP_EXECUTE)
#define PROCMAP_IS_PRIVATE(segment) ((segment).perms & PROCMAP_PRIVATE)
#define PROCMAP_IS_SHARED(segment) ((segment).perms & PROCMAP_SHARED)

typedef struct ProcMapEntry_s {
    void *start;
    void *end;
    int perms;
    size_t offset;
    unsigned int dev_major;
    unsigned int dev_minor;
    unsigned int inode;
    char *pathname;
} ProcMapEntry_t;

typedef struct ProcMapFile_s {
    FILE *file;
    ProcMapEntry_t *segments;
    size_t segment_count;
} ProcMapFile_t;

ProcMapFile_t *procmap_open(char *const filepath);
int procmap_is_in(ProcMapFile_t *const procmap, const size_t segment_index, void *const p);
int procmap_less_than(ProcMapFile_t *const procmap, const size_t segment_index, void *const p);
int procmap_greater_than(ProcMapFile_t *const procmap, const size_t segment_index, void *const p);
size_t procmap_find_segmentid(ProcMapFile_t *const procmap, void *const p);
void procmap_update(ProcMapFile_t *procmap);
void procmap_close(ProcMapFile_t *procmap);

#endif