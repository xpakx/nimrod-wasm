#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

// array can store only pointers
typedef struct {
    void** list;
    size_t size;
    size_t capacity;
    size_t element_size;
} Array;

void createArray(Array* arr, size_t initialCapacity);
void push(Array* arr, void* element);
void* getElement(const Array* arr, size_t index);
void destroyArray(Array* arr);

#endif // ARRAY_H
