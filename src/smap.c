#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ds/smap.h"
#include "utils.h"

SMap smap_create(hash_sig hash, vdestr_sig vdestr) {
        return (SMap) {
                .tbl = {
                        .buckets = (SMap_Node **)calloc(SMAP_DEF_BUCKET_SZ, sizeof(SMap_Node *)),
                        .len = 0,
                        .cap = SMAP_DEF_BUCKET_SZ,
                },
                .hash = hash ? hash : djb2,
                .vdestr = vdestr ? vdestr : NULL,
        };
}

SMap_Node *smap_node_alloc(char *k, void *v) {
        SMap_Node *n = (SMap_Node *)malloc(sizeof(SMap_Node));
        n->k = strdup(k);
        n->v = v;
        n->n = NULL;
        return n;
}

void smap_insert(SMap *m, char *k, void *v) {
        size_t idx = m->hash(k) % m->tbl.cap;

        SMap_Node *it = m->tbl.buckets[idx],
                *prev = NULL;

        while (it) {
                if (!strcmp(it->k, k)) {
                        if (m->vdestr) { m->vdestr(it->v); }
                        it->v = v;
                        break;
                }
                prev = it;
                it = it->n;
        }

        if (!it) {
                SMap_Node *n = smap_node_alloc(k, v);
                if (prev) {
                        prev->n = n;
                } else {
                        m->tbl.buckets[idx] = n;
                }
                ++m->tbl.len;
        }
}

void *smap_get(const SMap *m, const char *k) {
        size_t idx = m->hash(k) % m->tbl.cap;
        SMap_Node *it = m->tbl.buckets[idx];
        while (it) {
                if (!strcmp(it->k, k)) { return it->v; }
                it = it->n;
        }
        return NULL;
}

int smap_contains(const SMap *m, const char *k) {
        return smap_get(m, k) != NULL;
}

void smap_free(SMap *m) {
        assert(0 && "unimplemented");
}
