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
#ifndef CAP_VECTOR_H
#define CAP_VECTOR_H
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
	calloc(number_of_elements, sizeof(type))

// Abstract type
typedef struct {
	CAP_GENERIC_TYPE_PTR *_internal_buffer;
	size_t _size;
	size_t _capacity;
} cap_vector;

typedef struct {
	cap_vector *_internal_pointer;
	size_t _current_index;
	size_t _capacity;
	void *data;
} cap_vector_iterator;
#endif // !DOXYGEN_SHOULD_SKIP_THIS
// Prototypes (Public APIs)
// Init:
/**
 * Initilize a cap_vector with some size(like std::vector::reserved)
 *
 * @param init_size Initial size
 * @return Allocated cap_vector container
 */
static cap_vector *cap_vector_init(size_t init_size);
/**
 * Initilize an Iterator object for iterating over a cap_vector container
 *
 * @param vector cap_vector container for which we need to create a iterator
 * @return Allocated cap_vector_iterator object for iterating the container
 * which is given in the parameter
 */
static cap_vector_iterator *cap_vector_iterator_init(cap_vector *vector);

// Lookup & Update:
/**
 * Return the element on the cap_vector container the the specific index
 *
 * @param vector cap_vector container
 * @param index The index value to get the element
 * @return Element at the index, if the index is larget then the number of
 * elements or the cap_vector container is empty, it returns NULL
 */
static void *cap_vector_at(cap_vector *vector, size_t index);
/**
 * Pop the element at the front of the cap_vector container
 *
 * This operation might hurt performance, since this involves moving every item
 * from index 1 to index N
 * @param vector cap_vector container
 * @return First element, which is poped
 */
static void *cap_vector_pop_front(cap_vector *vector);
/**
 * Copy the element's pointers but not the underlying element itself.
 *
 * This operation will result in a cap_vector container which will point to the
 * same underlying elements
 * @param vector cap_vector container to copy from
 * @return New cap_vector container which will point to the same underlying
 * elements(like an alias)
 */
static cap_vector *cap_vector_copy(cap_vector *vector);
/**
 * This operation will copy the elements to a new cap_vector container and also
 * copies the underlying element's representation and does not point to the same
 * underlying elements but it's own version of the element's pointers.
 *
 * @param vector cap_vector container
 * @return New cap_vector with underlying pointer not pointing to same elements
 * but it's own copy
 */
static cap_vector *cap_vector_deep_copy(cap_vector *vector);
/**
 * Access the front element of the cap_vector container
 *
 * @param vector cap_vector container
 * @return Returns the first element of the container, if the container is
 * empty, it returns NULL
 */
static void *cap_vector_front(cap_vector *vector);
/**
 * Access the back element of the cap_vector container
 *
 * @param vector cap_vector container
 * @return Returns the back/last element of the container, if the container is
 * empty, it returns NULL
 */
static void *cap_vector_back(cap_vector *vector);
/**
 * Push an element at the back of the container.
 *
 * If the cap_vector container reaches it's capacity, the container gets resized
 * with order <b>2 * CURRENT-CAPACITY</b> and moves all the elements, same as
 * std::vector. If there is any error when allocating space for the new memory
 * capacity, the current state of the container is preserved and returns False
 * indicating that a memory error has occured and a user should handle that
 * effectively
 * @param vector cap_vector container
 * @param item Element to push back onto the container
 * @return Returns True if the operation is success, returns False if a memory
 * error occured in such a case, the current state of the container is preserved
 */
static bool cap_vector_push_back(cap_vector *vector, void *item);
/**
 * Pop an element at the back of the container
 *
 * @param vector cap_vector container
 * @return Last element of the container, returns NULL if the container is empty
 */
static void *cap_vector_pop_back(cap_vector *vector);
/**
 * Swap two cap_vector container's internal members
 *
 * @param vector_one First cap_vector container
 * @param vector_two Second cap_vector container
 */
static void cap_vector_swap(cap_vector *vector_one, cap_vector *vector_two);
/**
 * Find the first occurance of the element which matches the given predicate
 * function. Implementations ensures that NULL will never be passed into the
 * predicate function
 *
 * @param vector cap_vector container
 * @param predicate_fn Predicate function where we pass the items and check the
 * boolean return
 * @return Returns the first occurance of the item which matches the
 * predicate_fn, if none matches, NULL is returned
 */
static void *cap_vector_find_if(cap_vector *vector,
				bool (*predicate_fn)(void *));
/**
 * Remove all the occurance of the elements which matches the given predicate in
 * the container
 *
 * @param vector cap_vector container
 * @param predicate_fn Predicate function to check against
 * @return True if any element is removed, False if none of the elements matches
 * the predicate_fn and none is removed
 */
