#ifndef ARRAY_HXX
#define ARRAY_HXX

#include <cassert>

#include <stddef.h>

template <class T>
struct array {
    T *data;
    size_t len, cap;

    T &operator[](size_t i) {
        assert(i < len);
        return data[i];
    }
};

template <class T> array<T> array_create(void);
template <class T> array<T> array_push(array<T> *a, T &d);
template <class T> array<T> array_free(array<T> *a);

#endif // ARRAY_HXX
