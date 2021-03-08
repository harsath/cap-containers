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
	CAP_GENERIC_TYPE_PTR _internal_buffer = NULL;
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
static inline void cap_vector_push_back(cap_vector*);
static inline void* cap_vector_pop_back(cap_vector*);
static inline void cap_vector_swap(cap_vector*, cap_vector*);
static inline void* cap_vector_find_if(cap_vector*, bool (*)(void*));
static inline bool cap_vector_remove_if(cap_vector*, bool (*)(void*));

static inline cap_vector* cap_vector_init(size_t initial_size = 20){
	cap_vector* vector = (cap_vector*)CAP_ALLOCATOR(cap_vector, 1);
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR)CAP_ALLOCATOR(CAP_GENERIC_TYPE, initial_size);	
	vector->_capacity = initial_size;
	return vector;
}

__attribute__((always_inline))
static inline bool cap_vector_empty(cap_vector* vector){
	return vector->_size == 0 ? true : false;
}

__attribute__((always_inline))
static inline size_t cap_vector_size(cap_vector* vector){
	return vector->_size;
}





#endif // !CAP_VECTOR_H