static bool cap_vector_remove_if(cap_vector *vector,
				 bool (*predicate_fn)(void *));
/**
 * Pass all the elements from the container into a function.
 *
 * Typical usage is assumed to use this to do something for every item of the
 * container since we iterate over the container by passing all elements into
 * the fn_ptr function
 * @param vector cap_vector container
 * @param fn_ptr function to which we need to pass the elements as we iterate
 */
static void cap_vector_for_each(cap_vector *vector, void (*fn_ptr)(void *));

// Memory:
/**
 * Resize the cap_vector container to the given size(just list
 * std::vector::resize)
 *
 * @param vector cap_vector container
 * @param resize Size to resize the container to.
 * @param True if the operation is success, False if there is a memory error(in
 * which the case, the use has to handle that effectively). But the result of a
 * memory error doesn't modify teh current state of the container
 */
static bool cap_vector_resize(cap_vector *vector, size_t resize);
/**
 * Shrink the capacity of the cap_vector container to the current size/number of
 * elements on the container. Just like std::vector::shrink_to_fit operation
 *
 * @param vector cap_vector container
 * @param True if the operation is success, False if there is a memory error(in
 * which the case, the use has to handle that effectively). But the result of a
 * memory error doesn't modify teh current state of the container
 */
static bool cap_vector_shrink_to_fit(cap_vector *vector);
/**
 * Free the cap_vector container object and doesn't touch the underlying
 * elements which it contains.
 *
 * @param vector cap_vector container to be freed
 */
static void cap_vector_free(cap_vector *vector);
/**
 * Free the cap_vector container and also the underlying elements(assuming the
 * items are dynamically allocated)
 *
 * @param vector cap_vector container
 */
static void cap_vector_deep_free(cap_vector *vector);

// Checks:
/**
 * Query the size of the container
 *
 * @param vector cap_vector container
 * @return size of the container/number of elements
 */
static size_t cap_vector_size(cap_vector *vector);
/**
 * Query the current capacity of the cap_vector container
 *
 * @param vector cap_vector container
 * @return capacity of the container
 */
static size_t cap_vector_capacity(cap_vector *vector);
/**
 * Query if the container is empty or not
 *
 * @param vector cap_vector container
 * @return True if container is empty, False if not.
 */
static bool cap_vector_empty(cap_vector *vector);
/**
 * Query for the remaining space left in the container before it gets resized
 * into <b>2 * CURRENT-CAPACITY</b>
 *
 * @param vector cap_vector container
 * @return Remaining space in the container.
 */
static size_t cap_vector_remaining_space(cap_vector *vector);

// Iterator:
/**
 * Insert an item at the current position of the Iterator(ie replaces the item
 * at it's place)
 *
 * @param iterator cap_vector_iterator iterator object
 * @param item Element to replace.
 */
static void cap_vector_iterator_insert(cap_vector_iterator *iterator,
				       void *item);
/**
 * Remove the element at the current position of the Iterator
 *
 * @param iterator cap_vector_iterator iterator object
 */
static void cap_vector_iterator_remove(cap_vector_iterator *iterator);
/**
 * Increment the Iterator to the next element on the container
 *
 * @param iterator cap_vector_iterator iterator object
 */
static void cap_vector_iterator_increment(cap_vector_iterator *iterator);
/**
 * Decrement the Iterator object to the previous element on the container
 *
 * @param iterator cap_vector_iterator iterator object
 */
static void cap_vector_iterator_decrement(cap_vector_iterator *iterator);
/**
 * Peek into the next element of the container without incrementing the iterator
 *
 * @param iterator cap_vector_iterator iterator object
 * @return Next element of the container, if the iterator is at the last
 * element, it returns NULL
 */
static void *cap_vector_iterator_next(cap_vector_iterator *iterator);
/**
 * Peek into the previous element of the container without decrementing the
 * iterator
 *
 * @param iterator cap_vector_iterator iterator object
 * @return Previous element of the container, if the iterator is at the first
 * element, it returns NULL
 */
static void *cap_vector_iterator_previous(cap_vector_iterator *iterator);
/**
 * Check if two elements which pointed by the iterators are same
 *
 * @param iter_one First iterator
 * @param iter_two Secound iterator
 */
static bool cap_vector_iterator_equals(cap_vector_iterator *iter_one,
				       cap_vector_iterator *iter_two);
/**
 * Check if an element which is pointed by the iterator matches the given
 * predicate function
 *
 * @param iterator cap_vector_iterator iterator object
 * @param predicate_fn Predicate function
 * @return Returns True, if predicate_fn says so, if not returns False.
 */
