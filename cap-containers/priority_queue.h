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
#ifndef CAP_PRIORITY_QUEUE_H
#define CAP_PRIORITY_QUEUE_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	malloc(number_of_elements * sizeof(type))
#define CAP_MAP_MAX_SKIPLIST_SIZE 10

typedef struct cap_priority_queue {
	int _size;
	CAP_GENERIC_TYPE_PTR *_internal_buffer;
	int (*_compare_fn)(void *item_one, void *item_two);
	int _capacity;
} cap_priority_queue;

#endif // !DOXYGEN_SHOULD_SKIP_THIS

/**
 * Initilize a cap_priority_queue container object
 *
 * @param compare_fn Compare function for comparing two keys that will be passed
 * into the container. If item_one is larger than item_two, positive value is
 * returned but if it's lesser, negative value is returned and if they're equal,
 * zero is returned.
 * @return A priority queue container
 */
static cap_priority_queue *
cap_priority_queue_init(int (*compare_fn)(void *item_one, void *item_two));

/**
 * Query the current size of cap_priority_queue container
 *
 * @param pqueue cap_priority_queue container
 * @return Number of elements currently present at the container
 */
static int cap_priority_queue_size(cap_priority_queue *pqueue);

/**
 * Clear the size of the cap_priority_queue container
 *
 * @param pqueue cap_priority_queue container
 */
static void cap_priority_queue_clear(cap_priority_queue *pqueue);

/**
 * Get the element which will be poped from the container, but doesn't modify
 * the container. Gives access to the current root of the priority queue.
 *
 * @param pqueue cap_priority_queue container
 * @return Root item of the priority queue
 */
static void *cap_priority_queue_top(cap_priority_queue *pqueue);

/**
 * Pop the element at the root of the priority queue aka the one with the
 * highest priority.
 *
 * @param pqueue cap_priority_queue container
 * @return Root item that is poped from the container
 */
static void *cap_priority_queue_pop(cap_priority_queue *pqueue);

/**
 * Push an eleent onto the container
 *
 * If the capacity of the container is already reached, we will resize the
 * container to twice the current size (Current-size * 2) and copy all the items
 * over.
 *
 * @param pqueue cap_priority_queue container
 * @param item Push the item onto the priority queue
 * @return If there is no memory allocation error during resize operation,
 * returns true, if there is an error, returns false
 */
static bool cap_priority_queue_push(cap_priority_queue *pqueue, void *item);

/**
 * Free the cap_priority_queue container.
 *
 * This is ideally called at the end of the usage where the container is no
 * longer needed. This do not touch the element it container, but only frees the
 * memory that is dynamically allocated for the container i.e. Doesn't free the
 * items inside the container
 *
 * @param pqueue cap_priority_queue container
 */
static void cap_priority_queue_free(cap_priority_queue *pqueue);

static void _cap_priority_queue_swim(cap_priority_queue *pqueue);

static void _cap_priority_queue_sink(cap_priority_queue *pqueue);

static bool _cap_priority_queue_resize(cap_priority_queue *pqueue,
				       int new_capacity);

static cap_priority_queue *
cap_priority_queue_init(int (*compare_fn)(void *item_one, void *item_two)) {
	assert(compare_fn);
	cap_priority_queue *pqueue =
	    (cap_priority_queue *)CAP_ALLOCATOR(cap_priority_queue, 1);
	if (!pqueue) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	// Default capacity is 5, but it'll be increased in log(n) times.
	const size_t default_size = 5;
	pqueue->_internal_buffer = (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(
	    CAP_GENERIC_TYPE_PTR, default_size);
	if (!pqueue->_internal_buffer) {
		fprintf(stderr, "memory allocation failure\n");
		free(pqueue);
		return NULL;
	}
	pqueue->_capacity = default_size;
	pqueue->_size = 0;
	pqueue->_compare_fn = compare_fn;
	return pqueue;
}

static bool cap_priority_queue_push(cap_priority_queue *pqueue, void *item) {
	assert(pqueue && item);
	if (pqueue->_capacity <= pqueue->_size)
		if (!_cap_priority_queue_resize(pqueue, pqueue->_capacity * 2))
			return false;
	pqueue->_internal_buffer[++pqueue->_size] = (CAP_GENERIC_TYPE_PTR)item;
	_cap_priority_queue_swim(pqueue);
	return true;
}

static void _cap_priority_queue_swim(cap_priority_queue *pqueue) {
	int index = pqueue->_size;
	// pqueue->compare( current-node > parent-node )
	while (index > 1 &&
	       pqueue->_compare_fn(pqueue->_internal_buffer[index],
				   pqueue->_internal_buffer[index / 2]) > 0) {
		CAP_GENERIC_TYPE_PTR temp = pqueue->_internal_buffer[index];
		pqueue->_internal_buffer[index] =
		    pqueue->_internal_buffer[index / 2];
		pqueue->_internal_buffer[index / 2] = temp;
		index = index / 2;
	}
}

static void _cap_priority_queue_sink(cap_priority_queue *pqueue) {
	int index = 1;
	while (index * 2 <= pqueue->_size) {
		if (pqueue->_compare_fn(pqueue->_internal_buffer[index],
					pqueue->_internal_buffer[index * 2]) <
		    0) {
			void *temp = pqueue->_internal_buffer[index];
			pqueue->_internal_buffer[index] =
			    pqueue->_internal_buffer[index * 2];
			pqueue->_internal_buffer[index * 2] =
			    (CAP_GENERIC_TYPE_PTR)temp;
		} else {
			if (index * 2 + 1 <= pqueue->_size) {
				void *temp = pqueue->_internal_buffer[index];
				pqueue->_internal_buffer[index] =
				    pqueue->_internal_buffer[index * 2 + 1];
				pqueue->_internal_buffer[index * 2 + 1] =
				    (CAP_GENERIC_TYPE_PTR)temp;
			}
		}
		index *= 2;
	}
}

static bool _cap_priority_queue_resize(cap_priority_queue *pqueue,
				       int new_capacity) {
	CAP_GENERIC_TYPE_PTR *temp = (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(
	    CAP_GENERIC_TYPE_PTR, new_capacity);
	if (!temp) return false;
	memcpy(temp, pqueue->_internal_buffer,
	       sizeof(CAP_GENERIC_TYPE_PTR) * (pqueue->_size + 1));
	free(pqueue->_internal_buffer);
	pqueue->_internal_buffer = temp;
	pqueue->_capacity = new_capacity;
	return true;
}

static void *cap_priority_queue_pop(cap_priority_queue *pqueue) {
	assert(pqueue);
	if (pqueue->_size < 1) return NULL;
	void *returner = pqueue->_internal_buffer[1];
	pqueue->_internal_buffer[1] = pqueue->_internal_buffer[pqueue->_size--];
	_cap_priority_queue_sink(pqueue);
	return returner;
}

static void *cap_priority_queue_top(cap_priority_queue *pqueue) {
	assert(pqueue);
	return pqueue->_internal_buffer[1];
}

static int cap_priority_queue_size(cap_priority_queue *pqueue) {
	assert(pqueue);
	return pqueue->_size;
}

static void cap_priority_queue_free(cap_priority_queue *pqueue) {
	if (pqueue) {
		free(pqueue->_internal_buffer);
		free(pqueue);
	}
}

static void cap_priority_queue_clear(cap_priority_queue *pqueue) {
	assert(pqueue);
	pqueue->_size = 0;
}

#endif // !CAP_PRIORITY_QUEUE_H
