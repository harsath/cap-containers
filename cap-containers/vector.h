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
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE*
#define CAP_ALLOCATOR(type, number_of_elements) calloc(number_of_elements, sizeof(type))

// Abstract type
typedef struct {
	CAP_GENERIC_TYPE_PTR* _internal_buffer;
	size_t _size;
	size_t _capacity;
} cap_vector;

typedef struct {
	cap_vector* _internal_pointer;	
	size_t _current_index;
	void* data;
} cap_vector_iterator;

// Prototypes (Public APIs)
// Init:
static inline cap_vector* cap_vector_init(size_t);
static inline cap_vector_iterator* cap_vector_iterator_init(cap_vector*);

// Lookup & Update:
static inline void* cap_vector_at(cap_vector*, size_t);
static inline void cap_vector_clear(cap_vector*);
static inline void* cap_vector_pop_front(cap_vector* vector);
static inline cap_vector* cap_vector_copy(cap_vector*);
static inline cap_vector* cap_vector_deep_copy(cap_vector*);
static inline void* cap_vector_front(cap_vector*);
static inline void* cap_vector_back(cap_vector*);
static inline bool cap_vector_push_back(cap_vector*, void*);
static inline void* cap_vector_pop_back(cap_vector*);
static inline void cap_vector_swap(cap_vector*, cap_vector*);
static inline void* cap_vector_find_if(cap_vector*, bool (*)(void*));
static inline bool cap_vector_remove_if(cap_vector*, bool (*)(void*));
static inline void cap_vector_for_each(cap_vector*, void (*)(void*));

// Memory:
static inline bool cap_vector_resize(cap_vector*, size_t);
static inline bool cap_vector_shrink_to_fit(cap_vector*);
static inline void cap_vector_free(cap_vector*);
static inline void cap_vector_deep_free(cap_vector*);

// Checks:
static inline size_t cap_vector_size(cap_vector*);
static inline size_t cap_vector_capacity(cap_vector*);
static inline bool cap_vector_empty(cap_vector*);
static inline size_t cap_vector_remaining_space(cap_vector*);

// Iterator:
static inline void cap_vector_iterator_insert(cap_vector_iterator*, void*);
static inline void cap_vector_iterator_remove(cap_vector_iterator*);
static inline void cap_vector_iterator_increment(cap_vector_iterator*);
static inline void cap_vector_iterator_decrement(cap_vector_iterator*);
static inline void* cap_vector_iterator_next(cap_vector_iterator*);
static inline void* cap_vector_iterator_previous(cap_vector_iterator*);
static inline bool cap_vector_iterator_equals(cap_vector_iterator*, cap_vector_iterator*);
static inline bool cap_vector_iterator_equals_predicate(cap_vector_iterator*, bool(*)(void*));
static inline size_t cap_vector_iterator_index(cap_vector_iterator*);
static inline cap_vector_iterator* cap_vector_begin(cap_vector*);
static inline cap_vector_iterator* cap_vector_end(cap_vector*);
static inline void cap_vector_iterator_free(cap_vector_iterator*);

// Prototypes (Internal helpers)
static inline bool _cap_vector_reserve(cap_vector*, size_t);

// Implementations:
static inline cap_vector* cap_vector_init(size_t initial_size){
	assert(initial_size > 0);
	cap_vector* vector = (cap_vector*) CAP_ALLOCATOR(cap_vector, 1);
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR*) CAP_ALLOCATOR(CAP_GENERIC_TYPE_PTR, initial_size);	
	vector->_capacity = initial_size;
	vector->_size = 0;
	return vector;
}

static inline cap_vector_iterator* cap_vector_iterator_init(cap_vector* vector){
	assert(vector != NULL);	
	cap_vector_iterator* iterator = (cap_vector_iterator*) CAP_ALLOCATOR(cap_vector_iterator, 1);
	iterator->_current_index = 0;
	iterator->_internal_pointer = vector;
	iterator->data = vector->_internal_buffer[0];
	return iterator;
}