static bool cap_vector_iterator_equals_predicate(cap_vector_iterator *iterator,
						 bool (*predicate_fn)(void *));
/**
 * Get index of the element where the current iterator points to
 *
 * @param iterator cap_vector_iterator iterator
 * @return Index of the element pointed by the cap_vector_iterator object
 */
static size_t cap_vector_iterator_index(cap_vector_iterator *iterator);
/**
 * Get an iterator to the start of a cap_vector container
 *
 * @param vector cap_vector container
 * @return Allocated cap_vector_iterator iterator which points to the first
 * element of the container
 */
static cap_vector_iterator *cap_vector_begin(cap_vector *vector);
/**
 * Get an iterator to the last element of the cap_vector container
 *
 * @param vector cap_vector container
 * @return Allocated cap_vector_iterator iterator which points to the last
 * element of the container
 */
static cap_vector_iterator *cap_vector_end(cap_vector *vector);
/**
 * Free the cap_vector_iterator iterator object. This operation doesn't free the
 * underlying element which this iterator points to but only the
 * cap_vector_iterator object
 *
 * @param iterator cap_vector_iterator iterator object to be freed
 */
static void cap_vector_iterator_free(cap_vector_iterator *iterator);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Prototypes (Internal helpers)
static bool _cap_vector_reserve(cap_vector *, size_t);
#endif // !DOXYGEN_SHOULD_SKIP_THIS

