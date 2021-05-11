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
#ifndef CAP_MAP_H
#define CAP_MAP_H
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
	calloc(number_of_elements, sizeof(type))
#define CAP_MAP_MAX_SKIPLIST_SIZE 10

typedef struct cap_map {
	CAP_GENERIC_TYPE_PTR _key;
	size_t _key_size;
	void *_value;
	int _height;
	size_t _size;
	int (*_compare_fn)(void *key_one, void *key_two);
	struct cap_map *_forward[CAP_MAP_MAX_SKIPLIST_SIZE];
} cap_map;

typedef struct {
	CAP_GENERIC_TYPE_PTR key;
	void *value;
	size_t _current_index;
	cap_map *_original_reference;
	cap_map *_current_element;
} cap_map_iterator;

bool _cap_map_is_seeded = false;
#endif // !DOXYGEN_SHOULD_SKIP_THIS

/**
 * Initilize a cap_map container object
 *
 * @return Newly allocated pointer to the head
 */
static cap_map *cap_map_init(size_t key_size,
			     int (*compare_fn)(void *, void *));
/**
 * Get the current size of the container i.e number of elements
 *
 * @param map cap_map container
 * @return size of the container.
 */
static size_t cap_map_size(cap_map *map);
/**
 * Remove an element which is mapped to the given key from the container
 *
 * @param map cap_map container
 * @return 0 if the key is found and the element is removed successfully.
 * Returns -1 if the element is not found.
 */
static int cap_map_remove(cap_map *map, void *key);
/**
 * Get the max height of the cap_map
 *
 * The returned value is the max height of the skip list, which the cap_map
 * internally uses as the underlying implementation
 *
 * @param map cap_map container
 * @return Max height of the container's implementation
 */
static int cap_map_height(cap_map *map);
/**
 * Initialize an Iterator object for iterating over a cap_map container
 *
 * @param cap_map container for which we need to create a iterator
 * @return Allocated cap_map_iterator object for iterating the container
 * which is given in the parameter
 */
static cap_map_iterator *cap_map_iterator_init(cap_map *map);
/**
 * Insert a key-value pair onto the cap_map container.
 *
 * The container doesn't manage the life time of the given pointers, neither the
 * Key's pointer nor the Value's one. Use must explicitly free() the pointer
 * when they don't need it. They must remove the pointer before calling free()
 * on it, if not done so, the container will hold a pointer which is already
 * freed
 *
 * @param map cap_map container
 * @param key Pointer to the key
 * @param value Pointer to the value
 * @return Returns 0 if the operation is success, or else returns -1 is there
 * was a memory allocation error
 */
static int cap_map_insert(cap_map *map, void *key, void *value);
/**
 * Find an element on the cap_map container
 *
 * @param map cap_map container
 * @param key Key for which we need to find the value.
 * @return Returns the pointer to the element, if the element with key is not
 * found, it returns NULL
 */
static void *cap_map_find(cap_map *map, void *key);
/**
 * Check if an item with the given key exists within the cap_map container.
 *
 * @param map cap_map container
 * @param key Key for which we need to see if the elements exists.
 * @return True if the item does exists, or else returns false if not
 */
static bool cap_map_contains(cap_map *map, void *key);
/**
 * Get the function pointer to the handle which the cap_map container uses to
 * compare two keys
 *
 * @param map cap_map container
 * @return Function pointer to the handle which compares two keys. The one given
 * during the initilization of the cap_map
 */
static int (*cap_map_compare_fn(cap_map *map))(void *, void *);
/**
 * Check if an element which is pointed by the iterator matches the given
 * predicate function
 *
 * @param iterator cap_map_iterator iterator object
 * @param predicate_fn Predicate function
 * @return Returns True, if predicate_fn says so, if not returns False.
 */
static bool cap_map_iterator_equals_predicate(cap_map_iterator *iterator,
						 bool (*predicate_fn)(void *));
/**
 * Increment the Iterator to the next element on the container
 *
 * @param iterator cap_map_iterator iterator object
 */
static void cap_map_iterator_increment(cap_map_iterator *iterator);
/**
 * Peek into the next element of the container without incrementing the iterator
 *
 * @param iterator cap_map_iterator iterator object
 * @return Next element of the container, if the iterator is at the last
 * element, it returns NULL
 */
static void *cap_map_iterator_next(cap_map_iterator *iterator);
/**
 * Get an iterator to the start of a cap_map container
 *
 * @param iterator cap_map container
 * @return Allocated cap_map_iterator iterator which points to the first
 * element of the container
 */
