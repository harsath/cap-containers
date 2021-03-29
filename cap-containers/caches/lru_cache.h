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
#ifndef CAP_LRU_CACHE_H
#define CAP_LRU_CACHE_H
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, num_elements) calloc(num_elements, sizeof(type))

typedef struct _cap_list_node {
	CAP_GENERIC_TYPE_PTR data;
	CAP_GENERIC_TYPE_PTR key;
	struct _cap_list_node *next;
	struct _cap_list_node *prev;
} _cap_list_node;

typedef struct {
	size_t _key_size;
	size_t _size;
	_cap_list_node *_head_node;
	_cap_list_node *_tail_node;
} _cap_list;

typedef struct {
	void *key;
	_cap_list_node *node_ptr;
} _hash_table_node;

// Public APIs:
typedef struct {
	size_t _capacity;
	_cap_list *_d_list;
	_hash_table_node *_node_bucket;
	size_t (*_hash_fn)(uint8_t *, size_t);
} cap_lru_cache;

static cap_lru_cache *cap_lru_cache_init(size_t cache_size, size_t key_size);
static bool cap_lru_cache_contains(cap_lru_cache *cache, void *key);
static void *cap_lru_cache_lookup(cap_lru_cache *cache, void *key);
static bool cap_lru_cache_erase(cap_lru_cache *cache, void *key);
static void cap_lru_cache_insert(cap_lru_cache *cache, void *key, void *value,
				 size_t value_size);
static void *cap_lru_cache_front(cap_lru_cache *cache);
static void *cap_lru_cache_back(cap_lru_cache *cache);
static size_t cap_lru_cache_size(cap_lru_cache *cache);
static size_t cap_lru_cache_capacity(cap_lru_cache *cache);
static void cap_lru_cache_clear(cap_lru_cache *cache);
static void cap_lru_cache_free(cap_lru_cache *cache);

static size_t _hash_fn_default_hash(uint8_t *key, size_t key_size);

// Internal D-Linked list:
static _cap_list *_cap_list_init(size_t key_size);
static void _cap_list_push_front(_cap_list *d_list, void *key, void *data,
				 size_t data_size);
static _cap_list_node *_cap_list_pop_back(_cap_list *d_list);
// Returns the 'key'
static _cap_list_node *_cap_list_front(_cap_list *d_list);
static _cap_list_node *_cap_list_back(_cap_list *d_list);
static void _cap_list_move_front(_cap_list *d_list,
				 _cap_list_node *node_to_move);
static void _cap_list_remove_this(_cap_list *d_list,
				  _cap_list_node *node_to_remove);
static void _cap_list_free_node(_cap_list_node *);
static void _cap_list_free_entries(_cap_list *);
static void _cap_list_free(_cap_list *);
#include <stdio.h>
// clang-format off
static void cap_lru_cache_insert(cap_lru_cache *cache, void *key, void *value,
				 size_t value_size) {
	assert(cache != NULL && key != NULL && value != NULL &&
	       value_size != 0);
	size_t index =
	    cache->_hash_fn((uint8_t *)key, cache->_d_list->_key_size) % cache->_capacity;
	if (cache->_d_list->_size >= cache->_capacity) {
		_cap_list_pop_back(cache->_d_list);
		cache->_node_bucket[index].node_ptr = NULL;
	}
	_cap_list_push_front(cache->_d_list, key, value, value_size);
	cache->_node_bucket[index].node_ptr = _cap_list_front(cache->_d_list);
	memcpy(cache->_node_bucket[index].key, key, cache->_d_list->_key_size);
}
// clang-format on

static void cap_lru_cache_clear(cap_lru_cache *cache) {
	assert(cache != NULL);
	_cap_list_free_entries(cache->_d_list);
	for (size_t i = 0; i < cache->_capacity; i++)
		cache->_node_bucket[i].node_ptr = NULL;
}

static void cap_lru_cache_free(cap_lru_cache *cache) {
	assert(cache != NULL);
	for (size_t i = 0; i < cache->_capacity; ++i) {
		free(cache->_node_bucket[i].key);
	}
	free(cache->_node_bucket);
	_cap_list_free(cache->_d_list);
	free(cache);
}

