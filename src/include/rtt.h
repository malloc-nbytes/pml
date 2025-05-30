#ifndef RTT_H
#define RTT_H

typedef enum {
        RTT_UNIT,
        RTT_INT,
        RTT_STR,
} RTT_Kind;

typedef struct { RTT_Kind kind; } RTT;

typedef struct {
        RTT base;
} RTT_Int;

typedef struct {
        RTT base;
} RTT_Str;

typedef struct {
        RTT base;
} RTT_Unit;

int rttcompat(const RTT *r1, const RTT *r2);
RTT *rtt_alloc(RTT_Kind k);

#endif // RTT_H
