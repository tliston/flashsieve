#include "cpuinfo.h"

#define DEFAULT_L1_CACHE_SIZE (32 * 1024)
#define DEFAULT_L2_CACHE_SIZE (256 * 1024)

inline static size_t read_sysfs_cache_size(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file)
        return 0;
    size_t size = 0;
    char suffix = 0;
    if (fscanf(file, "%zu%c", &size, &suffix) > 0) {
        if (suffix == 'K' || suffix == 'k')
            size *= 1024;
        else if (suffix == 'M' || suffix == 'm')
            size *= 1024 * 1024;
    }
    fclose(file);
    return size;
}

size_t get_min_cache_size(int clevel) {
    size_t min_cachesize = SIZE_MAX;
    char path_level[256], path_type[256], path_size[256];

    for (int cpu = 0; cpu < 256; cpu++) {
        for (int index = 0; index < 4; index++) {
            snprintf(path_level, sizeof(path_level), "/sys/devices/system/cpu/cpu%d/cache/index%d/level", cpu, index);
            FILE *f_level = fopen(path_level, "r");
            if (!f_level)
                break;

            int level = 0;
            if (fscanf(f_level, "%d", &level)) {
                if (level == clevel) {
                    snprintf(path_type, sizeof(path_type), "/sys/devices/system/cpu/cpu%d/cache/index%d/type", cpu, index);
                    FILE *f_type = fopen(path_type, "r");
                    if (f_type) {
                        char type[32] = { 0 };
                        if (fscanf(f_type, "%31s", type)) {
                            if (clevel == 1) {
                                if (strcmp(type, "Data") == 0) {
                                    snprintf(path_size, sizeof(path_size),
                                             "/sys/devices/system/cpu/cpu%d/cache/index%d/size", cpu, index);
                                    size_t size = read_sysfs_cache_size(path_size);
                                    if (size > 0 && size < min_cachesize)
                                        min_cachesize = size;
                                }
                            }
                            if (clevel == 2) {
                                if ((strcmp(type, "Data") == 0) || (strcmp(type, "Unified") == 0)) {
                                    snprintf(path_size, sizeof(path_size),
                                             "/sys/devices/system/cpu/cpu%d/cache/index%d/size", cpu, index);
                                    size_t size = read_sysfs_cache_size(path_size);
                                    if (size > 0 && size < min_cachesize)
                                        min_cachesize = size;
                                }
                            }
                        }
                        fclose(f_type);
                    }
                }
            }
            fclose(f_level);
        }
    }
    if (clevel == 1)
        return (min_cachesize != SIZE_MAX) ? min_cachesize : DEFAULT_L1_CACHE_SIZE;
    else
        return (min_cachesize != SIZE_MAX) ? min_cachesize : DEFAULT_L2_CACHE_SIZE;
}
