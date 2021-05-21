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
#ifndef CAP_CIRCULAR_QUEUE_H
#define CAP_CIRCULAR_QUEUE_H
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

typedef struct {
	size_t _capacity;
	size_t _head_index;
	int _size;
	size_t _tail_index;
	CAP_GENERIC_TYPE_PTR *_internal_buffer;
} cap_circular_queue;
#endif

// Prototypes (Public API)
/**
 * Initilize the cap_circular_queue container with initial capacity
 *
 * @param capacity_of_queue Initial capacity of the circular queue
 * @return Allocated cap_circular_queue container
 */
static cap_circular_queue *cap_circular_queue_init(size_t capacity_of_queue);
/**
 * Query the capacity of the given cap_circular_queue
 *
 * @param cqueue cap_circular_queue container
 * @return Capacity of the container
 */
static size_t cap_circular_queue_capacity(const cap_circular_queue *cqueue);
/**
 * Push a new element to the cap_circular_queue container
 *
 * @param cqueue cap_circular_queue container
 */
static void cap_circular_queue_push(cap_circular_queue *cqueue, void *item);
/**
 * Pop an element from the cap_circular_queue container
 *
 * @param cqueue cap_circular_queue container
 */
static void *cap_circular_queue_pop(cap_circular_queue *cqueue);
/**
 * Query the size of the cap_circular_queue container. Returns the remaining
 * elements before which the call to 'pop' returns NULL
 *
 * @param cqueue cap_circular_queue container
 * @return Size of the container
 */
static int cap_circular_queue_size(cap_circular_queue *cqueue);
/**
 * Free the cap_circular_queue. This doesn't touch the memory of the underlying
 * element which this container holds, but only frees the memory which the
 * cap_circular_queue internally owns.
 *
 * @param cqueue cap_circular_queue container
 */
static void cap_circular_queue_free(cap_circular_queue *cqueue);
/**
 * Free the memory owned by the cap_circular_queue container and also the
 * element's memory which this container holds (assuming the memory is
 * dynamically allocated)
 *
 * @param cqueue cap_circular_queue container
 */
static void cap_circular_queue_deep_free(cap_circular_queue *cqueue);

static cap_circular_queue *cap_circular_queue_init(size_t capacity_of_queue) {
	assert(capacity_of_queue);
	cap_circular_queue *cqueue =
	    (cap_circular_queue *)CAP_ALLOCATOR(cap_circular_queue, 1);
	if (!cqueue) {
		fprintf(stderr, "memory allocation failur\n");
		return NULL;
	}
	cqueue->_capacity = capacity_of_queue;
	cqueue->_internal_buffer = (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(
	    CAP_GENERIC_TYPE_PTR, capacity_of_queue);
	if (!cqueue->_internal_buffer) {
		fprintf(stderr, "memory allocation failur\n");
		free(cqueue);
		return NULL;
	}
	cqueue->_head_index = 0;
	cqueue->_tail_index = 0;
	cqueue->_size = 0;
	return cqueue;
}

static void cap_circular_queue_push(cap_circular_queue *cqueue, void *item) {
	assert(cqueue != NULL);
	cqueue->_internal_buffer[cqueue->_tail_index] =
	    (CAP_GENERIC_TYPE_PTR)item;
	cqueue->_tail_index = (cqueue->_tail_index + 1) % cqueue->_capacity;
	cqueue->_size += 1;
}

static void *cap_circular_queue_pop(cap_circular_queue *cqueue) {
	assert(cqueue != NULL);
	if (!cqueue->_size) return NULL;
	void *returner = cqueue->_internal_buffer[cqueue->_head_index];
	cqueue->_head_index = (cqueue->_head_index + 1) % cqueue->_capacity;
	cqueue->_size -= 1;
	return returner;
}

static int cap_circular_queue_size(cap_circular_queue *cqueue) {
	assert(cqueue != NULL);
	return cqueue->_size;
}

static void cap_circular_queue_free(cap_circular_queue *cqueue) {
	assert(cqueue != NULL);
	if (cqueue->_internal_buffer) free(cqueue->_internal_buffer);
	free(cqueue);
}

static void cap_circular_queue_deep_free(cap_circular_queue *cqueue) {
	assert(cqueue != NULL);
	for (int index = cqueue->_head_index - 1; index >= cqueue->_tail_index;
	     index++)
		free(cqueue->_internal_buffer[index]);
	free(cqueue->_internal_buffer);
	free(cqueue);
}

static size_t cap_circular_queue_capacity(const cap_circular_queue *cqueue) {
	assert(cqueue);
	return cqueue->_capacity;
}

#endif // !CAP_CIRCULAR_QUEUE_H
