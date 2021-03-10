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
#ifndef CAP_DYNAMIC_QUEUE_H
#define CAP_DYNAMIC_QUEUE_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE*
#define CAP_ALLOCATOR(type, number_of_elements) malloc(sizeof(type) * number_of_elements)

// Abstract types
typedef struct _cap_list_node {
	CAP_GENERIC_TYPE_PTR data;
	struct _cap_list_node* next;
	struct _cap_list_node* previous;
} _cap_list_node;

// Head and tail are sentinel nodes. It is design decision.
typedef struct {
	_cap_list_node* _head_node;
	_cap_list_node* _tail_node;
} _cap_list;

typedef struct {
	size_t _current_size;
	_cap_list* _internal_list;
} cap_dynamic_queue;

// Prototypes (Public APIs)
// Init:
static inline cap_dynamic_queue* cap_dynamic_queue_init();

// Lookup & Update:
static inline size_t cap_dynamic_queue_size(const cap_dynamic_queue*);
static inline bool cap_dynamic_queue_push(cap_dynamic_queue*, void*);
static inline void* cap_dynamic_queue_pop(cap_dynamic_queue*);
static inline void* cap_dynamic_queue_front(cap_dynamic_queue*);
static inline void* cap_dynamic_queue_back(cap_dynamic_queue*);
static inline void cap_dynamic_queue_swap(cap_dynamic_queue* dynamic_queue_one, cap_dynamic_queue* dynamic_queue_two);

// Memory:
static inline void cap_dynamic_queue_free(cap_dynamic_queue*);
static inline void cap_dynamic_queue_deep_free(cap_dynamic_queue*);

// Prototypes (Internal helpers)
static inline _cap_list* _cap_list_init();
static inline bool _cap_list_push_front(_cap_list*, void*);
static inline void* _cap_list_pop_back(_cap_list*);
static inline void* _cap_list_front(const _cap_list*);
static inline void* _cap_list_back(const _cap_list*);
static inline void _cap_list_deep_free(_cap_list*);
static inline void _cap_list_free(_cap_list*);

static inline cap_dynamic_queue* cap_dynamic_queue_init(){
	cap_dynamic_queue* dynamic_queue = (cap_dynamic_queue*) CAP_ALLOCATOR(cap_dynamic_queue, 1);
	CAP_CHECK_NULL(dynamic_queue);
	dynamic_queue->_internal_list = _cap_list_init();
	CAP_CHECK_NULL(dynamic_queue->_internal_list);
	dynamic_queue->_current_size = 0;
	return dynamic_queue;
}

static inline void cap_dynamic_queue_free(cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	_cap_list_free(dynamic_queue->_internal_list);
	free(dynamic_queue);
}

static inline void cap_dynamic_queue_deep_free(cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	_cap_list_deep_free(dynamic_queue->_internal_list);
	free(dynamic_queue);
}

__attribute__((always_inline))
static inline size_t cap_dynamic_queue_size(const cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	return dynamic_queue->_current_size;
}

__attribute__((always_inline))
static inline bool cap_dynamic_queue_push(cap_dynamic_queue* dynamic_queue, void* data){
	assert((dynamic_queue != NULL) && (data != NULL));
	bool return_val = _cap_list_push_front(dynamic_queue->_internal_list, data);
	dynamic_queue->_current_size++;
	return return_val;
}

__attribute__((always_inline))
static inline void* cap_dynamic_queue_pop(cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	if(dynamic_queue->_current_size <= 0) return NULL;
	void* returner = _cap_list_pop_back(dynamic_queue->_internal_list);
	dynamic_queue->_current_size--;
	return returner;
}

__attribute__((always_inline))
static inline void* cap_dynamic_queue_front(cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	if(dynamic_queue->_current_size <= 0) return NULL;
	return _cap_list_back(dynamic_queue->_internal_list);
}

