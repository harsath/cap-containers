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
#ifndef CAP_CONCURRENT_FORWARD_LIST_H
#define CAP_CONCURRENT_FORWARD_LIST_H
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
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

typedef struct _cap_flist_node {
	CAP_GENERIC_TYPE_PTR data;
	struct _cap_flist_node *next;
} _cap_flist_node;

typedef struct {
	size_t size;
	_cap_flist_node *head;
	pthread_mutex_t _forward_list_mtx;
} cap_forward_list;
#endif // !DOXYGEN_SHOULD_SKIP_THIS

/**
 * These are the concurrent or thread-safe version of the containers. There is
 * no difference in the operation. We use mutex locks during performing
 * operations to make it thread-safe and keep the Forward-List's invariant
 * valid
 */

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
	f_list->head = NULL;
	f_list->size = 0;
	f_list->_forward_list_mtx = PTHREAD_MUTEX_INITIALIZER;
	return f_list;
}

static bool cap_forward_list_push_front(cap_forward_list *f_list, void *data) {
	assert(f_list != NULL && data != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	_cap_flist_node *current_node = f_list->head;
	f_list->head = (_cap_flist_node *)CAP_ALLOCATOR(_cap_flist_node, 1);
	bool return_value;
	if (f_list->head == NULL) {
		return_value = false;
	} else {
		f_list->head->data = (CAP_GENERIC_TYPE_PTR)data;
		f_list->head->next = current_node;
		f_list->size++;
		return_value = true;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void *cap_forward_list_pop_front(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	void *return_value;
	if (f_list->head == NULL) {
		return_value = NULL;
	} else {
		return_value = f_list->head->data;
		f_list->head = f_list->head->next;
		f_list->size--;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void *cap_forward_list_front(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	void *return_value;
	if (f_list->head == NULL) {
		return_value = NULL;
	} else {
		return_value = f_list->head->data;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void cap_forward_list_swap(cap_forward_list *f_list_one,
				  cap_forward_list *f_list_two) {
	assert(f_list_one != NULL && f_list_two != NULL);
	int ret = pthread_mutex_lock(&f_list_one->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	int ret2 = pthread_mutex_lock(&f_list_two->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret2);
	_cap_flist_node *one_temp_head = f_list_one->head;
	size_t one_temp_size = f_list_one->size;
	f_list_one->head = f_list_two->head;
	f_list_one->size = f_list_two->size;
	f_list_two->head = one_temp_head;
	f_list_two->size = one_temp_size;
	ret = pthread_mutex_unlock(&f_list_one->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	ret2 = pthread_mutex_unlock(&f_list_two->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret2);
}

static void cap_forward_list_free(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	_cap_flist_node *current_node = f_list->head;
	while (current_node != NULL) {
		_cap_flist_node *next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
}

static size_t cap_forward_list_size(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	size_t return_value = f_list->size;
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void cap_forward_list_deep_free(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	_cap_flist_node *current_node = f_list->head;
	while (current_node != NULL) {
		_cap_flist_node *next_node = current_node->next;
		free(current_node);
		free(current_node->data);
		current_node = next_node;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
}

static bool cap_forward_list_empty(cap_forward_list *f_list) {
	assert(f_list != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	bool return_value = (f_list->size == 0);
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static void *cap_forward_list_find_if(cap_forward_list *f_list,
				      bool (*predicate_fn)(void *)) {
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	assert(f_list != NULL && predicate_fn != NULL);
	_cap_flist_node *current_node = f_list->head;
	void *return_value = NULL;
	while (current_node != NULL) {
		if (current_node->data != NULL &&
		    predicate_fn(current_node->data)) {
			return_value = current_node->data;
			break;
		}
		current_node = current_node->next;
	}
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

static bool cap_forward_list_remove_if(cap_forward_list *f_list,
				       bool (*predicate_fn)(void *)) {
	assert(f_list != NULL && predicate_fn != NULL);
	int ret = pthread_mutex_lock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_LOCK_STATUS(ret);
	_cap_flist_node *current_node = f_list->head;
	_cap_flist_node *prev_node = NULL;
	size_t num_removed = 0;
	while (current_node != NULL) {
		if (current_node->data != NULL &&
		    predicate_fn(current_node->data)) {
			if (prev_node == NULL && current_node->next == NULL) {
				f_list->head = NULL;
				f_list->size = 0;
				++num_removed;
				break;
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
			++num_removed;
		} else {
			prev_node = current_node;
			current_node = current_node->next;
		}
	}
	bool return_value = (num_removed != 0);
	ret = pthread_mutex_unlock(&f_list->_forward_list_mtx);
	CAP_PTHREAD_MUTEX_UNLOCK_STATUS(ret);
	return return_value;
}

// static bool cap_forward_list_remove_if(cap_forward_list* f_list, bool
// (*predicate_fn)(void*)){ 	cap_forward_list* tmp_iter_list =
// (cap_forward_list*) CAP_ALLOCATOR(cap_forward_list, 1);
// tmp_iter_list->head = f_list->head; 	tmp_iter_list->size = f_list->size;
// f_list->head = NULL; 	f_list->size = 0; 	_cap_flist_node*
// current_node = tmp_iter_list->head; 	size_t num_removed = 0;
// while(current_node != NULL){ 		if(current_node->data != NULL &&
// 			predicate_fn(current_node->data)){
// 			void* delete_node = current_node;
// 			current_node = current_node->next;
// 			free(delete_node);
// 			num_removed += 1;
// 		}else{
// 			cap_forward_list_push_front(f_list, current_node->data);
// 			current_node = current_node->next;
// 		}
// 	}
// 	return (num_removed != 0);
// }

#endif // !CAP_CONCURRENT_FORWARD_LIST_H
