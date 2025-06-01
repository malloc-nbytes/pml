#ifndef SEM_H
#define SEM_H

#include "dyn_array.h"
#include "grammar.h"
#include "rtt.h"
#include "ds/smap.h"
#include "ds/arrays.h"

typedef struct {
        char *id;
        RTT *rtt;
} Sem_Sym;

typedef struct {
        SMap_Array scope;
        Str_Array errs;
} Sem_Scope;

Sem_Scope semantic_analyze(Program *p);
Sem_Sym *sem_scope_lookup(const Sem_Scope *scope, const char *id);
void sem_add_sym_to_scope(Sem_Scope *ss, Sem_Sym *sym);

#endif // SEM_H
