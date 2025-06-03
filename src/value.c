#include <stdlib.h>
#include <string.h>

#include "value.h"
#include "err.h"

Value *value_alloc(Value_Type ty) {
        Value *v = NULL;
        switch (ty) {
        case VALUE_TYPE_UNIT: {
                v = malloc(sizeof(Value_Unit));
        } break;
        case VALUE_TYPE_INT: {
                v = malloc(sizeof(Value_Int));
        } break;
        case VALUE_TYPE_STR: {
                v = malloc(sizeof(Value_Str));
        } break;
        default: {
                err_wargs("unknown value type %d", (int)ty);
        } break;
        }
        v->ty = ty;
        return v;
}

Value_Int *value_int_alloc(int data) {
        Value_Int *v = (Value_Int *)value_alloc(VALUE_TYPE_INT);
        v->actual.data = data;
        return v;
}

Value_Unit *value_unit_alloc(void) {
        Value_Unit *v = (Value_Unit *)value_alloc(VALUE_TYPE_UNIT);
        return v;
}

Value_Str *value_str_alloc(char *data) {
        Value_Str *v = (Value_Str *)value_alloc(VALUE_TYPE_STR);
        v->actual.data = strdup(data);
        v->actual.len = strlen(data);
        v->actual.cap = v->actual.len;
        return v;
}
