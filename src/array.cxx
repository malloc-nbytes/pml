#include "ds/array.hxx"

template <class T> array<T> array_create(void) {
    return (array<T>) {
        .data = nullptr,
        .len = 0,
        .cap = 0,
    };
}

template <class T> array<T> array_push(array<T> *a, T &d) {
    if (a->len >= a->cap) {
        a->cap = a->cap == 0 ? 2 : a->cap*2;
        T *old = new T[a->cap];
        for (size_t i = 0; i < a->len; ++i) {
            old[i] = a->data[i];
        }
        delete[] a->data;
        a->data = old;
    }
    a->data[a->len++] = d;
}

template <class T> array<T> array_free(array<T> *a) {
    delete[] a->data;
    a->len = a->cap = 0;
}