static cap_map_iterator *cap_map_begin(cap_map *map);
/**
 * Pop the element at the front of the cap_map container
 *
 * This operation might hurt performance, since this involves moving every item
 * from index 1 to index N
 * @param map cap_map container
 * @return First element, which is poped
 */
static void *cap_map_front(cap_map *map);
/**
 * Free the cap_map_iterator iterator object. This operation doesn't free the
 * underlying element which this iterator points to but only the
 * cap_map_iterator object
 *
 * @param iterator cap_map_iterator iterator object to be freed
 */
static void cap_map_iterator_free(cap_map_iterator *iterator);
/**
 * Free the cap_map container. It doesn't touch the key or value's underlying
 * memory. Only frees the memory which the container owns.
 *
 * @param map cap_map container
 */
static void cap_map_free(cap_map *map);
/**
 * Free the cap_map container and also the underlying memory of the values. When
 * we free the node's memory, we also call free() on the values(assuming that
 * the values are dynamically allocated)
 *
 * @param map cap_map container
 */
static void cap_map_deep_free(cap_map *map);
/**
 * Check if the cap_map is empty or not
 *
 * @param map cap_map container
 * @return True if the container is empty, False if not.
 */
static bool cap_map_empty(cap_map *map);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
static int _cap_map_get_rand_level(int max_number);
static void _cap_map_free_node(cap_map *map);
static void _cap_map_deep_free_node(cap_map *map);
#endif // !DOXYGEN_SHOULD_SKIP_THIS

static cap_map *cap_map_init(size_t key_size,
			     int (*compare_fn)(void *, void *)) {
	assert(compare_fn != NULL);
	if (!_cap_map_is_seeded) srand(time(NULL));
	cap_map *map = (cap_map *)CAP_ALLOCATOR(cap_map, 1);
	if (!map) {
		fprintf(stderr, "memory allocation failur\n");
		return NULL;
	}
	map->_compare_fn = compare_fn;
	map->_height = CAP_MAP_MAX_SKIPLIST_SIZE;
	map->_size = 0;
	map->_key_size = key_size;
	return map;
}

static size_t cap_map_size(cap_map *map) {
	assert(map != NULL);
	return map->_size;
}

static int cap_map_height(cap_map *map) {
	assert(map != NULL);
	return map->_height;
}

static cap_map_iterator *cap_map_iterator_init(cap_map *map) {
	assert(map != NULL);
	cap_map_iterator *iterator =
	    (cap_map_iterator *)CAP_ALLOCATOR(cap_map_iterator, 1);
	if (!iterator) {
		fprintf(stderr, "memory allocation failue\n");
		return NULL;
	}
	iterator->_original_reference = map;
	iterator->_current_element = map->_forward[0];
	iterator->_current_index = 0;
	iterator->key = iterator->_current_element->_key;
	iterator->value = iterator->_current_element->_value;
	return iterator;
}

static int cap_map_insert(cap_map *map, void *key, void *value) {
	assert(map != NULL && key != NULL && value != NULL);
	cap_map *current_node = map;
	int current_level = map->_height - 1;
	cap_map *previous[CAP_MAP_MAX_SKIPLIST_SIZE];
	while (current_level >= 0) {
		previous[current_level] = current_node;
		if (current_node->_forward[current_level] == NULL) {
			current_level--;
		} else {
			int cmp = map->_compare_fn(
			    current_node->_forward[current_level]->_key, key);
			if (cmp == 0) {
				current_node->_forward[current_level]->_value =
				    value;
				return 0;
			} else if (cmp > 0) {
				current_level--;
			} else {
				current_node =
				    current_node->_forward[current_level];
			}
		}
	}
	cap_map *new_node = (cap_map *)CAP_ALLOCATOR(cap_map, 1);
	if (!new_node) {
		fprintf(stderr, "memory allocation failur\n");
		return -1;
	}
	new_node->_value = value;
	new_node->_key =
	    (unsigned char *)CAP_ALLOCATOR(unsigned char, map->_key_size);
	if (!new_node->_key) {
		free(new_node);
		fprintf(stderr, "memory allocation failur\n");
		return -1;
	}
	memcpy(new_node->_key, key, map->_key_size);
	new_node->_height = _cap_map_get_rand_level(map->_height);
	new_node->_key_size = map->_key_size;
	++map->_size;
	for (int i = CAP_MAP_MAX_SKIPLIST_SIZE - 1; i > new_node->_height; --i)
		new_node->_forward[i] = NULL;
	for (int i = new_node->_height - 1; i >= 0; --i) {
		new_node->_forward[i] = previous[i]->_forward[i];
		previous[i]->_forward[i] = new_node;
	}
	return 0;
}

