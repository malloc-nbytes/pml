#ifndef RTT_H
#define RTT_H

typedef enum {
        RTT_UNIT,
        RTT_INT,
        RTT_STR,
} Runtime_Type_Kind;

typedef struct { Runtime_Type_Kind kind; } RTT;

typedef struct {
        RTT base;
} RTT_Int;

typedef struct {
        RTT base;
} RTT_Str;

typedef struct {
        RTT base;
} RTT_Unit;

#endif // RTT_H
