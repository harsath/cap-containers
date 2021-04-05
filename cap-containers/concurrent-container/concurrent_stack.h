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
#ifndef CAP_CONCURRENT_STACK_H
#define CAP_CONCURRENT_STACK_H
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_STACK_INITIAL_SIZE 10
#define CAP_CHECK_NULL(value)                                                  \
	if (value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	calloc(number_of_elements, sizeof(type))
#define CAP_PTHREAD_MUTEX_LOCK_STATUS(return_value)                            \
	do {                                                                   \
		if (return_value != 0) {                                       \
			perror("pthread_mutex_lock");                          \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)
#define CAP_PTHREAD_MUTEX_UNLOCK_STATUS(return_value)                          \
	do {                                                                   \
		if (return_value != 0) {                                       \
			perror("pthread_mutex_unlock");                        \
			exit(EXIT_FAILURE);                                    \
		}                                                              \
	} while (0)

// Abstract type
typedef struct {
	CAP_GENERIC_TYPE_PTR *_internal_buffer;
	size_t _size;
	size_t _capacity;
} _cap_vector;

typedef struct {
	_cap_vector *_internal_container;
	pthread_mutex_t _stack_mtx;
} cap_stack;
#endif // !DOXYGEN_SHOULD_SKIP_THIS

/**
 * These are the concurrent or thread-safe version of the containers. There is
 * no difference in the operation. We use mutex locks during performing
 * operations to make it thread-safe and keep the Stack's invariant
 * valid
 */

// Prototypes (Public APIs)
/**
 * Initilize a cap_stack object
 *
 * @return Allocated cap_stack container
 */
static cap_stack *cap_stack_init();
/**
 * Push an element onto the cap_stack container
 *
 * @param stack cap_stack container
 * @param item Element to push onto the stack
 * @return True if success, or else returns False
 */
static bool cap_stack_push(cap_stack *stack, void *item);
/**
 * Pop an element onto the stack
 *
 * @param stack cap_stack container
 * @return Pop-ed item from the stack, if cap_stack is empty, returns NULL
 */
static void *cap_stack_pop(cap_stack *stack);
/**
 * Gives access to the top of the stack without modifying the element
 *
 * @param stack cap_stack container
 * @return Item on the top of the stack, returns NULL if cap_stack is empty
 */
static void *cap_stack_top(cap_stack *stack);
/**
 * Query for the size of the cap_stack container
 *
 * @param stack cap_stack container
 * @return Size/number of elements on the container.
 */
static size_t cap_stack_size(cap_stack *stack);
/**
 * Check if the cap_stack is empty
 *
 * @param stack cap_stack container
 * @return True if cap_stack is empty, or else returns False.
 */
static bool cap_stack_empty(cap_stack *stack);

// Prototypes (Internal helpers)
#ifndef DOXYGEN_SHOULD_SKIP_THIS
static bool _cap_vector_push_back(_cap_vector *, void *);
static _cap_vector *_cap_vector_init(size_t);
static void *_cap_vector_pop_back(_cap_vector *);
static void *_cap_vector_back(_cap_vector *);
#endif // !DOXYGEN_SHOULD_SKIP_THIS

static cap_stack *cap_stack_init() {
	cap_stack *stack = (cap_stack *)CAP_ALLOCATOR(cap_stack, 1);
	CAP_CHECK_NULL(stack);
	stack->_internal_container = _cap_vector_init(CAP_STACK_INITIAL_SIZE);
	stack->_stack_mtx = PTHREAD_MUTEX_INITIALIZER;
	return stack;
}

static bool cap_stack_push(cap_stack *stack, void *data) {
	assert(stack != NULL && data != NULL);
	int ret = pthread_mutex_lock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	bool return_value =
	    _cap_vector_push_back(stack->_internal_container, data);
	ret = pthread_mutex_unlock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void *cap_stack_pop(cap_stack *stack) {
	assert(stack != NULL);
	int ret = pthread_mutex_lock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	void *return_value = _cap_vector_pop_back(stack->_internal_container);
	ret = pthread_mutex_unlock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void *cap_stack_top(cap_stack *stack) {
	assert(stack != NULL);
	int ret = pthread_mutex_lock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	void *return_value = _cap_vector_back(stack->_internal_container);
	ret = pthread_mutex_unlock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static size_t cap_stack_size(cap_stack *stack) {
	assert(stack != NULL);
	int ret = pthread_mutex_lock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	size_t return_value = stack->_internal_container->_size;
	ret = pthread_mutex_unlock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static bool cap_stack_empty(cap_stack *stack) {
	assert(stack != NULL);
	int ret = pthread_mutex_lock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	bool return_value = (stack->_internal_container->_size == 0);
	ret = pthread_mutex_unlock(&stack->_stack_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static _cap_vector *_cap_vector_init(size_t initial_size) {
	assert(initial_size > 0);
	_cap_vector *vector = (_cap_vector *)CAP_ALLOCATOR(_cap_vector, 1);
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(
	    CAP_GENERIC_TYPE_PTR, initial_size);
	vector->_capacity = initial_size;
	vector->_size = 0;
	return vector;
}

static bool _cap_vector_reserve(_cap_vector *vector, size_t new_size) {
	assert(vector != NULL && new_size > 0);
	CAP_GENERIC_TYPE_PTR *tmp_ptr = (CAP_GENERIC_TYPE_PTR *)realloc(
	    vector->_internal_buffer, sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if (tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static bool _cap_vector_push_back(_cap_vector *vector, void *data) {
	assert(vector != NULL && data != NULL);
	if (vector->_capacity <= vector->_size)
		if (!_cap_vector_reserve(vector, vector->_capacity * 2))
			return false;
	vector->_internal_buffer[vector->_size++] = (CAP_GENERIC_TYPE_PTR)data;
	return true;
}

static void *_cap_vector_pop_back(_cap_vector *vector) {
	assert(vector != NULL);
	if (vector->_size <= 0) return NULL;
	return vector->_internal_buffer[--vector->_size];
}

static void *_cap_vector_back(_cap_vector *vector) {
	assert(vector != NULL);
	return vector->_size == 0 ? NULL
				  : vector->_internal_buffer[vector->_size - 1];
}

#endif // !CAP_CONCURRENT_STACK_H
