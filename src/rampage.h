#ifndef __LIBRAMPAGE_RAMPAGE_H__
#define __LIBRAMPAGE_RAMPAGE_H__

#include "process.h"


typedef struct RampageMatches_s {
    size_t *matches;
    size_t size;
} RampageMatches_t;

extern double rampage_benchmark(Process_t *proc, int size);

#endif