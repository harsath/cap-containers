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
#ifndef CAP_FIXED_QUEUE_H
#define CAP_FIXED_QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL

// Abstract types
typedef struct {
	size_t _memory_capacity;
	size_t _current_size;
	unsigned char** _internal_buffer;
} cap_fixed_queue;

// Prototypes (Public APIs)
static inline cap_fixed_queue* cap_fixed_queue_init(size_t, size_t);
static inline void cap_fixed_queue_free(cap_fixed_queue*);
static inline size_t cap_fixed_queue_get_capacity(const cap_fixed_queue*);
static inline size_t cap_fixed_queue_get_size(const cap_fixed_queue*);
static inline size_t cap_fixed_queue_get_remaining_space(const cap_fixed_queue*);
static inline void cap_fixed_queue_push(cap_fixed_queue*, void*);
static inline void* cap_fixed_queue_pop(cap_fixed_queue*);
static inline void* cap_fixed_queue_front(cap_fixed_queue*);
static inline void* cap_fixed_queue_back(cap_fixed_queue*);
static inline void cap_fixed_queue_swap(cap_fixed_queue* fixed_queue_one, cap_fixed_queue* fixed_queue_two);
// Prototypes (Internal helpers)
static inline void _cap_fixed_queue_assign(cap_fixed_queue* fixed_queue, size_t index, void* element);

static inline cap_fixed_queue* cap_fixed_queue_init(size_t initial_size){
	cap_fixed_queue* fixed_queue = (cap_fixed_queue*)malloc(sizeof(cap_fixed_queue));
	CAP_CHECK_NULL(fixed_queue);
	fixed_queue->_internal_buffer = (unsigned char**)malloc(sizeof(unsigned char*) * initial_size);
	CAP_CHECK_NULL(fixed_queue->_internal_buffer);
	fixed_queue->_current_size = 0;
	fixed_queue->_memory_capacity = initial_size;
	return fixed_queue;
}

static inline void cap_fixed_queue_free(cap_fixed_queue* fixed_queue){
	for(size_t i = 0; i < fixed_queue->_current_size; i++)
	{ free((fixed_queue->_internal_buffer + i)); }
	if(fixed_queue != NULL) free(fixed_queue->_internal_buffer);
}

__attribute__((always_inline))
static inline size_t cap_fixed_queue_get_capacity(const cap_fixed_queue* fixed_queue){
	return fixed_queue->_memory_capacity;
}

__attribute__((always_inline))
static inline size_t cap_fixed_queue_get_size(const cap_fixed_queue* fixed_queue){
	return fixed_queue->_current_size;
}

__attribute__((always_inline))
static inline size_t cap_fixed_queue_get_remaining_space(const cap_fixed_queue* fixed_queue){
	return (fixed_queue->_memory_capacity - fixed_queue->_current_size);
}

__attribute__((always_inline))
static inline void cap_fixed_queue_push(cap_fixed_queue* fixed_queue, void* data){
	_cap_fixed_queue_assign(fixed_queue, fixed_queue->_current_size, data);
	fixed_queue->_current_size++;
}

__attribute__((always_inline))
static inline void* cap_fixed_queue_pop(cap_fixed_queue* fixed_queue){
	unsigned char* returner = *(fixed_queue->_internal_buffer + fixed_queue->_current_size);
	fixed_queue->_current_size--;
	return returner;
}

__attribute__((always_inline))
static inline void* cap_fixed_queue_front(cap_fixed_queue* fixed_queue){
	return fixed_queue->_internal_buffer[0];
}

__attribute__((always_inline))
static inline void* cap_fixed_queue_back(cap_fixed_queue* fixed_queue){
	return (void*)*(fixed_queue->_internal_buffer + (fixed_queue->_current_size-1));
}

__attribute__((always_inline))
static inline void cap_fixed_queue_swap(cap_fixed_queue* fixed_queue_one, cap_fixed_queue* fixed_queue_two){
	size_t one_capacity = fixed_queue_one->_memory_capacity;
	size_t one_size = fixed_queue_one->_current_size;
	void* one_tmp_internal_buffer = fixed_queue_one->_internal_buffer;
	fixed_queue_one->_current_size = fixed_queue_two->_current_size;		
	fixed_queue_one->_internal_buffer = fixed_queue_two->_internal_buffer;
	fixed_queue_one->_memory_capacity = fixed_queue_two->_memory_capacity;
	fixed_queue_two->_memory_capacity = one_capacity;
	fixed_queue_two->_current_size = one_size;
	fixed_queue_two->_internal_buffer = (unsigned char**)one_tmp_internal_buffer;
}

__attribute__((always_inline))
static inline void _cap_fixed_queue_assign(cap_fixed_queue* fixed_queue, size_t index, void* element){
	assert(index < fixed_queue->_memory_capacity);
	*(fixed_queue->_internal_buffer + index) = (unsigned char*)element;
}

#endif // !CAP_FIXED_QUEUE_H