// Implementations:
static cap_vector *cap_vector_init(size_t initial_size) {
	assert(initial_size > 0);
	cap_vector *vector = (cap_vector *)CAP_ALLOCATOR(cap_vector, 1);
	if (!vector) {
		fprintf(stderr, "memory allocation failue\n");
		return NULL;
	}
	vector->_internal_buffer = (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(
	    CAP_GENERIC_TYPE_PTR, initial_size);
	vector->_capacity = initial_size;
	vector->_size = 0;
	return vector;
}

static cap_vector_iterator *cap_vector_iterator_init(cap_vector *vector) {
	assert(vector != NULL);
	cap_vector_iterator *iterator =
	    (cap_vector_iterator *)CAP_ALLOCATOR(cap_vector_iterator, 1);
	if (!iterator) {
		fprintf(stderr, "memory allocation failue\n");
		return NULL;
	}
	iterator->_current_index = 0;
	iterator->_capacity = vector->_capacity;
	iterator->_internal_pointer = vector;
	iterator->data = vector->_internal_buffer[0];
	return iterator;
}

static bool _cap_vector_reserve(cap_vector *vector, size_t new_size) {
	assert(vector != NULL && new_size > 0);
	CAP_GENERIC_TYPE_PTR *tmp_ptr = (CAP_GENERIC_TYPE_PTR *)realloc(
	    vector->_internal_buffer, sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if (tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static void cap_vector_deep_free(cap_vector *vector) {
	assert(vector != NULL);
	if (vector->_internal_buffer != NULL) {
		for (size_t i = 0; i < vector->_size; i++)
			if (vector->_internal_buffer[i] != NULL)
				free(vector->_internal_buffer[i]);
		free(vector->_internal_buffer);
	}
	free(vector);
}

static void cap_vector_free(cap_vector *vector) {
	assert(vector != NULL);
	free(vector->_internal_buffer);
	free(vector);
}

static bool cap_vector_resize(cap_vector *vector, size_t new_size) {
	assert(vector != NULL && new_size >= 0);
	CAP_GENERIC_TYPE_PTR *tmp_ptr = (CAP_GENERIC_TYPE_PTR *)realloc(
	    vector->_internal_buffer, sizeof(CAP_GENERIC_TYPE_PTR) * new_size);
	if (tmp_ptr == NULL) return false;
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = new_size;
	return true;
}

static cap_vector *cap_vector_copy(cap_vector *vector) {
	assert(vector != NULL);
	cap_vector *copy_vector = (cap_vector *)CAP_ALLOCATOR(cap_vector, 1);
	if (!copy_vector) {
		fprintf(stderr, "memory allocation failue\n");
		return NULL;
	}
	copy_vector->_capacity = vector->_capacity;
	copy_vector->_internal_buffer = vector->_internal_buffer;
	copy_vector->_size = vector->_size;
	return copy_vector;
}

static cap_vector *cap_vector_deep_copy(cap_vector *vector) {
	assert(vector != NULL);
	cap_vector *deep_copy_vector =
	    (cap_vector *)CAP_ALLOCATOR(cap_vector, 1);
	if (!deep_copy_vector) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	deep_copy_vector->_capacity = vector->_capacity;
	deep_copy_vector->_size = vector->_size;
	deep_copy_vector->_internal_buffer =
	    (CAP_GENERIC_TYPE_PTR *)CAP_ALLOCATOR(CAP_GENERIC_TYPE_PTR,
						  vector->_capacity);
	if (!deep_copy_vector->_internal_buffer) {
		fprintf(stderr, "memory allocation failue\n");
		free(deep_copy_vector);
		return NULL;
	}
	memcpy(deep_copy_vector->_internal_buffer, vector->_internal_buffer,
	       sizeof(CAP_GENERIC_TYPE_PTR) * vector->_capacity);
	return deep_copy_vector;
}

static void *cap_vector_pop_front(cap_vector *vector) {
	assert(vector != NULL);
	if (vector->_size <= 0) return NULL;
	CAP_GENERIC_TYPE_PTR returner = vector->_internal_buffer[0];
	memmove(&vector->_internal_buffer[0], &vector->_internal_buffer[1],
		sizeof(CAP_GENERIC_TYPE_PTR) * (vector->_size - 1));
	vector->_size--;
	return returner;
}

static bool cap_vector_push_back(cap_vector *vector, void *data) {
	assert(vector != NULL && data != NULL);
	if (vector->_capacity <= vector->_size)
		if (!_cap_vector_reserve(vector, vector->_capacity * 2))
			return false;
	vector->_internal_buffer[vector->_size++] = (CAP_GENERIC_TYPE_PTR)data;
	return true;
}

static void *cap_vector_pop_back(cap_vector *vector) {
	assert(vector != NULL);
	if (vector->_size <= 0) return NULL;
	return vector->_internal_buffer[--vector->_size];
}

static void *cap_vector_find_if(cap_vector *vector,
				bool (*predicate_fn)(void *)) {
	assert(vector != NULL && predicate_fn != NULL);
	size_t current_index = 0;
	if (!vector->_size) return NULL;
	do {
		if (vector->_internal_buffer[current_index] != NULL)
			if (predicate_fn(
				vector->_internal_buffer[current_index]))
				return vector->_internal_buffer[current_index];
		current_index++;
	} while (vector->_size >= current_index);
	return NULL;
}

static bool cap_vector_remove_if(cap_vector *vector,
				 bool (*predicate_fn)(void *)) {
	assert(vector != NULL && predicate_fn != NULL);
	size_t num_removed = 0;
	if (!vector->_size) return false;
	size_t write_index = 0;
	for (size_t read_index = 0; read_index < vector->_size; read_index++) {
		if (predicate_fn(vector->_internal_buffer[read_index])) {
			num_removed++;
			continue;
		}
		if (read_index != write_index)
			vector->_internal_buffer[write_index] =
			    vector->_internal_buffer[read_index];
		write_index++;
	}
	vector->_size = (vector->_size - num_removed);
	return (num_removed != 0);
}

static void cap_vector_for_each(cap_vector *vector,
				void (*callback_fn)(void *)) {
	assert(vector != NULL && callback_fn != NULL);
	if (!vector->_size) return;
	size_t current_index = 0;
	do {
		if (vector->_internal_buffer[current_index] != NULL)
			callback_fn(vector->_internal_buffer[current_index]);
		current_index++;
	} while (vector->_size >= current_index);
}

static bool cap_vector_shrink_to_fit(cap_vector *vector) {
	assert(vector != NULL);
	CAP_GENERIC_TYPE_PTR *tmp_ptr = (CAP_GENERIC_TYPE_PTR *)realloc(
	    vector->_internal_buffer,
	    sizeof(CAP_GENERIC_TYPE_PTR) * vector->_size);
	if (!tmp_ptr) {
		fprintf(stderr, "memory allocation failure\n");
		return false;
	}
	vector->_internal_buffer = tmp_ptr;
	vector->_capacity = vector->_size;
	return true;
}

static void cap_vector_swap(cap_vector *vector_one, cap_vector *vector_two) {
	assert(vector_one != NULL && vector_two != NULL);
	CAP_GENERIC_TYPE_PTR *one_temp_internal_ptr =
	    vector_one->_internal_buffer;
	size_t one_temp_size = vector_one->_size;
	size_t one_temp_cap = vector_one->_capacity;
	vector_one->_internal_buffer = vector_two->_internal_buffer;
	vector_one->_size = vector_two->_size;
	vector_one->_capacity = vector_two->_capacity;
	vector_two->_internal_buffer = one_temp_internal_ptr;
	vector_two->_size = one_temp_size;
	vector_two->_capacity = one_temp_cap;
}

static bool cap_vector_empty(cap_vector *vector) {
	assert(vector != NULL);
	return (vector->_size == 0);
}

static size_t cap_vector_size(cap_vector *vector) {
	assert(vector != NULL);
	return vector->_size;
}

static size_t cap_vector_capacity(cap_vector *vector) {
	assert(vector != NULL);
	return vector->_capacity;
}

static size_t cap_vector_remaining_space(cap_vector *vector) {
	assert(vector != NULL);
	return (vector->_capacity - vector->_size);
}

static void *cap_vector_front(cap_vector *vector) {
	assert(vector != NULL);
	return vector->_internal_buffer[0];
}

static void *cap_vector_back(cap_vector *vector) {
	assert(vector != NULL);
	return vector->_size == 0 ? NULL
				  : vector->_internal_buffer[vector->_size - 1];
}

static void *cap_vector_at(cap_vector *vector, size_t index) {
	assert(vector != NULL && index > 0);
	return vector->_size < index ? NULL : &vector->_internal_buffer[index];
}

static void cap_vector_iterator_insert(cap_vector_iterator *iterator,
				       void *data) {
	assert(iterator != NULL && data != NULL);
	iterator->_internal_pointer
	    ->_internal_buffer[iterator->_current_index] =
	    (CAP_GENERIC_TYPE_PTR)data;
	iterator->data = data;
}

static void cap_vector_iterator_remove(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	if (!iterator->_internal_pointer->_size) return;
	if (iterator->_internal_pointer->_size ==
	    iterator->_current_index + 1) {
		iterator->data = NULL;
		iterator->_internal_pointer->_size--;
		return;
	}
	memmove(&iterator->_internal_pointer
		     ->_internal_buffer[iterator->_current_index],
		&iterator->_internal_pointer
		     ->_internal_buffer[iterator->_current_index + 1],
		sizeof(CAP_GENERIC_TYPE_PTR) *
		    (iterator->_internal_pointer->_capacity -
		     (iterator->_current_index + 1)));
	iterator->data = iterator->_internal_pointer
			     ->_internal_buffer[iterator->_current_index];
	iterator->_internal_pointer->_size--;
}

static void cap_vector_iterator_increment(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	if ((!iterator->_internal_pointer->_size) ||
	    iterator->_internal_pointer->_size ==
		iterator->_current_index + 1) {
		iterator->data = NULL;
		return;
	}
	if (iterator->data != NULL) iterator->_current_index++;
	iterator->data = iterator->_internal_pointer
			     ->_internal_buffer[iterator->_current_index];
}

static void *cap_vector_iterator_next(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	if (iterator->_internal_pointer->_size < (iterator->_current_index + 1))
		return NULL;
	return iterator->_internal_pointer
	    ->_internal_buffer[(iterator->_current_index + 1)];
}

static void *cap_vector_iterator_previous(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	if ((iterator->_current_index - 1) < 0) return NULL;
	return iterator->_internal_pointer
	    ->_internal_buffer[(iterator->_current_index - 1)];
}

static bool cap_vector_iterator_equals(cap_vector_iterator *iter_one,
				       cap_vector_iterator *iter_two) {
	assert(iter_one != NULL && iter_two != NULL);
	if (iter_one->data == iter_two->data) return true;
	return false;
}

static cap_vector_iterator *cap_vector_begin(cap_vector *vector) {
	assert(vector != NULL);
	// begin() is alias to init
	return cap_vector_iterator_init(vector);
}

static cap_vector_iterator *cap_vector_end(cap_vector *vector) {
	assert(vector != NULL);
	cap_vector_iterator *iterator = cap_vector_iterator_init(vector);
	iterator->_current_index = (vector->_size - 1);
	iterator->data = vector->_internal_buffer[iterator->_current_index];
	return iterator;
}

static bool cap_vector_iterator_equals_predicate(cap_vector_iterator *iter,
						 bool (*predicate_fn)(void *)) {
	assert(iter != NULL && predicate_fn != NULL);
	return predicate_fn(iter->data);
}

static size_t cap_vector_iterator_index(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	return iterator->_current_index;
}

static void cap_vector_iterator_decrement(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	if (iterator->_current_index <= 0) {
		iterator->data = NULL;
		return;
	}
	if (iterator->data != NULL) iterator->_current_index--;
	iterator->data = iterator->_internal_pointer
			     ->_internal_buffer[iterator->_current_index];
}

static void cap_vector_iterator_free(cap_vector_iterator *iterator) {
	assert(iterator != NULL);
	free(iterator);
}

#endif // !CAP_VECTOR_H
