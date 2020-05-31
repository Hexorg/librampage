#include "procmap.h"
#include <string.h>
#include <stdlib.h>

#define SEGMENT_ALLOCATION_SIZE 1024

void parse_line(char *const line, const size_t line_length, ProcMapEntry_t *result);
size_t binary_search(ProcMapFile_t *const procmap, const size_t start, const size_t end, void *const p);

ProcMapFile_t *procmap_open(char *const filepath) {
    ProcMapFile_t *result = malloc(sizeof(ProcMapFile_t));
    result->file = fopen(filepath, "r");
    result->segments = NULL;
    result->segment_count = 0;
    procmap_update(result);
    return result;
}

void procmap_close(ProcMapFile_t *procmap) {
    for (size_t i=0; i<procmap->segment_count; i++) {
        if (procmap->segments[i].pathname != NULL) {
            free(procmap->segments[i].pathname);
        }
    }
    free(procmap->segments);
    fclose(procmap->file);
    procmap->segments = NULL;
    procmap->file = NULL;
    procmap->segment_count = 0;
}

void procmap_update(ProcMapFile_t *procmap) {
    // printf("Old procmap segments address is 0x%lx\n", (unsigned long) procmap->segments);
    for (size_t i=0; i<procmap->segment_count; i++) {
        // printf("%s checking if %ldth segment has pathname to free it\n", __func__, i);
        if (procmap->segments[i].pathname != NULL) {
            free(procmap->segments[i].pathname);
        }
    }
    if (procmap->segments != NULL) {
        free(procmap->segments);
        procmap->segments = NULL;
    }
    char *line = NULL;
    size_t line_length;
    size_t pos = 0;
    fseek(procmap->file, 0, SEEK_SET);
    while (getline(&line, &line_length, procmap->file) != -1) {
        if (pos % SEGMENT_ALLOCATION_SIZE == 0) {
            size_t new_size = pos + SEGMENT_ALLOCATION_SIZE;
            ProcMapEntry_t *new_segment_list = malloc(sizeof(ProcMapEntry_t) * new_size);
            if (procmap->segments != NULL) {
                for (size_t i=0; i<pos; i++) {
                    new_segment_list[i] = procmap->segments[i];
                }
                // printf("Freeing old procmap segments address 0x%lx\n", (unsigned long) procmap->segments);
                free(procmap->segments);
            }
            // printf("New procmap segments address 0x%lx\n", (unsigned long) new_segment_list);
            procmap->segments = new_segment_list;   
        }
        parse_line(line, line_length, procmap->segments + pos);
        free(line);
        line = NULL;
        line_length = 0;
        pos++;
    }
    free(line);
    // printf("New segment size is %ld\n", pos);
    procmap->segment_count = pos;
}


void parse_line(char *line, size_t line_length, ProcMapEntry_t *result) {
    const char delim[] = " :";
    const char dash[] = "-";
    char *saveptr = NULL;
    char *token = strtok_r(line, dash, &saveptr);
    result->start = (void *)strtoul(token, NULL, 16);
    token = strtok_r(NULL, delim, &saveptr);
    result->end = (void *)strtoul(token, NULL, 16);
    token = strtok_r(NULL, delim, &saveptr);
    result->perms = 0;
    if (token[0] == 'r') {
        result->perms |= PROCMAP_READ;
    }
    if (token[1] == 'w') {
        result->perms |= PROCMAP_WRITE;
    }
    if (token[2] == 'x') {
        result->perms |= PROCMAP_EXECUTE;
    }
    if (token[3] == 'p') {
        result->perms |= PROCMAP_PRIVATE;
    } else if (token[3] == 's') {
        result->perms |= PROCMAP_SHARED;
    }
    token = strtok_r(NULL, delim, &saveptr);
    result->offset = strtoul(token, NULL, 16);
    token = strtok_r(NULL, delim, &saveptr);
    result->dev_major = strtoul(token, NULL, 10);
    token = strtok_r(NULL, delim, &saveptr);
    result->dev_minor = strtoul(token, NULL, 10);
    token = strtok_r(NULL, delim, &saveptr);
    result->inode = strtoul(token, NULL, 10);


    token = strtok_r(NULL, delim, &saveptr);
    if (strlen(saveptr) > 0) {
        token[strlen(token)] = ' ';
    }
    
    if (token) {
        size_t path_len = strlen(token);
        token[path_len-1] = '\0';
        if (path_len != 0) {
            result->pathname = malloc(path_len);
            memcpy(result->pathname, token, path_len);
        } else {
            result->pathname = NULL;
        }
    }
}


int procmap_is_in(ProcMapFile_t *const procmap, const size_t segment_index, void *const p) {
    return procmap->segments[segment_index].start <= p && p < procmap->segments[segment_index].end;
}

int procmap_greater_than(ProcMapFile_t *const procmap, const size_t segment_index, void *const p) {
    return p >= procmap->segments[segment_index].end;
}

int procmap_less_than(ProcMapFile_t *const procmap, const size_t segment_index, void *const p) {
    return p < procmap->segments[segment_index].start;
}

size_t procmap_find_segmentid(ProcMapFile_t *const procmap, void *const p) {
    return binary_search(procmap, 0, procmap->segment_count, p);
}

size_t binary_search(ProcMapFile_t *const procmap, const size_t start, const size_t end, void *const p) {
    size_t middle = start + (end - start) / 2;
    if (procmap_is_in(procmap, middle, p) || middle == start) {
        return middle;
    } else if (procmap_less_than(procmap, middle, p)) {
        return binary_search(procmap, start, middle, p);
    } else {
        return binary_search(procmap, middle, end, p);
    }
}