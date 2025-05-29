#ifndef SMAP_H
#define SMAP_H

#include <stddef.h>

#define SMAP_DEF_BUCKET_SZ 256

typedef void (*vdestr_sig)(void *);
typedef unsigned long (*hash_sig)(const char *);

typedef struct SMap_Node {
        char *k;
        void *v;
        struct SMap_Node *n;
} SMap_Node;

typedef struct {
        struct {
                SMap_Node **buckets;
                size_t len;
                size_t cap;
        } tbl;
        hash_sig hash;
        vdestr_sig vdestr;
} SMap;

SMap smap_create(hash_sig hash, vdestr_sig vdestr);
void smap_insert(SMap *m, char *k, void *v);
void smap_free(SMap *m);
void *smap_get(const SMap *m, const char *k);
int smap_contains(const SMap *m, const char *k);

#endif // SMAP_H
