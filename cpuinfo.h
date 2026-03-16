#ifndef CPUINFO_H
#define CPUINFO_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
	size_t size;
	size_t share;
} CacheInfo;

void get_min_cache_size(int, CacheInfo*);

#endif // CPUINFO_H
