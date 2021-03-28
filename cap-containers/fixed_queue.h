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
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_CHECK_NULL(value)                                                  \
	if (value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	malloc(sizeof(type) * number_of_elements)

// Abstract types
typedef struct _cap_list_node {
	CAP_GENERIC_TYPE_PTR data;
	struct _cap_list_node *next;
	struct _cap_list_node *previous;
} _cap_list_node;

// Head and tail are sentinel nodes. It is design decision.
typedef struct {
	_cap_list_node *_head_node;
	_cap_list_node *_tail_node;
} _cap_list;

typedef struct {
	size_t _capacity;
	size_t _current_size;
	_cap_list *_internal_list;
} cap_fixed_queue;
#endif

// Prototypes (Public APIs)
// Init:
/**
 * Initilize a cap_fixed_queue object
 *
 * @param init_size Size of the fixed queue
 * @return Dynamically allocated cap_fixed_queue container object
 */
static cap_fixed_queue *cap_fixed_queue_init(size_t init_size);

// Lookup & Update:
/**
 * Query the capacity of the cap_fixed_queue container
 *
 * @param cap_fixed_queue container
 * @return capacity of the container
 */
static size_t cap_fixed_queue_capacity(const cap_fixed_queue *queue);
/**
 * Query the size of the cap_fixed_queue container
 *
 * @param cap_fixed_queue container
 * @return Current number of elements in the container
 */
static size_t cap_fixed_queue_size(const cap_fixed_queue *queue);
/**
 * Query the remaining size left in the queue
 *
 * @param queue cap_fixed_queue container
 * @return Space left in the container
 */
static size_t cap_fixed_queue_remaining_space(const cap_fixed_queue *queue);
/**
 * Push an element onto the queue(User is totally responsible for lifetime of
 * that pointer/element)
 *
 * @param queue cap_fixed_queue container
 * @param item Element to be push onto the queue
 * @return True if success, or else returns False(indicated queue is full or
 * memory allocation error)
 */
static bool cap_fixed_queue_push(cap_fixed_queue *queue, void *item);
/**
 * Pop an element from the end of the queue
 *
 * @param queue cap_fixed_queue container
 * @return Returns a pointer to the element, or returns NULL if queue is empty
 */
static void *cap_fixed_queue_pop(cap_fixed_queue *queue);
/**
 * Gives access to the front of the queue and does not modify the container
 *
 * @param queue cap_fixed_queue container
 * @return Returns access to the element without poping the element or else
 * returns NULL if queue is empty
 */
static void *cap_fixed_queue_front(cap_fixed_queue *queue);
/**
 * Gives access to the back of the queue and doesn't modify the container
 *
 * @param queue cap_fixed_queue container
 * @return Returns access to the back of the container without modifying the
 * container or returns NULL if queue is empty
 */
static void *cap_fixed_queue_back(cap_fixed_queue *queue);
/**
 * Swap two cap_fixed_queue object's internal elements
 *
 * @param fixed_queue_one First queue container
 * @param fixed_queue_two Second queue container
 */
static void cap_fixed_queue_swap(cap_fixed_queue *fixed_queue_one,
				 cap_fixed_queue *fixed_queue_two);

// Memory:
/**
 * Free the cap_fixed_queue container object without touching the elements which
 * it contains.
 *
 * @param queue cap_fixed_queue container
 */
static void cap_fixed_queue_free(cap_fixed_queue *queue);
/**
 * Frees the cap_fixed_queue container object along with elements it
 * container(assuming the elements are dynamically allocated)
 *
 * @param queue cap_fixed_queue container
 */
static void cap_fixed_queue_deep_free(cap_fixed_queue *queue);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Prototypes (Internal helpers)
static _cap_list *_cap_list_init();
static bool _cap_list_push_front(_cap_list *, void *);
static void *_cap_list_pop_back(_cap_list *);
static void *_cap_list_front(const _cap_list *);
static void *_cap_list_back(const _cap_list *);
static void _cap_list_deep_free(_cap_list *);
static void _cap_list_free(_cap_list *);
#endif

static cap_fixed_queue *cap_fixed_queue_init(size_t initial_size) {
	assert(initial_size > 0);
	cap_fixed_queue *fixed_queue =
	    (cap_fixed_queue *)CAP_ALLOCATOR(cap_fixed_queue, 1);
	CAP_CHECK_NULL(fixed_queue);
	fixed_queue->_internal_list = _cap_list_init();
	CAP_CHECK_NULL(fixed_queue->_internal_list);
	fixed_queue->_current_size = 0;
	fixed_queue->_capacity = initial_size;
	return fixed_queue;
}

static void cap_fixed_queue_free(cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	_cap_list_free(fixed_queue->_internal_list);
	free(fixed_queue);
}

static void cap_fixed_queue_deep_free(cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	_cap_list_deep_free(fixed_queue->_internal_list);
	free(fixed_queue);
}

static size_t cap_fixed_queue_capacity(const cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	return fixed_queue->_capacity;
}

static size_t cap_fixed_queue_size(const cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	return fixed_queue->_current_size;
}

static size_t
cap_fixed_queue_remaining_space(const cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	return (fixed_queue->_capacity - fixed_queue->_current_size);
}

static bool cap_fixed_queue_push(cap_fixed_queue *fixed_queue, void *data) {
	assert((fixed_queue != NULL) && (data != NULL));
	if (fixed_queue->_current_size >= fixed_queue->_capacity) return false;
	_cap_list_push_front(fixed_queue->_internal_list, data);
	fixed_queue->_current_size++;
	return true;
}

static void *cap_fixed_queue_pop(cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	if (fixed_queue->_current_size <= 0) return NULL;
	void *returner = _cap_list_pop_back(fixed_queue->_internal_list);
	fixed_queue->_current_size--;
	return returner;
}

static void *cap_fixed_queue_front(cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	if (fixed_queue->_current_size <= 0) return NULL;
	return _cap_list_back(fixed_queue->_internal_list);
}

static void *cap_fixed_queue_back(cap_fixed_queue *fixed_queue) {
	assert(fixed_queue != NULL);
	return _cap_list_front(fixed_queue->_internal_list);
}

static void cap_fixed_queue_swap(cap_fixed_queue *fixed_queue_one,
				 cap_fixed_queue *fixed_queue_two) {
	assert((fixed_queue_one != NULL) && (fixed_queue_two != NULL));
	size_t one_capacity = fixed_queue_one->_capacity;
	size_t one_size = fixed_queue_one->_current_size;
	_cap_list *one_tmp_internal_list = fixed_queue_one->_internal_list;
	fixed_queue_one->_current_size = fixed_queue_two->_current_size;
	fixed_queue_one->_internal_list = fixed_queue_two->_internal_list;
	fixed_queue_one->_capacity = fixed_queue_two->_capacity;
	fixed_queue_two->_capacity = one_capacity;
	fixed_queue_two->_current_size = one_size;
	fixed_queue_two->_internal_list = one_tmp_internal_list;
}

static _cap_list *_cap_list_init() {
	_cap_list_node *head_and_tail_nodes =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 2);
	_cap_list *d_list = (_cap_list *)CAP_ALLOCATOR(_cap_list, 1);
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

static bool _cap_list_push_front(_cap_list *d_list, void *data) {
	assert((d_list != NULL) && (data != NULL));
	_cap_list_node *new_node =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 1);
	if (new_node == NULL) return false;
	new_node->data = (CAP_GENERIC_TYPE_PTR)data;
	new_node->previous = d_list->_head_node;
	_cap_list_node *tmp_head_next_holder = d_list->_head_node->next;
	d_list->_head_node->next = new_node;
	new_node->next = tmp_head_next_holder;
	tmp_head_next_holder->previous = new_node;
	return true;
}

static void *_cap_list_pop_back(_cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *pop_node = d_list->_tail_node->previous;
	d_list->_tail_node->previous = pop_node->previous;
	pop_node->previous->next = d_list->_tail_node;
	return pop_node->data;
}

static void *_cap_list_front(const _cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_head_node->next->data;
}

static void *_cap_list_back(const _cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_tail_node->previous->data;
}

static void _cap_list_free(_cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *current_node = d_list->_head_node->next;
	_cap_list_node *next_node;
	do {
		next_node = current_node->next;
		current_node->data = NULL;
		if (current_node != NULL) free(current_node);
		current_node = next_node;
	} while ((current_node != NULL) &&
		 (current_node != d_list->_tail_node));
	free(d_list->_head_node);
}

static void _cap_list_deep_free(_cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *current_node = d_list->_head_node->next;
	_cap_list_node *next_node;
	do {
		next_node = current_node->next;
		if (current_node->data != NULL) free(current_node->data);
		if (current_node != NULL) free(current_node);
		current_node = next_node;
	} while ((current_node != NULL) &&
		 (current_node != d_list->_tail_node));
	free(d_list->_head_node);
}

#endif // !CAP_FIXED_QUEUE_H
