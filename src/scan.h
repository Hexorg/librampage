#ifndef __CSCAN_SCAN_H__
#define __CSCAN_SCAN_H__

#include <inttypes.h>

typedef struct MatchConditions_s {
    uint8_t *data;
    int data_length;
    int alignment;
    int is_float;
    float precision;
} MatchConditions_t;

typedef struct MatchedOffsets_s {
    size_t *matchbuffer;
    size_t size;
} MatchedOffsets_t;

MatchedOffsets_t *scan_exact(uint8_t *buffer, size_t buffer_size, uint8_t *data, size_t data_size, int increment);
MatchedOffsets_t *filter(uint8_t *buffer, size_t size, MatchConditions_t *match, MatchedOffsets_t *matches);
void free_offsets(MatchedOffsets_t *offsets);

#endif