static void *cap_map_find(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	cap_map *current_node = map;
	int current_level = map->_height - 1;
	while (current_level >= 0) {
		if (current_node->_forward[current_level] == NULL) {
			current_level--;
		} else {
			int cmp = map->_compare_fn(
			    current_node->_forward[current_level]->_key, key);
			if (cmp == 0) {
				return current_node->_forward[current_level]
				    ->_value;
			} else if (cmp > 0) {
				current_level--;
			} else {
				current_node =
				    current_node->_forward[current_level];
			}
		}
	}
	return NULL;
}

static bool cap_map_contains(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	return cap_map_find(map, key) != NULL;
}

static int (*cap_map_compare_fn(cap_map *map))(void *, void *) {
	assert(map != NULL);
	return map->_compare_fn;
}
static bool cap_map_iterator_equals_predicate(cap_map_iterator *iter,
					      bool (*predicate_fn)(void *)) {
	assert(iter != NULL && predicate_fn != NULL);
	return predicate_fn(iter->key);
}

static void cap_map_iterator_increment(cap_map_iterator *iterator) {
	assert(iterator != NULL);
	if (iterator->_original_reference->_size ==
	    iterator->_current_index + 1) {
		iterator->key = NULL;
		iterator->value = NULL;
		return;
	}
	iterator->_current_element = iterator->_current_element->_forward[0];
	iterator->key = iterator->_current_element->_key;
	iterator->value = iterator->_current_element->_value;
	++iterator->_current_index;
}
 
static void *cap_map_iterator_next(cap_map_iterator *iterator) {
	assert(iterator != NULL);
	if (iterator->_original_reference->_size ==
	    iterator->_current_index + 1)
		return NULL;
	return iterator->_current_element->_forward[0]->_key;
}

static cap_map_iterator *cap_map_begin(cap_map *map) {
	assert(map != NULL);
	return cap_map_iterator_init(map);
}

static void *cap_map_front(cap_map *map) {
	assert(map != NULL);
	return map->_forward[0];
}

static int cap_map_remove(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	if (!map->_size) return -1;
	cap_map *current_node = map;
	int current_level = map->_height - 1;
	cap_map *previous[CAP_MAP_MAX_SKIPLIST_SIZE];
	int cmp = 1;
	while (current_level >= 0) {
		previous[current_level] = current_node;
		if (current_node->_forward[current_level] == NULL) {
			current_level--;
		} else {
			cmp = map->_compare_fn(
			    current_node->_forward[current_level]->_key, key);
			if (cmp >= 0) {
				current_level--;
			} else {
				current_node =
				    current_node->_forward[current_level];
			}
		}
	}
	if (!cmp) {
		cap_map *free_me = current_node->_forward[0];
		for (int i = CAP_MAP_MAX_SKIPLIST_SIZE - 1; i >= 0; --i)
			previous[i]->_forward[i] = free_me->_forward[i];
		_cap_map_free_node(free_me);
		map->_size--;
		return 0;
	}
	return -1;
}

static void cap_map_free(cap_map *map) {
	assert(map != NULL);
	if (!map->_size) {
		free(map);
		return;
	}
	cap_map *free_me = map;
	map = map->_forward[0];
	for (int i = free_me->_size - 1; i >= 0; --i) {
		_cap_map_free_node(free_me);
		free_me = map;
		map = map->_forward[0];
	}
}

static void cap_map_deep_free(cap_map *map) {
	assert(map != NULL);
	if (!map->_size) {
		free(map);
		return;
	}
	cap_map *free_me = map;
	map = map->_forward[0];
	for (int i = map->_height - 1; i >= 0; --i) {
		_cap_map_deep_free_node(free_me);
		free_me = map;
		map = map->_forward[0];
	}
}

static bool cap_map_empty(cap_map *map) {
	assert(map != NULL);
	return map->_size == 0;
}

static int _cap_map_get_rand_level(int max_number) {
	int returner = 1;
	while (returner < max_number && (rand() > RAND_MAX / 2)) returner += 1;
	return returner;
}

static void _cap_map_free_node(cap_map *map) {
	if (map) {
		free(map->_key);
		free(map);
	}
}

static void _cap_map_deep_free_node(cap_map *map) {
	if (map) {
		if (map->_key) free(map->_key);
		if (map->_value) free(map->_value);
		free(map);
	}
}

static void cap_map_iterator_free(cap_map_iterator *iterator) {
	assert(iterator != NULL);
	free(iterator);
}

#endif // !CAP_MAP_H
