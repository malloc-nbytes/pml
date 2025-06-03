#ifndef VALUE_H
#define VALUE_H

#include <stddef.h>

typedef enum {
        VALUE_TYPE_UNIT,
        VALUE_TYPE_INT,
        VALUE_TYPE_STR,
} Value_Type;

typedef struct {
        Value_Type ty;
} Value;

typedef struct {
        Value base;
} Value_Unit;

typedef struct {
        Value base;
        struct {
                int data;
        } actual;
} Value_Int;

typedef struct {
        Value base;
        struct {
                char *data;
                size_t len;
                size_t cap;
        } actual;
} Value_Str;

Value_Int *value_int_alloc(int data);
Value_Unit *value_unit_alloc(void);
Value_Str *value_str_alloc(char *data);

#endif // VALUE_H
