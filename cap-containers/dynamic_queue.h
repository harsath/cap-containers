// cap-containers for pure C
// Copyright © 2021 Harsath <harsath@tuta.io>
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
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	malloc(sizeof(type) * number_of_elements)
#endif // !DOXYGEN_SHOULD_SKIP_THIS

#ifndef DOXYGEN_SHOULD_SKIP_THIS
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
#endif // !DOXYGEN_SHOULD_SKIP_THIS

typedef struct {
	size_t _current_size;
	_cap_list *_internal_list;
} cap_dynamic_queue;

/**
 * Initilize a cap_dynamic_queue object
 *
 * @return A dynamic queue container.
 */
static cap_dynamic_queue *cap_dynamic_queue_init();

/**
 * Query size of the dynamic queue
 *
 * @param d_queue Takes a cap_dynamic_queue container object
 * @return Size of the queue
 */
static size_t cap_dynamic_queue_size(const cap_dynamic_queue *d_queue);
/**
 * Push an element onto the queue
 *
 * @param d_queue Takes a cap_dynamic_queue container object
 * @param item Element to push
 * @return If success, returns <pre>true</pre>
 */
static bool cap_dynamic_queue_push(cap_dynamic_queue *d_queue, void *item);
/**
 * Pop the last element of the queue
 *
 * @param d_queue Takes cap_dynamic_queue container object
 * @return Data of last item on the queue
 */
static void *cap_dynamic_queue_pop(cap_dynamic_queue *d_queue);
/**
 * Access the front of the queue but does not modify the queue.
 *
 * @param d_queue Takes cap_dynamic_queue container object
 * @return Data of the front of the queue. Returns NULL if queue is empty
 */
static void *cap_dynamic_queue_front(cap_dynamic_queue *d_queue);
/**
 * Access the back of the queue but does not modify the queue.
 *
 * @param d_queue Takes cap_dynamic_queue container object
 * @return Data of the back of the queue. Returns NULL if queue is empty
 */
static void *cap_dynamic_queue_back(cap_dynamic_queue *d_queue);
/**
 * Swap two cap_dynamic_queue container object's members
 *
 * @param dynamic_queue_one cap_dynamic_queue container one.
 * @param dynamic_queue_two cap_dynamic_queue container two.
 */
static void cap_dynamic_queue_swap(cap_dynamic_queue *dynamic_queue_one,
				   cap_dynamic_queue *dynamic_queue_two);

// Memory:
/**
 * Frees the cap_dynamic_queue object and does not touch the elements/items
 * inside the container.
 *
 * @param d_queue Takes cap_dynamic_queue container object.
 */
static void cap_dynamic_queue_free(cap_dynamic_queue *d_queue);
/**
 * Frees the cap_dynamic_queue object and also the elements inside the
 * container, assuming the items are dynamically allocated
 *
 * @param d_queue Takes cap_dynamic_queue container object.
 */
static void cap_dynamic_queue_deep_free(cap_dynamic_queue *d_queue);

// Prototypes (Internal helpers)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
static _cap_list *_cap_list_init();
static bool _cap_list_push_front(_cap_list *, void *);
static void *_cap_list_pop_back(_cap_list *);
static void *_cap_list_front(const _cap_list *);
static void *_cap_list_back(const _cap_list *);
static void _cap_list_deep_free(_cap_list *);
static void _cap_list_free(_cap_list *);
#endif

static cap_dynamic_queue *cap_dynamic_queue_init() {
	cap_dynamic_queue *dynamic_queue =
	    (cap_dynamic_queue *)CAP_ALLOCATOR(cap_dynamic_queue, 1);
	if (!dynamic_queue) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	dynamic_queue->_internal_list = _cap_list_init();
	if (!dynamic_queue->_internal_list) {
		fprintf(stderr, "memory allocation failure\n");
		free(dynamic_queue);
		return NULL;
	}
	dynamic_queue->_current_size = 0;
	return dynamic_queue;
}

static void cap_dynamic_queue_free(cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	_cap_list_free(dynamic_queue->_internal_list);
	free(dynamic_queue);
}

static void cap_dynamic_queue_deep_free(cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	_cap_list_deep_free(dynamic_queue->_internal_list);
	free(dynamic_queue);
}

static size_t cap_dynamic_queue_size(const cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	return dynamic_queue->_current_size;
}

static bool cap_dynamic_queue_push(cap_dynamic_queue *dynamic_queue,
				   void *data) {
	assert((dynamic_queue != NULL) && (data != NULL));
	bool return_val =
	    _cap_list_push_front(dynamic_queue->_internal_list, data);
	if (return_val) dynamic_queue->_current_size++;
	return return_val;
}

static void *cap_dynamic_queue_pop(cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	if (dynamic_queue->_current_size <= 0) return NULL;
	void *returner = _cap_list_pop_back(dynamic_queue->_internal_list);
	if (returner) dynamic_queue->_current_size--;
	return returner;
}

static void *cap_dynamic_queue_front(cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	if (dynamic_queue->_current_size <= 0) return NULL;
	return _cap_list_back(dynamic_queue->_internal_list);
}

static void *cap_dynamic_queue_back(cap_dynamic_queue *dynamic_queue) {
	assert(dynamic_queue != NULL);
	return _cap_list_front(dynamic_queue->_internal_list);
}

static void cap_dynamic_queue_swap(cap_dynamic_queue *dynamic_queue_one,
				   cap_dynamic_queue *dynamic_queue_two) {
	assert((dynamic_queue_one != NULL) && (dynamic_queue_two != NULL));
	size_t one_size = dynamic_queue_one->_current_size;
	_cap_list *one_tmp_internal_list = dynamic_queue_one->_internal_list;
	dynamic_queue_one->_current_size = dynamic_queue_two->_current_size;
	dynamic_queue_one->_internal_list = dynamic_queue_two->_internal_list;
	dynamic_queue_two->_current_size = one_size;
	dynamic_queue_two->_internal_list = one_tmp_internal_list;
}

static _cap_list *_cap_list_init() {
	_cap_list_node *head_and_tail_nodes =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 2);
	if (!head_and_tail_nodes) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	_cap_list *d_list = (_cap_list *)CAP_ALLOCATOR(_cap_list, 1);
	if (!d_list) {
		fprintf(stderr, "memory allocation failure\n");
		free(head_and_tail_nodes);
		return NULL;
	}
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
	if (!new_node) {
		fprintf(stderr, "memory allocation failure\n");
		return false;
	}
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

#endif // !CAP_DYNAMIC_QUEUE_H
