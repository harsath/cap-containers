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
#include <cstdlib>
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

bool _cap_map_is_seeded = false;

static cap_map *cap_map_init(size_t key_size,
			     int (*compare_fn)(void *, void *));
static int cap_map_insert(cap_map *map, void *key, void *value);
static void *cap_map_find(cap_map *map, void *key);
static bool cap_map_contains(cap_map *map, void *key);
static int (*cap_map_compare_fn(cap_map *map))(void *, void *);
static size_t cap_map_size(cap_map *map);
static int cap_map_remove(cap_map *map, void *key);
static int cap_map_height(cap_map *map);
static void cap_map_free(cap_map *map);
static void cap_map_deep_free(cap_map *map);
static bool cap_map_empty(cap_map *map);

static int _cap_map_get_rand_level(int max_number);
static void _cap_map_free_node(cap_map *map);
static void _cap_map_deep_free_node(cap_map *map);

static cap_map *cap_map_init(size_t key_size,
			     int (*compare_fn)(void *, void *)) {
	assert(compare_fn != NULL);
	if (!_cap_map_is_seeded) srand(time(NULL));
	cap_map *map = (cap_map *)CAP_ALLOCATOR(cap_map, 1);
	if (!map) return NULL;
	map->_compare_fn = compare_fn;
	map->_height = CAP_MAP_MAX_SKIPLIST_SIZE;
	map->_size = 0;
	map->_key_size = key_size;
	return map;
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
	if (!new_node) return -1;
	new_node->_value = value;
	new_node->_key =
	    (unsigned char *)CAP_ALLOCATOR(unsigned char, map->_key_size);
	if (!new_node->_key) return -1;
	memcpy(new_node->_key, key, map->_key_size);
	new_node->_height = _cap_map_get_rand_level(map->_height);
	new_node->_key_size = map->_key_size;
	++map->_size;
	for (int i = map->_height - 1; i > new_node->_height; --i)
		new_node->_forward[i] = NULL;
	for (int i = map->_height - 1; i >= 0; --i) {
		new_node->_forward[i] = previous[i]->_forward[i];
		previous[i]->_forward[i] = new_node;
	}
	return 0;
}

static void *cap_map_find(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	cap_map *current_node;
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

static int cap_map_height(cap_map *map) {
	assert(map != NULL);
	return map->_height;
}

static bool cap_map_contains(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	return cap_map_find(map, key) != NULL;
}

static int (*cap_map_compare_fn(cap_map *map))(void *, void *) {
	assert(map != NULL);
	return map->_compare_fn;
}

static size_t cap_map_size(cap_map *map) {
	assert(map != NULL);
	return map->_size;
}

static bool cap_map_empty(cap_map *map) {
	assert(map != NULL);
	return map->_size == 0;
}

static int cap_map_remove(cap_map *map, void *key) {
	assert(map != NULL && key != NULL);
	cap_map *current_node = map;
	int current_level = map->_height - 1;
	cap_map *previous[CAP_MAP_MAX_SKIPLIST_SIZE];
	int cmp;
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
		cap_map *node_to_remove = current_node->_forward[0];
		for (int i = node_to_remove->_height; i >= 0; --i)
			previous[i]->_forward[i] = node_to_remove->_forward[i];
		_cap_map_free_node(node_to_remove);
		map->_size--;
		return 0;
	}
	return -1;
}

static void cap_map_free(cap_map *map) {
	assert(map != NULL);
	cap_map *free_me = map;
	map = map->_forward[0];
	for (int i = map->_height - 1; i >= 0; --i) {
		_cap_map_free_node(free_me);
		free_me = map;
		map = map->_forward[0];
	}
}

static void cap_map_deep_free(cap_map *map) {
	assert(map != NULL);
	cap_map *free_me = map;
	map = map->_forward[0];
	for (int i = map->_height - 1; i >= 0; --i) {
		_cap_map_deep_free_node(free_me);
		free_me = map;
		map = map->_forward[0];
	}
}

static int _cap_map_get_rand_level(int max_number) {
	int returner = 1;
	while (returner < max_number && (rand() > RAND_MAX / 2)) returner += 1;
	return returner;
}

static void _cap_map_free_node(cap_map *map) {
	if (map)
		if (map->_key) free(map->_key);
}

static void _cap_map_deep_free_node(cap_map *map) {
	if (map) {
		if (map->_key) free(map->_key);
		if (map->_value) free(map->_value);
		free(map);
	}
}

#endif // !CAP_MAP_H
