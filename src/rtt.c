#include <assert.h>
#include <stdlib.h>

#include "rtt.h"
#include "err.h"

int rttcompat(const RTT *r1, const RTT *r2) {
        return r1->kind == r2->kind;
}

RTT *rtt_alloc(RTT_Kind k) {
        RTT *rtt = NULL;
        switch (k) {
        case RTT_UNIT: {
                rtt = malloc(sizeof(RTT_Unit));
                rtt->kind = RTT_UNIT;
        } break;
        case RTT_INT: {
                rtt = malloc(sizeof(RTT_Int));
                rtt->kind = RTT_INT;
        } break;
        case RTT_STR: {
                rtt = malloc(sizeof(RTT_Str));
                rtt->kind = RTT_STR;
        } break;
        case RTT_FUNCTION: {
                rtt = malloc(sizeof(RTT_Function));
                rtt->kind = RTT_FUNCTION;
        } break;
        default: {
                err_wargs("unknown rtt type: %d", (int)k);
        } break;
        }
        rtt->kind = k;
        return rtt;
}
