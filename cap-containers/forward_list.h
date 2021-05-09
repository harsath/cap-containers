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
#ifndef CAP_FORWARD_LIST_H
#define CAP_FORWARD_LIST_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	calloc(number_of_elements, sizeof(type))

typedef struct _cap_flist_node {
	CAP_GENERIC_TYPE_PTR data;
	struct _cap_flist_node *next;
} _cap_flist_node;

typedef struct {
	size_t size;
	_cap_flist_node *head;
} cap_forward_list;
#endif // !DOXYGEN_SHOULD_SKIP_THIS

// Prototypes (Public APIs)
// Init:
/**
 * Initilize a cap_forward_list container object
 *
 * @return Allocated cap_forward_list object
 */
static cap_forward_list *cap_forward_list_init();

// Lookup & Update:
/**
 * Search for an element with given predicate function by traversing the
 * container
 *
 * @param list cap_forward_list container
 * @param predicate_fn Predicate function(We pass the elements to the predicate
 * and NULL will never be passed onto the given predicate)
 * @return Returns pointer/element for the first match of an element of the
 * predicate_fn or if not found, returns NULL
 */
static void *cap_forward_list_find_if(cap_forward_list *list,
				      bool (*predicate_fn)(void *));
/**
 * Push an element onto the front of the cap_forward_list container
 *
 * @param list cap_forward_list container
 * @param item Element to be pushed
 * @return True if operation is sucess, or else returns False.
 */
static bool cap_forward_list_push_front(cap_forward_list *list, void *item);
/**
 * Pop an element from the cap_forward_list container
 *
 * @param list cap_forward_list container
 * @return Pointer of the element, if container is empty, returns NULL
 */
static void *cap_forward_list_pop_front(cap_forward_list *list);
/**
 * Gives access to the front element of the cap_forward_list container without
 * modifying the conainer
 *
 * @apram list cap_forward_list conainer
 * @return Pointer of the element, if container is empty, returns NULL
 */
static void *cap_forward_list_front(cap_forward_list *list);
/**
 * Query the size of the cap_forward_list container
 *
 * @param list cap_forward_list container
 * @return Size of the container
 */
static size_t cap_forward_list_size(cap_forward_list *list);
/**
 * Remove all occurance of an element which matches the predicate
 *
 * We traverse the cap_forward_list container and pass each element to the
 * predicate function and remove all the elements which matches the given
 * predicate
 * @param list cap_forward_list container
 * @return predicate_fn Predicate function
 */
static bool cap_forward_list_remove_if(cap_forward_list *list,
				       bool (*predicate_fn)(void *));
/**
 * Query if the cap_forward_list container is empty or not.
 *
 * @param list cap_forward_list container
 * @return True if container is empty, if not returns False.
 */
static bool cap_forward_list_empty(cap_forward_list *list);
/**
 * Swap two cap_forward_list container's internal members
 *
 * @param list_one First list
 * @param list_two Second list
 */
static void cap_forward_list_swap(cap_forward_list *list_one,
				  cap_forward_list *list_two);

// Memory:
/**
 * Free cap_forward_list container without touching the elements it container.
 *
 * @param list cap_forward_list container
 */
static void cap_forward_list_free(cap_forward_list *list);
/**
 * Free cap_forward_list container and also the elements it containers(assuming
 * the elements are dynamically allocated)
 *
 * @param list cap_forward_list container
 */
static void cap_forward_list_deep_free(cap_forward_list *list);

static cap_forward_list *cap_forward_list_init() {
	cap_forward_list *f_list =
	    (cap_forward_list *)CAP_ALLOCATOR(cap_forward_list, 1);
	if (!f_list) {
		fprintf(stderr, "memory allocation failur\n");
		return NULL;
	}
	f_list->head = NULL;
	f_list->size = 0;
	return f_list;
}

static bool cap_forward_list_push_front(cap_forward_list *f_list, void *data) {
	assert(f_list != NULL && data != NULL);
	_cap_flist_node *current_node = f_list->head;
	f_list->head = (_cap_flist_node *)CAP_ALLOCATOR(_cap_flist_node, 1);
	if (!f_list->head) {
		fprintf(stderr, "memory allocation failur\n");
		return false;
	}
	f_list->head->data = (CAP_GENERIC_TYPE_PTR)data;
	f_list->head->next = current_node;
	f_list->size++;
	return true;
}

static void *cap_forward_list_pop_front(cap_forward_list *f_list) {
	assert(f_list != NULL);
	if (!f_list->head) return NULL;
	void *returner = f_list->head->data;
	f_list->head = f_list->head->next;
	f_list->size--;
	return returner;
}

static void *cap_forward_list_front(cap_forward_list *f_list) {
	assert(f_list != NULL);
	if (f_list->head == NULL) return NULL;
	return f_list->head->data;
}

static void cap_forward_list_swap(cap_forward_list *f_list_one,
				  cap_forward_list *f_list_two) {
	assert(f_list_one != NULL && f_list_two != NULL);
	_cap_flist_node *one_temp_head = f_list_one->head;
	size_t one_temp_size = f_list_one->size;
	f_list_one->head = f_list_two->head;
	f_list_one->size = f_list_two->size;
	f_list_two->head = one_temp_head;
	f_list_two->size = one_temp_size;
}

static void cap_forward_list_free(cap_forward_list *f_list) {
	assert(f_list != NULL);
	_cap_flist_node *current_node = f_list->head;
	while (current_node != NULL) {
		_cap_flist_node *next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}

static size_t cap_forward_list_size(cap_forward_list *f_list) {
	assert(f_list != NULL);
	return f_list->size;
}

static void cap_forward_list_deep_free(cap_forward_list *f_list) {
	assert(f_list != NULL);
	_cap_flist_node *current_node = f_list->head;
	while (current_node != NULL) {
		_cap_flist_node *next_node = current_node->next;
		free(current_node);
		free(current_node->data);
		current_node = next_node;
	}
}

static bool cap_forward_list_empty(cap_forward_list *f_list) {
	assert(f_list != NULL);
	return (f_list->size == 0);
}

static void *cap_forward_list_find_if(cap_forward_list *f_list,
				      bool (*predicate_fn)(void *)) {
	assert(f_list != NULL && predicate_fn != NULL);
	_cap_flist_node *current_node = f_list->head;
	while (current_node != NULL) {
		if (current_node->data != NULL &&
		    predicate_fn(current_node->data))
			return current_node->data;
		current_node = current_node->next;
	}
	return NULL;
}

static bool cap_forward_list_remove_if(cap_forward_list *f_list,
				       bool (*predicate_fn)(void *)) {
	assert(f_list != NULL && predicate_fn != NULL);
	_cap_flist_node *current_node = f_list->head;
	_cap_flist_node *prev_node = NULL;
	size_t num_removed = 0;
	while (current_node != NULL) {
		if (current_node->data != NULL &&
		    predicate_fn(current_node->data)) {
			if (prev_node == NULL && current_node->next == NULL) {
				f_list->head = NULL;
				f_list->size = 0;
				return true;
			} else if (prev_node == NULL &&
				   current_node->next != NULL) {
				f_list->head = current_node->next;
				f_list->size--;
				free(current_node);
			} else {
				prev_node->next = current_node->next;
				f_list->size--;
				free(current_node);
			}
			num_removed += 1;
		} else {
			prev_node = current_node;
			current_node = current_node->next;
		}
	}
	return (num_removed != 0);
}

#endif // !CAP_FORWARD_LIST_H
