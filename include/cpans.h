#ifndef __CPANS_H__
#define __CPANS_H__

#include <sys/types.h>

typedef struct {
    char uri[80];
    float version;
    time_t updatetime;
    unsigned int modulenum; // module num
} sourcemeta_t;

typedef struct {
    char name[100];
    char version[12];
    char path[200];
    char reserved[100];
} moduledata_t;

typedef moduledata_t* modulelist_t;

#endif
