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
#ifndef CAP_STACK_H
#define CAP_STACK_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#define CAP_STACK_INITIAL_SIZE 10
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE*
#define CAP_ALLOCATOR(type, number_of_elements) calloc(number_of_elements, sizeof(type))

// Abstract type
typedef struct {
	CAP_GENERIC_TYPE_PTR* _internal_buffer;
	size_t _size;
	size_t _capacity;
} _cap_vector;

typedef struct {
	_cap_vector* _internal_container;
} cap_stack;

// Prototypes (Public APIs)
static cap_stack* cap_stack_init();
static bool cap_stack_push(cap_stack*, void*);
static void* cap_stack_pop(cap_stack*);
static void* cap_stack_top(cap_stack*);
static size_t cap_stack_size(cap_stack*);
static bool cap_stack_empty(cap_stack*);
static void cap_stack_swap(cap_stack*, cap_stack*);

// Prototypes (Internal helpers)
static bool _cap_vector_push_back(_cap_vector*, void*);
static _cap_vector* _cap_vector_init(size_t); 
static void* _cap_vector_pop_back(_cap_vector*);
static void* _cap_vector_back(_cap_vector*);

static cap_stack* cap_stack_init(){
	cap_stack* stack = (cap_stack*) CAP_ALLOCATOR(cap_stack, 1);
	CAP_CHECK_NULL(stack);
	stack->_internal_container = _cap_vector_init(CAP_STACK_INITIAL_SIZE);
	return stack;
}

static bool cap_stack_push(cap_stack* stack, void* data){
	return _cap_vector_push_back(stack->_internal_container, data);
}

static void* cap_stack_pop(cap_stack* stack){
	return _cap_vector_pop_back(stack->_internal_container);
}

static void* cap_stack_top(cap_stack* stack){
	return _cap_vector_back(stack->_internal_container);
}

static size_t cap_stack_size(cap_stack* stack){
	return stack->_internal_container->_size;
}

static bool cap_stack_empty(cap_stack* stack){
	return (stack->_internal_container->_size == 0);
}

static void cap_stack_swap(cap_stack* stack_one, cap_stack* stack_two){
	_cap_vector* one_tmp_internal_container = stack_one->_internal_container;
	stack_one->_internal_container = stack_two->_internal_container;
	stack_two->_internal_container = one_tmp_internal_container;
}

static _cap_vector* _cap_vector_init(size_t initial_size){
	assert(initial_size > 0);
	_cap_vector* vector = (_cap_vector*) CAP_ALLOCATOR(_cap_vector, 1);
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR*) CAP_ALLOCATOR(CAP_GENERIC_TYPE_PTR, initial_size);	
	vector->_capacity = initial_size;
	vector->_size = 0;
	return vector;
}

static bool _cap_vector_reserve(_cap_vector* vector, size_t new_size){
	assert(vector != NULL && new_size > 0);
	CAP_GENERIC_TYPE_PTR* tmp_ptr = (CAP_GENERIC_TYPE_PTR*) realloc(vector->_internal_buffer, 
						sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if(tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static bool _cap_vector_push_back(_cap_vector* vector, void* data){
	assert(vector != NULL && data != NULL);
	if(vector->_capacity <= vector->_size)
		if(!_cap_vector_reserve(vector, vector->_capacity * 2)) return false;
	vector->_internal_buffer[vector->_size++] = (CAP_GENERIC_TYPE_PTR) data;
	return true;
}

static void* _cap_vector_pop_back(_cap_vector* vector){
	assert(vector != NULL);
	if(vector->_size <= 0) return NULL;
	return vector->_internal_buffer[--vector->_size];
}

static void* _cap_vector_back(_cap_vector* vector){
	assert(vector != NULL);
	return vector->_size == 0 ? NULL : vector->_internal_buffer[vector->_size-1];
}

#endif // !CAP_STACK_H
