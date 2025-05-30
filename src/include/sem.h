#ifndef SEM_H
#define SEM_H

#include "dyn_array.h"
#include "grammar.h"
#include "rtt.h"
#include "ds/smap.h"
#include "ds/arrays.h"

typedef struct {
        const char *id;
        RTT *rtt;
} Sem_Sym;

DYN_ARRAY_TYPE(SMap, SMap_Array);

typedef struct {
        SMap_Array scope;
        Str_Array errs;
} Sem_Scope;

Sem_Scope semantic_analyze(Program *p);

#endif // SEM_H