__attribute__((always_inline))
static inline void* cap_dynamic_queue_back(cap_dynamic_queue* dynamic_queue){
	assert(dynamic_queue != NULL);
	return _cap_list_front(dynamic_queue->_internal_list);
}

__attribute__((always_inline))
static inline void cap_dynamic_queue_swap(cap_dynamic_queue* dynamic_queue_one, cap_dynamic_queue* dynamic_queue_two){
	assert((dynamic_queue_one != NULL) && (dynamic_queue_two != NULL));
	size_t one_size = dynamic_queue_one->_current_size;
	_cap_list* one_tmp_internal_list = dynamic_queue_one->_internal_list;
	dynamic_queue_one->_current_size = dynamic_queue_two->_current_size;		
	dynamic_queue_one->_internal_list = dynamic_queue_two->_internal_list;
	dynamic_queue_two->_current_size = one_size;
	dynamic_queue_two->_internal_list = one_tmp_internal_list;
}

static inline _cap_list* _cap_list_init(){
	_cap_list_node* head_and_tail_nodes = (_cap_list_node*) CAP_ALLOCATOR(_cap_list_node, 2);
	_cap_list* d_list = (_cap_list*) CAP_ALLOCATOR(_cap_list, 1);
	d_list->_head_node = head_and_tail_nodes;
	d_list->_tail_node = (head_and_tail_nodes + 1);
	d_list->_head_node->data = NULL;
	d_list->_head_node->next = d_list->_tail_node;
	d_list->_head_node->previous = NULL;
	d_list->_tail_node->data = NULL;
	d_list->_tail_node->next = NULL;
	d_list->_tail_node->previous = d_list->_head_node;
	return d_list;
}

static inline bool _cap_list_push_front(_cap_list* d_list, void* data){
	assert((d_list != NULL) && (data != NULL));
	_cap_list_node* new_node = (_cap_list_node*) CAP_ALLOCATOR(_cap_list_node, 1);
	if(new_node == NULL) return false;
	new_node->data = (CAP_GENERIC_TYPE_PTR) data;
	new_node->previous = d_list->_head_node;
	_cap_list_node* tmp_head_next_holder = d_list->_head_node->next;
	d_list->_head_node->next = new_node;
	new_node->next = tmp_head_next_holder;
	tmp_head_next_holder->previous = new_node;
	return true;
}

__attribute__((always_inline))
static inline void* _cap_list_pop_back(_cap_list* d_list){
	assert(d_list != NULL);
	_cap_list_node* pop_node = d_list->_tail_node->previous;
	d_list->_tail_node->previous = pop_node->previous;
	pop_node->previous->next = d_list->_tail_node;
	return pop_node->data;
}

__attribute__((always_inline))
static inline void* _cap_list_front(const _cap_list* d_list){
	assert(d_list != NULL);
	return d_list->_head_node->next->data;
}

__attribute__((always_inline))
static inline void* _cap_list_back(const _cap_list* d_list){
	assert(d_list != NULL);
	return d_list->_tail_node->previous->data;
}

__attribute__((always_inline))
static inline void _cap_list_free(_cap_list* d_list){
	assert(d_list != NULL);
	_cap_list_node* current_node = d_list->_head_node->next;
	_cap_list_node* next_node;
	do {
		next_node = current_node->next;
		current_node->data = NULL;
		if(current_node != NULL) free(current_node);
		current_node = next_node;
	} while((current_node != NULL) && (current_node != d_list->_tail_node));
	free(d_list->_head_node);
}

__attribute__((always_inline))
static inline void _cap_list_deep_free(_cap_list* d_list){
	assert(d_list != NULL);
	_cap_list_node* current_node = d_list->_head_node->next;
	_cap_list_node* next_node;
	do {
		next_node = current_node->next;
		if(current_node->data != NULL) free(current_node->data);
		if(current_node != NULL) free(current_node);
		current_node = next_node;
	} while((current_node != NULL) && (current_node != d_list->_tail_node));
	free(d_list->_head_node);
}

#endif // !CAP_DYNAMIC_QUEUE_H