static inline bool _cap_vector_reserve(cap_vector* vector, size_t new_size){
	assert(vector != NULL && new_size > 0);
	CAP_GENERIC_TYPE_PTR* tmp_ptr = (CAP_GENERIC_TYPE_PTR*) realloc(vector->_internal_buffer, 
						sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if(tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static inline void cap_vector_deep_free(cap_vector* vector){
	assert(vector != NULL);
	if(vector->_internal_buffer != NULL){
		for(size_t i = 0; i < vector->_size; i++)
			if(vector->_internal_buffer[i] != NULL) free(vector->_internal_buffer[i]);
		free(vector->_internal_buffer);
	}
	free(vector);
}

static inline void cap_vector_free(cap_vector* vector){
	assert(vector != NULL);
	free(vector);
}

static inline bool cap_vector_resize(cap_vector* vector, size_t new_size){
	assert(vector != NULL && new_size >= 0);
	CAP_GENERIC_TYPE_PTR* tmp_ptr = (CAP_GENERIC_TYPE_PTR*) realloc(vector->_internal_buffer, 
						sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if(tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static inline cap_vector* cap_vector_copy(cap_vector* vector){
	assert(vector != NULL);
	cap_vector* copy_vector = (cap_vector*) CAP_ALLOCATOR(cap_vector, 1);
	if(copy_vector == NULL) return NULL;
	copy_vector->_capacity = vector->_capacity;
	copy_vector->_internal_buffer = vector->_internal_buffer;
	copy_vector->_size = vector->_size;
	return copy_vector;
}

static inline cap_vector* cap_vector_deep_copy(cap_vector* vector){
	assert(vector != NULL);
	cap_vector* deep_copy_vector = (cap_vector*) CAP_ALLOCATOR(cap_vector, 1);
	if(deep_copy_vector == NULL) return NULL;
	deep_copy_vector->_capacity = vector->_capacity;
	deep_copy_vector->_size = vector->_size;
	deep_copy_vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR*) CAP_ALLOCATOR(CAP_GENERIC_TYPE_PTR, vector->_capacity);
	if(deep_copy_vector->_internal_buffer == NULL){
		free(deep_copy_vector);
		return NULL;
	}
	memcpy(deep_copy_vector->_internal_buffer, vector->_internal_buffer, sizeof(CAP_GENERIC_TYPE_PTR) * vector->_capacity);
	return deep_copy_vector;
}

static inline void* cap_vector_pop_front(cap_vector* vector){
	assert(vector != NULL);
	if(vector->_size <= 0) return NULL;
	CAP_GENERIC_TYPE_PTR returner = vector->_internal_buffer[0];
	memmove(&vector->_internal_buffer[0], &vector->_internal_buffer[1], sizeof(CAP_GENERIC_TYPE_PTR)*(vector->_size-1));
	vector->_size--;
	return returner;
}

__attribute__((always_inline))
static inline bool cap_vector_push_back(cap_vector* vector, void* data){
	assert(vector != NULL && data != NULL);
	if(vector->_capacity <= vector->_size)
		if(!_cap_vector_reserve(vector, vector->_capacity * 2)) return false;
	vector->_internal_buffer[vector->_size++] = (CAP_GENERIC_TYPE_PTR) data;
	return true;
}

__attribute__((always_inline))
static inline void* cap_vector_pop_back(cap_vector* vector){
	assert(vector != NULL);
	if(vector->_size <= 0) return NULL;
	return vector->_internal_buffer[--vector->_size];
}

__attribute__((always_inline))
static inline void* cap_vector_find_if(cap_vector* vector, bool (*predicate_fn)(void*)){
	assert(vector != NULL && predicate_fn != NULL);
	size_t current_index = 0;
	if(!vector->_size) return NULL;
	do {
		if(vector->_internal_buffer[current_index] != NULL)
			if(predicate_fn(vector->_internal_buffer[current_index]))
				return vector->_internal_buffer[current_index];
		current_index++;
	} while(vector->_size >= current_index);
	return NULL;
}

__attribute__((always_inline))
static inline bool cap_vector_remove_if(cap_vector* vector, bool (*predicate_fn)(void*)){
	assert(vector != NULL && predicate_fn != NULL);
	size_t num_removed = 0;
	if(!vector->_size) return false;
	size_t write_index = 0;
	for(size_t read_index = 0; read_index < vector->_size; read_index++){
		if(predicate_fn(vector->_internal_buffer[read_index])){
			num_removed++;
			continue;
		}
		if(read_index != write_index)
			vector->_internal_buffer[write_index] = vector->_internal_buffer[read_index];
		write_index++;
	}
	vector->_size = (vector->_size - num_removed);
	return (num_removed != 0);
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

static inline bool cap_vector_shrink_to_fit(cap_vector* vector){
	assert(vector != NULL);
	CAP_GENERIC_TYPE_PTR* tmp_ptr = (CAP_GENERIC_TYPE_PTR*) realloc(vector->_internal_buffer, 
						sizeof(CAP_GENERIC_TYPE_PTR) * vector->_size);
	if(tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = vector->_size;
	return true;
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
	return (vector->_capacity - vector->_size);
}

__attribute__((always_inline))
static inline void* cap_vector_front(cap_vector* vector){
	assert(vector != NULL);
	return vector->_internal_buffer[0];
}

__attribute__((always_inline))
static inline void* cap_vector_back(cap_vector* vector){
	assert(vector != NULL);
	return vector->_size == 0 ? NULL : vector->_internal_buffer[vector->_size-1];
}

__attribute__((always_inline))
static inline void* cap_vector_at(cap_vector* vector, size_t index){
	assert(vector != NULL && index > 0);
	return vector->_size < index ? NULL : &vector->_internal_buffer[index];
}

__attribute__((always_inline))
static inline void cap_vector_iterator_insert(cap_vector_iterator* iterator, void* data){
	assert(iterator != NULL && data != NULL);
	iterator->_internal_pointer->_internal_buffer[iterator->_current_index] = (CAP_GENERIC_TYPE_PTR)data;
	iterator->data = data;
}

static inline void cap_vector_iterator_remove(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	memmove(&iterator->_internal_pointer->_internal_buffer[iterator->_current_index],
		&iterator->_internal_pointer->_internal_buffer[iterator->_current_index+1],
		sizeof(CAP_GENERIC_TYPE_PTR) * (iterator->_internal_pointer->_capacity - iterator->_current_index));
	iterator->data = iterator->_internal_pointer->_internal_buffer[iterator->_current_index];
}

__attribute__((always_inline))
static inline void cap_vector_iterator_increment(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	if(iterator->_internal_pointer->_size < iterator->_current_index){
		iterator->data = NULL;
		return;
	}
	if(iterator->data != NULL) iterator->_current_index++;
	iterator->data = iterator->_internal_pointer->_internal_buffer[iterator->_current_index];
}

__attribute__((always_inline))
static inline void* cap_vector_iterator_next(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	if(iterator->_internal_pointer->_size < (iterator->_current_index+1)) return NULL;
	return iterator->_internal_pointer->_internal_buffer[(iterator->_current_index+1)];
}

__attribute__((always_inline))
static inline void* cap_vector_iterator_previous(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	if((iterator->_current_index-1) < 0) return NULL;
	return iterator->_internal_pointer->_internal_buffer[(iterator->_current_index-1)];
}

__attribute__((always_inline))
static inline bool cap_vector_iterator_equals(cap_vector_iterator* iter_one, cap_vector_iterator* iter_two){
	assert(iter_one != NULL && iter_two != NULL);
	if(iter_one->data == iter_two->data) return true;
	return false;
}

__attribute__((always_inline))
static inline cap_vector_iterator* cap_vector_begin(cap_vector* vector){
	assert(vector != NULL);
	// begin() is alias to init
	return cap_vector_iterator_init(vector);
}

__attribute__((always_inline))
static inline cap_vector_iterator* cap_vector_end(cap_vector* vector){
	assert(vector != NULL);
	cap_vector_iterator* iterator = cap_vector_iterator_init(vector);
	iterator->_current_index = (vector->_size-1);
	iterator->data = vector->_internal_buffer[iterator->_current_index];
	return iterator;
}

__attribute__((always_inline))
static inline bool cap_vector_iterator_equals_predicate(cap_vector_iterator* iter, bool (*predicate_fn)(void*)){
	assert(iter != NULL & predicate_fn != NULL);
	return predicate_fn(iter->data);
}

__attribute__((always_inline))
static inline size_t cap_vector_iterator_index(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	return iterator->_current_index;
}

__attribute__((always_inline))
static inline void cap_vector_iterator_decrement(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	if(iterator->_current_index <= 0){
		iterator->data = NULL;
		return;
	}
	if(iterator->data != NULL) iterator->_current_index--;
	iterator->data = iterator->_internal_pointer->_internal_buffer[iterator->_current_index];
}

__attribute__((always_inline))
static inline void cap_vector_iterator_free(cap_vector_iterator* iterator){
	assert(iterator != NULL);
	free(iterator);
}

#endif // !CAP_VECTOR_H