static void _cap_list_free_entries(_cap_list *list) {
	assert(list != NULL);
	_cap_list_node *current_node = list->_head_node->next;
	do {
		if (current_node->data != NULL && current_node->key != NULL) {
			free(current_node->data);
			free(current_node->key);
		}
		current_node = current_node->next;
		--list->_size;
	} while (current_node != list->_tail_node && current_node != NULL);
}

static void _cap_list_free(_cap_list *list) {
	assert(list != NULL);
	_cap_list_node *current_node = list->_head_node->next;
	do {
		if (current_node->data != NULL && current_node->key != NULL) {
			free(current_node->data);
			free(current_node->key);
		}
	} while (current_node != list->_tail_node && current_node != NULL);
	free(list->_head_node);
	free(list);
}

static size_t cap_lru_cache_size(cap_lru_cache *cache) {
	assert(cache != NULL);
	return cache->_d_list->_size;
}

static size_t cap_lru_cache_capacity(cap_lru_cache *cache) {
	assert(cache != NULL);
	return cache->_capacity;
}

static void *cap_lru_cache_front(cap_lru_cache *cache) {
	assert(cache != NULL);
	if (cache->_d_list->_head_node->next != NULL)
		return cache->_d_list->_head_node->next->data;
	return NULL;
}

static void *cap_lru_cache_back(cap_lru_cache *cache) {
	assert(cache != NULL);
	if (cache->_d_list->_tail_node->prev != NULL)
		return cache->_d_list->_tail_node->prev->data;
	return NULL;
}

// clang-format off
static bool cap_lru_cache_erase(cap_lru_cache *cache, void *key){
	assert(cache != NULL && key != NULL);
	size_t index = cache->_hash_fn((uint8_t*)key, cache->_d_list->_key_size) % cache->_capacity;
	if(memcmp(cache->_node_bucket[index].key, key, cache->_d_list->_key_size) != 0 &&
			cache->_node_bucket[index].node_ptr == NULL) return false;
	_cap_list_remove_this(cache->_d_list, cache->_node_bucket[index].node_ptr);
	cache->_node_bucket[index].node_ptr = NULL;
	return true;
}
// clang-format on

static cap_lru_cache *cap_lru_cache_init(size_t cache_size, size_t key_size) {
	assert(cache_size != 0 && key_size != 0);
	cap_lru_cache *cache = (cap_lru_cache *)CAP_ALLOCATOR(cap_lru_cache, 1);
	if (cache == NULL) assert(false);
	cache->_capacity = cache_size;
	cache->_d_list = _cap_list_init(key_size);
	cache->_node_bucket =
	    (_hash_table_node *)CAP_ALLOCATOR(_cap_list_node, cache_size);
	if (cache->_node_bucket == NULL) assert(false);
	for (size_t i = 0; i < cache_size; ++i) {
		cache->_node_bucket[i].node_ptr = NULL;
		cache->_node_bucket[i].key =
		    (CAP_GENERIC_TYPE_PTR)CAP_ALLOCATOR(CAP_GENERIC_TYPE,
							key_size);
	}
	cache->_hash_fn = _hash_fn_default_hash;
	return cache;
}

// clang-format off
static bool cap_lru_cache_contains(cap_lru_cache *cache, void *key){
	assert(cache != NULL && key != NULL);
	size_t index = cache->_hash_fn((uint8_t*)key, cache->_d_list->_key_size) % cache->_capacity;
	if(memcmp(cache->_node_bucket[index].key, key, cache->_d_list->_key_size) == 0 &&
			cache->_node_bucket[index].node_ptr != NULL) return true;
	return false;
}
// clang-format on

// clang-format off
static void *cap_lru_cache_lookup(cap_lru_cache *cache, void *key){
	assert(cache != NULL && key != NULL);
	size_t index = cache->_hash_fn((uint8_t*)key, cache->_d_list->_key_size) % cache->_capacity;
	if(cache->_node_bucket[index].node_ptr == NULL) return NULL;
	_cap_list_move_front(cache->_d_list, cache->_node_bucket[index].node_ptr);
	return cache->_node_bucket[index].node_ptr->data;
}
// clang-format on

