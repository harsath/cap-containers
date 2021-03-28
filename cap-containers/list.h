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
#ifndef CAP_LIST_H
#define CAP_LIST_H
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

typedef struct _cap_list_node {
	CAP_GENERIC_TYPE_PTR data;
	struct _cap_list_node *next;
	struct _cap_list_node *previous;
} _cap_list_node;

// Head and tail are sentinel nodes. It is design decision.
typedef struct {
	_cap_list_node *_head_node;
	_cap_list_node *_tail_node;
} cap_list;

typedef struct {
	CAP_GENERIC_TYPE_PTR data;
	_cap_list_node *current_node;
} cap_list_iterator;
#endif // !DOXYGEN_SHOULD_SKIP_THIS

// Prototypes (Public APIs)
/**
 * Initilize a cap_list container
 *
 * @return  Allocated cap_list container
 */
static cap_list *cap_list_init();

// Lookup & Update:
/**
 * Iterate the list and find the first occurance of the element which matches the predicate-function
 *
 * @param list cap_list container
 * @param predicate_fn Predicate function where we will pass the elements(NULL is never passed into the predicate function)
 * @return Returns the element which matches the first occurance of the predicate, if none matches, NULL is returned
 */
static void *cap_list_find_if(const cap_list *list, bool (*predicate_fn)(void *));
/**
 * Push an element at the front of the cap_list container
 *
 * @param list cap_list container
 * @param item Element to push at the front of the list
 * @return True if the operation is success, False if memory failur
 */
static bool cap_list_push_front(cap_list *list, void *item);
/**
 * Push an element at the end of the cap_list container
 *
 * @param list cap_list container
 * @param item Element to push at the end of the list
 * @return True if the operation is success, False if memory failur
 */
static bool cap_list_push_back(cap_list *list, void *item);
/**
 * Pop the element at the front of the cap_list container
 *
 * @param list cap_list container
 * @return Returns the element if operation is success, returns NULL if cap_list container is empty
 */
static void *cap_list_pop_front(cap_list *list);
/**
 * Pop the element at the end of the cap_list container
 *
 * @param list cap_list container
 * @return Returns the element if operation is success, returns NULL if cap_list container is empty
 */
static void *cap_list_pop_back(cap_list *);
/**
 * Gives access to the front of the cap_list without modifying the container
 *
 * @param list cap_list container
 * @return Gives access to the front element of the cap_list container, returns NULL if the container is empty
 */
static void *cap_list_front(const cap_list *list);
/**
 * Gives access to the end of the cap_list without modifying the container
 *
 * @param list cap_list container
 * @return Gives access to the element at the back of the cap_list container, returns NULL if the container is empty
 */
static void *cap_list_back(const cap_list *list);
/**
 * Iterates the cap_list container and remove all occurance of the element which matches the predicate function
 *
 * @param list cap_list container
 * @param predicate_fn Predicate function which we pass the elements into, implementation ensures that predicate_fn will never gets a NULL as param.
 * @return True if any element is removed, False if the given predicate_fn does not match any of the elements
 */
static bool cap_list_remove_if(cap_list *list, bool (*predicate_fn)(void *));

// Memory:
/**
 * Frees the cap_list container object and also frees the elements which it contains(assuming the elements are dynamically allocated)
 *
 * @param list cap_list container
 */
static void cap_list_deep_free(cap_list *list);
/**
 * Frees only the cap_list container object and does not touch the elements which it contains
 *
 * @param list cap_list container
 */
static void cap_list_free(cap_list *list);

static cap_list *cap_list_init() {
	_cap_list_node *head_and_tail_nodes =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 2);
	cap_list *d_list = (cap_list *)malloc(sizeof(cap_list));
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

static bool cap_list_push_front(cap_list *d_list, void *data) {
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

static bool cap_list_push_back(cap_list *d_list, void *data) {
	assert((d_list != NULL) && (data != NULL));
	_cap_list_node *new_node =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 1);
	if (new_node == NULL) return false;
	new_node->data = (CAP_GENERIC_TYPE_PTR)data;
	new_node->next = d_list->_tail_node;
	_cap_list_node *tmp_tail_prev_holder = d_list->_tail_node->previous;
	tmp_tail_prev_holder->next = new_node;
	new_node->previous = tmp_tail_prev_holder;
	tmp_tail_prev_holder->next = new_node;
	d_list->_tail_node->previous = new_node;
	return true;
}

static void *cap_list_pop_front(cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *pop_node = d_list->_head_node->next;
	d_list->_head_node->next = pop_node->next;
	pop_node->next->previous = d_list->_head_node;
	return pop_node->data;
}

static void *cap_list_pop_back(cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *pop_node = d_list->_tail_node->previous;
	d_list->_tail_node->previous = pop_node->previous;
	pop_node->previous->next = d_list->_tail_node;
	return pop_node->data;
}

static void *cap_list_front(const cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_head_node->next->data;
}

static void *cap_list_back(const cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_tail_node->previous->data;
}

// We will pass the 'data' pointer to the predicate and return the 'data' which
// returns true; Returns NULL if not found
static void *cap_list_find_if(const cap_list *d_list,
			      bool (*predicate_fn)(void *)) {
	assert((d_list != NULL) && (predicate_fn != NULL));
	_cap_list_node *iter_node = d_list->_head_node->next;
	do {
		if (iter_node->data != NULL) {
			if (predicate_fn(iter_node->data)) {
				return iter_node->data;
			}
		} else {
			return NULL;
		}
		iter_node = iter_node->next;
	} while (iter_node != NULL);
	return NULL;
}

// Removes the first occurance of predicate's 'true'
static bool cap_list_remove_if(cap_list *d_list, bool (*predicate_fn)(void *)) {
	assert((d_list != NULL) && (predicate_fn != NULL));
	_cap_list_node *iter_node = d_list->_head_node->next;
	size_t num_removed = 0;
	do {
		if (iter_node->data != NULL) {
			if (predicate_fn(iter_node->data)) {
				iter_node->previous->next = iter_node->next;
				iter_node->next->previous = iter_node->previous;
				if (iter_node != NULL) free(iter_node);
				num_removed++;
			}
		}
		iter_node = iter_node->next;
	} while (iter_node != NULL && iter_node->next != NULL);
	return (num_removed != 0);
}

static bool cap_list_insert_after(cap_list *d_list,
				  void *insert_after_this_node, void *data) {
	assert((d_list != NULL) && (insert_after_this_node != NULL) &&
	       (data != NULL));
	_cap_list_node *new_node =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 1);
	if (new_node == NULL) return false;
	new_node->data = (CAP_GENERIC_TYPE_PTR)data;
	new_node->previous = (_cap_list_node *)insert_after_this_node;
	new_node->next = ((_cap_list_node *)insert_after_this_node)->next;
	((_cap_list_node *)insert_after_this_node)->next = new_node;
	new_node->next->previous = new_node;
	return true;
}

static void cap_list_free(cap_list *d_list) {
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

static void cap_list_deep_free(cap_list *d_list) {
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

#endif // !CAP_LIST_H
