#include <array.h>

void createArray(Array* arr, size_t initialCapacity) {
	arr->list = malloc(initialCapacity * sizeof(void*));
	arr->size = 0;
	arr->capacity = initialCapacity;
}

void push(Array* arr, void* element) {
	if (arr->size == arr->capacity) {
		arr->capacity *= 2;
		arr->list = realloc(arr->list, arr->capacity * sizeof(void*));
	}
	arr->list[arr->size++] = element;
}

void destroyArray(Array* arr) {
	//TODO: clean elements
	free(arr->list);
	arr->list = NULL;
	arr->size = arr->capacity = 0;
}

void* getElement(const Array* arr, size_t index) {
	if (index >= arr->size) {
		return NULL;
	}
	return arr->list[index];
}
