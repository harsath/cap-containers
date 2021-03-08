// cap-containers for pure C
// Copyright © 2021 Harsath
// The software is licensed under the MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef CAP_VECTOR_H
#define CAP_VECTOR_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE*
#define CAP_ALLOCATOR(type, number_of_elements) malloc(sizeof(type) * number_of_elements)

// Abstract type
typedef struct {
	CAP_GENERIC_TYPE_PTR* _internal_buffer = NULL;
	size_t _size = 0;
	size_t _capacity = 0;
} cap_vector;

// Prototypes (Public APIs)
static inline cap_vector* cap_vector_init(size_t);
static inline void* cap_vector_at(cap_vector*, size_t);
static inline void* cap_vector_front(cap_vector*);
static inline void* cap_vector_back(cap_vector*);
static inline bool cap_vector_empty(cap_vector*);
static inline size_t cap_vector_size(cap_vector*);
static inline size_t cap_vector_capacity(cap_vector*);
static inline size_t cap_vector_remaining_space(cap_vector*);
static inline bool cap_vector_push_back(cap_vector*, void*);
static inline void* cap_vector_pop_back(cap_vector*);
static inline void cap_vector_swap(cap_vector*, cap_vector*);
static inline void* cap_vector_find_if(cap_vector*, bool (*)(void*));
static inline bool cap_vector_remove_if(cap_vector*, bool (*)(void*));
static inline void cap_vector_for_each(cap_vector*, void (*)(void*));
static inline void* cap_vector_pop_front(cap_vector* vector);
static inline cap_vector* cap_vector_copy(cap_vector*);
static inline cap_vector* cap_vector_deep_copy(cap_vector*);
static inline bool cap_vector_insert(cap_vector*, size_t, void*);
static inline bool cap_vector_remove(cap_vector, size_t);
static inline bool cap_vector_resize(cap_vector*, size_t);
static inline bool cap_vector_shrink_to_fit(cap_vector*);

// Prototypes (Internal helpers)
static inline bool _cap_vector_reserve(cap_vector*);

static inline cap_vector* cap_vector_init(size_t initial_size = 20){
	assert(initial_size > 0);
	cap_vector* vector = (cap_vector*)CAP_ALLOCATOR(cap_vector, 1);
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR*)CAP_ALLOCATOR(CAP_GENERIC_TYPE, initial_size);	
	vector->_capacity = initial_size;
	return vector;
}

static inline bool _cap_vector_reserve(cap_vector* vector, size_t new_size){
	assert(vector != NULL && new_size > 0);
	CAP_GENERIC_TYPE_PTR* tmp_ptr = (CAP_GENERIC_TYPE_PTR*) realloc(vector->_internal_buffer, new_size);
	if(tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

__attribute__((always_inline))
static inline bool cap_vector_push_back(cap_vector* vector, void* data){
	assert(vector != NULL && data != NULL);
	if(vector->_capacity <= vector->_size){
		if(!_cap_vector_reserve(vector, vector->_capacity * 2)) return false;
	}
	vector->_internal_buffer[vector->_size] = (CAP_GENERIC_TYPE_PTR)data;
	vector->_size++;	
	return true;
}

__attribute__((always_inline))
static inline void* cap_vector_pop_back(cap_vector* vector){
	assert(vector != NULL);
	if(vector->_size == 0) return NULL;
	return &vector->_internal_buffer[vector->_size];
}

__attribute__((always_inline))
static inline void* cap_vector_find_if(cap_vector* vector, bool (*predicate_fn)(void*)){
	assert(vector != NULL && predicate_fn != NULL);
	size_t current_index = 0;
	if(!vector->_size) return NULL;
	do {
		if(vector->_internal_buffer[current_index] != NULL){
			if(predicate_fn(vector->_internal_buffer[current_index]))
			{ return vector->_internal_buffer[current_index]; }
		}
	} while(vector->_size >= current_index);
	return NULL;
}

__attribute__((always_inline))
static inline bool cap_vector_remove_if(cap_vector* vector, bool (*predicate_fn)(void*)){
	assert(vector != NULL && predicate_fn != NULL);
	CAP_GENERIC_TYPE_PTR* start_pointer = vector->_internal_buffer;
	size_t current_index = 0;
	if(!vector->_size) return false;
	do {
		if(start_pointer[current_index] != NULL){
			if(predicate_fn(start_pointer[current_index])){
				// memmove(&vector->_internal_buffer[current_index], 
				// 		&vector->_internal_buffer[current_index+1], vector->_size * sizeof *vector->_internal_buffer);
				for(size_t i = current_index+1; i < vector->_size; i++){
					vector->_internal_buffer[i-1] = vector->_internal_buffer[i];
				}
				vector->_size--;
			}
		}
	} while(vector->_size >= current_index);
	return false;
}

__attribute__((always_inline))
static inline void cap_vector_for_each(cap_vector* vector, void (*callback_fn)(void *)){
	assert(vector != NULL && callback_fn != NULL);
	if(!vector->_size) return;
	size_t current_index = 0;
	do {
		if(vector->_internal_buffer[current_index] != NULL)
			callback_fn(vector->_internal_buffer[current_index]);
		current_index++;
	} while(vector->_size >= current_index);
}

__attribute__((always_inline))
static inline void cap_vector_swap(cap_vector* vector_one, cap_vector* vector_two){
	assert(vector_one != NULL && vector_two != NULL);
	CAP_GENERIC_TYPE_PTR* one_temp_internal_ptr = vector_one->_internal_buffer;
	size_t one_temp_size = vector_one->_size;
	size_t one_temp_cap = vector_one->_capacity;
	vector_one->_internal_buffer = vector_two->_internal_buffer;
	vector_one->_size = vector_two->_size;
	vector_one->_capacity = vector_two->_capacity;
	vector_two->_internal_buffer = one_temp_internal_ptr;
	vector_two->_size = one_temp_size;
	vector_two->_capacity = one_temp_cap;
}

__attribute__((always_inline))
static inline bool cap_vector_empty(cap_vector* vector){
	assert(vector != NULL);
	return (vector->_size == 0);
}

__attribute__((always_inline))
static inline size_t cap_vector_size(cap_vector* vector){
	assert(vector != NULL);
	return vector->_size;
}

__attribute__((always_inline))
static inline size_t cap_vector_capacity(cap_vector* vector){
	assert(vector != NULL);
	return vector->_capacity;
}

__attribute__((always_inline))
static inline size_t cap_vector_remaining_space(cap_vector* vector){
	assert(vector != NULL);
	return (vector->_capacity - vector->_capacity);
}

__attribute__((always_inline))
static inline void* cap_vector_front(cap_vector* vector){
	assert(vector != NULL);
	return &vector->_internal_buffer[0];
}

__attribute__((always_inline))
static inline void* cap_vector_back(cap_vector* vector){
	assert(vector != NULL);
	return vector->_size == 0 ? &vector->_internal_buffer[0] : &vector->_internal_buffer[vector->_size-1];
}

__attribute__((always_inline))
static inline void* cap_vector_at(cap_vector* vector, size_t index){
	assert(vector != NULL && index > 0);
	return vector->_size < index ? NULL : &vector->_internal_buffer[vector->_size-1];
}

#endif // !CAP_VECTOR_H