static size_t _hash_fn_default_hash(uint8_t *key, size_t key_size) {
	// Hash type: djb2
	// Reference: http://www.cse.yorku.ca/~oz/hash.html
	size_t hash = 5381;
	for (uint8_t byte = 0; byte < key_size; byte++) {
		hash = ((hash << 5) + hash) ^ key[byte];
	}
	return hash;
}

static _cap_list *_cap_list_init(size_t key_size) {
	assert(key_size != 0);
	_cap_list_node *head_and_tail_nodes =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 2);
	if (head_and_tail_nodes == NULL) assert(false);
	_cap_list *d_list = (_cap_list *)CAP_ALLOCATOR(_cap_list, 1);
	if (d_list == NULL) assert(false);
	d_list->_key_size = key_size;
	d_list->_head_node = head_and_tail_nodes;
	d_list->_tail_node = (head_and_tail_nodes + 1);
	d_list->_head_node->data = NULL;
	d_list->_head_node->key = NULL;
	d_list->_head_node->next = d_list->_tail_node;
	d_list->_head_node->prev = NULL;
	d_list->_tail_node->prev = d_list->_head_node;
	d_list->_tail_node->next = NULL;
	d_list->_tail_node->data = NULL;
	d_list->_tail_node->key = NULL;
	d_list->_size = 0;
	return d_list;
}

static void _cap_list_push_front(_cap_list *d_list, void *key, void *data,
				 size_t data_size) {
	assert(d_list != NULL && key != NULL && data != NULL && data_size != 0);
	_cap_list_node *new_node =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 1);
	if (new_node == NULL) assert(false);
	new_node->key = (CAP_GENERIC_TYPE *)CAP_ALLOCATOR(CAP_GENERIC_TYPE,
							  d_list->_key_size);
	if (new_node->key == NULL) assert(false);
	new_node->data =
	    (CAP_GENERIC_TYPE *)CAP_ALLOCATOR(CAP_GENERIC_TYPE, data_size);
	if (new_node->data == NULL) assert(false);
	memcpy(new_node->data, data, data_size);
	memcpy(new_node->key, key, d_list->_key_size);
	_cap_list_node *head_next = d_list->_head_node->next;
	d_list->_head_node->next = new_node;
	new_node->next = head_next;
	head_next->prev = new_node;
	++d_list->_size;
}

static _cap_list_node *_cap_list_pop_back(_cap_list *d_list) {
	assert(d_list != NULL);
	_cap_list_node *pop_node = d_list->_tail_node->prev;
	d_list->_tail_node->prev = pop_node->prev;
	pop_node->prev->next = d_list->_tail_node;
	--d_list->_size;
	return pop_node;
}

static _cap_list_node *_cap_list_front(_cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_head_node->next;
}

static _cap_list_node *_cap_list_back(_cap_list *d_list) {
	assert(d_list != NULL);
	return d_list->_tail_node->prev;
}

static void _cap_list_move_front(_cap_list *d_list,
				 _cap_list_node *node_to_move) {
	assert(d_list != NULL && node_to_move != NULL);
	node_to_move->prev->next = node_to_move->next;
	node_to_move->next->prev = node_to_move->prev;
	_cap_list_node *head_next = d_list->_head_node->next;
	d_list->_head_node->next = node_to_move;
	node_to_move->prev = d_list->_head_node;
	node_to_move->next = head_next;
	head_next->prev = node_to_move;
}
#include <stdio.h>
// [1] <-> [2] <-> [3]
static void _cap_list_remove_this(_cap_list *d_list,
				  _cap_list_node *node_to_remove) {
	assert(d_list != NULL && node_to_remove != NULL);
	if(node_to_remove->prev == NULL){
		printf("NULL NULL!!\n");
	}
	node_to_remove->prev->next = node_to_remove->next;
	node_to_remove->next->prev = node_to_remove->prev;
	--d_list->_size;
	_cap_list_free_node(node_to_remove);
}

static void _cap_list_free_node(_cap_list_node *node_to_free) {
	assert(node_to_free != NULL);
	if (node_to_free->data) free(node_to_free->data);
	if (node_to_free->key) free(node_to_free->key);
	free(node_to_free);
}

#endif // !CAP_LRU_CACHE_H
