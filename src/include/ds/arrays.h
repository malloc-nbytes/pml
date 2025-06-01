#ifndef ARRAYS_H
#define ARRAYS_H

#include "dyn_array.h"
#include "smap.h"

DYN_ARRAY_TYPE(char, Char_Array);
DYN_ARRAY_TYPE(char *, Str_Array);
DYN_ARRAY_TYPE(SMap, SMap_Array);

#endif // ARRAYS_H
