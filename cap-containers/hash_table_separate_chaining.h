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
#ifndef CAP_HASHTABLE_SP_H
#define CAP_HASHTABLE_SP_H
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_DEFAULT_HASHTABLE_MAX_LOAD_FACTOR 0.80
#define CAP_HASHTABLE_LOAD_FACTOR(hash_table_ptr)                              \
	(hash_table_ptr->size == hash_table_ptr->capacity)
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	calloc(number_of_elements, sizeof(type))

typedef struct _cap_hash_node {
	CAP_GENERIC_TYPE_PTR data;
	CAP_GENERIC_TYPE_PTR key;
	struct _cap_hash_node *next;
} _cap_hash_node;

typedef struct {
	_cap_hash_node *_head_node;
	size_t _num_items;
} _cap_ll_chain;

typedef struct {
	size_t size;
	size_t capacity;
	size_t key_size;
	bool (*compare_fn)(void *key_one, void *key_two, size_t key_size);
	size_t (*hash_fn)(uint8_t *key, size_t key_size);
	_cap_ll_chain *_hash_buckets;
} cap_hash_table;
#endif

// Prototypes(Public APIs)
/**
 * Initilize a cap_hash_table container with specified initial-capacity and
 * key-key
 *
 * @param key_size Key-size for the cap_hash_table container
 * @param init_capacity Initial bucket capacity for the cap_hash_table container
 * @return Allocated cap_hash_table container
 */
static cap_hash_table *cap_hash_table_init(size_t key_size,
					   size_t init_capacity);

// Lookup & Update:
/**
 * Check if an element with the given key contains within cap_hash_table
 * container
 *
 * @param table cap_hash_table container
 * @param key Key to check against
 * @return Returns True if an element is contained within the cap_hash_table
 * with the given key
 */
static bool cap_hash_table_contains(cap_hash_table *table, void *key);
/**
 * Insert an element onto the hash table O(1) operation
 *
 * If the elements already exists in the bucket the key gets hash into, we use
 * separate-chaining for collision resolution i.e we use linked list
 * @param table cap_hash_table container
 * @param key Key for element to be inserted into the hash-table
 * @param value Item to be inserted into the hash table
 */
static void cap_hash_table_insert(cap_hash_table *table, void *key,
				  void *value);
/**
 * Lookup an element in the cap_hash_table container O(1) operation
 *
 * @param table cap_hash_table container
 * @param key Key for the lookup operation
 * @return Returns the element if there is an element on the bucker where the
 * key gets hash into, if there is no elements(bucket is empty), returns NULL
 */
static void *cap_hash_table_lookup(cap_hash_table *table, void *key);
/**
 * Erase/remove an element from the cap_hash_table container which have the give
 * key O(1) operation
 *
 * @param table cap_hash_table container
 * @param key Key for the element to be erased/removed
 * @return Returns True if there is a valid element on the bucket where the key
 * gets hash into and removed the element successfully, if not returns False
 */
static bool cap_hash_table_erase(cap_hash_table *table, void *key);
/**
 * Erase/remove an element and also free() the element O(1) operation as far as
 * the container goes
 *
 * @param table cap_hash_table container
 * @param key Key for the element to be removed from the hash-table and freed.
 * @return Returns True if there is a valid element on the bucket where the key
 * gets hash into and removed(and freed) the element successfully, if not
 * returns False
 */
static bool cap_hash_table_deep_erase(cap_hash_table *table, void *key);
/**
 * Query if the element is empty
 *
 * @param table cap_hash_table container
 * @return Returns True if the cap_hash_table container is empty, or else
 * returns False
 */
static bool cap_hash_table_empty(cap_hash_table *table);
/**
 * Swap two cap_hash_table container object's internal members
 *
 * @param table_one First cap_hash_table container
 * @param table_two Second cap_hash_table container
 */
static void cap_hash_table_swap(cap_hash_table *table_one,
				cap_hash_table *table_two);
/**
 * Query the size/number of buckets the cap_hash_table container contains at
 * present.
 *
 * @param table cap_hash_table container
 * @return Size of the cap_hash_table buckets
 */
static size_t cap_hash_table_bucket_size(cap_hash_table *table);
/**
 * Query the size/number of elements the cap_hash_table container contains at
 * present.
 *
 * @param table cap_hash_table container
 * @return Size of the cap_hash_table container
 */
static size_t cap_hash_table_size(cap_hash_table *table);
/**
 * Frees the cap_hash_table container and doesn't touch the underlying elements
 * which the hash-table contains
 *
 * @param table cap_hash_table container
 */
static void cap_hash_table_free(cap_hash_table *table);
/**
 * Frees the cap_hash_table container and also the underlying elements which the
 * hash-table contains(assyming the elements are dynamically allocated)
 *
 * @param table cap_hash_table container
 */
static void cap_hash_table_deep_free(cap_hash_table *table);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Prototypes(Internal helpers)
// Hash table:
static cap_hash_table *_cap_hash_table_rehash(cap_hash_table *);
static bool _cap_hash_table_default_compare(void *key_one, void *key_two,
					    size_t key_size);
static size_t _hash_fn_default_hash(uint8_t *key, size_t key_size);

// Linked list chain:
// Init:
static _cap_ll_chain *_cap_ll_chain_init();

// Lookups:
static _cap_hash_node *_cap_ll_chain_find_if(_cap_ll_chain *, void *key,
					     size_t key_size);
static bool _cap_ll_chain_push_front(_cap_ll_chain *, void *key,
				     size_t key_size, void *data);
static bool _cap_ll_chain_remove_if(_cap_ll_chain *, void *key, size_t key_size,
				    bool is_deep_free);
static size_t _cap_ll_chain_size(_cap_ll_chain *);

// Memory:
static void _cap_ll_chain_free(_cap_ll_chain *);
static void _cap_ll_chain_deep_free(_cap_ll_chain *);
#endif

static cap_hash_table *
_cap_hash_table_rehash(cap_hash_table *hash_table_original) {
	assert(hash_table_original != NULL);
	cap_hash_table *new_hash_table = cap_hash_table_init(
	    hash_table_original->key_size, hash_table_original->capacity * 2);
	for (size_t i = 0; i < hash_table_original->size; i++) {
		_cap_hash_node *current_node =
		    hash_table_original->_hash_buckets[i]._head_node;
		_cap_hash_node *prev_node = NULL;
		while (current_node != NULL) {
			cap_hash_table_insert(new_hash_table, current_node->key,
					      current_node->data);
			prev_node = current_node;
			current_node = current_node->next;
			free(prev_node);
		}
	}
	return new_hash_table;
}

static size_t cap_hash_table_bucket_size(cap_hash_table *hash_table) {
	assert(hash_table != NULL);
	return hash_table->capacity;
}

static void cap_hash_table_swap(cap_hash_table *hash_table_one,
				cap_hash_table *hash_table_two) {
	assert(hash_table_one != NULL && hash_table_two != NULL);
	_cap_ll_chain *temp_one_hash_bucket = hash_table_one->_hash_buckets;
	size_t temp_one_size = hash_table_one->size;
	size_t temp_one_key_size = hash_table_one->key_size;
	size_t temp_one_capacity = hash_table_one->capacity;
	size_t (*temp_one_hash_fn)(uint8_t *, size_t) = hash_table_one->hash_fn;
	bool (*temp_one_cmp_fn)(void *, void *, size_t) =
	    hash_table_one->compare_fn;
	hash_table_one->_hash_buckets = hash_table_two->_hash_buckets;
	hash_table_one->compare_fn = hash_table_two->compare_fn;
	hash_table_one->hash_fn = hash_table_two->hash_fn;
	hash_table_one->capacity = hash_table_two->capacity;
	hash_table_one->key_size = hash_table_two->key_size;
	hash_table_one->size = hash_table_two->size;
	hash_table_two->_hash_buckets = temp_one_hash_bucket;
	hash_table_two->hash_fn = temp_one_hash_fn;
	hash_table_two->capacity = temp_one_capacity;
	hash_table_two->key_size = temp_one_key_size;
	hash_table_two->compare_fn = temp_one_cmp_fn;
	hash_table_two->size = temp_one_size;
}

static bool cap_hash_table_empty(cap_hash_table *hash_table) {
	assert(hash_table != NULL);
	return (hash_table->size == 0);
}

static size_t cap_hash_table_size(cap_hash_table *hash_table) {
	assert(hash_table != NULL);
	return hash_table->size;
}

static bool cap_hash_table_deep_erase(cap_hash_table *hash_table, void *key) {
	assert(hash_table != NULL && key != NULL);
	size_t key_index =
	    hash_table->hash_fn((uint8_t *)key, hash_table->key_size) %
	    hash_table->capacity;
	bool remove_if_return =
	    _cap_ll_chain_remove_if(&hash_table->_hash_buckets[key_index], key,
				    hash_table->key_size, true);
	if (remove_if_return) hash_table->size--;
	return remove_if_return;
}

static bool cap_hash_table_erase(cap_hash_table *hash_table, void *key) {
	assert(hash_table != NULL && key != NULL);
	size_t key_index =
	    hash_table->hash_fn((uint8_t *)key, hash_table->key_size) %
	    hash_table->capacity;
	bool remove_if_return =
	    _cap_ll_chain_remove_if(&hash_table->_hash_buckets[key_index], key,
				    hash_table->key_size, false);
	if (remove_if_return) hash_table->size--;
	return remove_if_return;
}

static void *cap_hash_table_lookup(cap_hash_table *hash_table, void *key) {
	assert(hash_table != NULL && key != NULL);
	size_t key_index =
	    hash_table->hash_fn((uint8_t *)key, hash_table->key_size) %
	    hash_table->capacity;
	_cap_hash_node *find_if_key = _cap_ll_chain_find_if(
	    &hash_table->_hash_buckets[key_index], key, hash_table->key_size);
	if (find_if_key == NULL) return NULL;
	return find_if_key->data;
}

static void cap_hash_table_insert(cap_hash_table *hash_table, void *key,
				  void *value) {
	assert(hash_table != NULL && key != NULL && value != NULL);
	if (CAP_HASHTABLE_LOAD_FACTOR(hash_table)) {
		cap_hash_table *rehashed_table =
		    _cap_hash_table_rehash(hash_table);
		cap_hash_table_swap(hash_table, rehashed_table);
		free(rehashed_table);
	}
	size_t key_index =
	    hash_table->hash_fn((uint8_t *)key, hash_table->key_size) %
	    hash_table->capacity;
	_cap_hash_node *find_if_key = _cap_ll_chain_find_if(
	    &hash_table->_hash_buckets[key_index], key, hash_table->key_size);
	if (find_if_key != NULL) {
		find_if_key->data = (CAP_GENERIC_TYPE_PTR)value;
	}
	_cap_ll_chain_push_front(&hash_table->_hash_buckets[key_index], key,
				 hash_table->key_size, value);
	if (find_if_key == NULL) hash_table->size++;
}

static bool cap_hash_table_contains(cap_hash_table *hash_table, void *key) {
	assert(hash_table != NULL && key != NULL);
	size_t index =
	    hash_table->hash_fn((uint8_t *)key, hash_table->key_size) %
	    hash_table->capacity;
	void *find_if_return = _cap_ll_chain_find_if(
	    &hash_table->_hash_buckets[index], key, hash_table->key_size);
	if (find_if_return == NULL) return false;
	return true;
}

static void cap_hash_table_free(cap_hash_table *hash_table) {
	for (size_t i = 0; i < hash_table->size; i++) {
		_cap_ll_chain_free(&hash_table->_hash_buckets[i]);
	}
	free(hash_table->_hash_buckets);
	free(hash_table);
}

static void cap_hash_table_deep_free(cap_hash_table *hash_table) {
	for (size_t i = 0; i < hash_table->size; i++) {
		_cap_ll_chain_deep_free(&hash_table->_hash_buckets[i]);
	}
	free(hash_table->_hash_buckets);
	free(hash_table);
}

static cap_hash_table *cap_hash_table_init(size_t key_size,
					   size_t init_capacity) {
	cap_hash_table *hash_table =
	    (cap_hash_table *)CAP_ALLOCATOR(cap_hash_table, 1);
	if (!hash_table) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	hash_table->capacity = init_capacity;
	hash_table->key_size = key_size;
	hash_table->size = 0;
	hash_table->compare_fn = _cap_hash_table_default_compare;
	hash_table->_hash_buckets =
	    (_cap_ll_chain *)CAP_ALLOCATOR(_cap_ll_chain, init_capacity);
	if (!hash_table->_hash_buckets) {
		fprintf(stderr, "memory allocation failure\n");
		free(hash_table);
		return NULL;
	}
	hash_table->hash_fn = _hash_fn_default_hash;
	for (size_t i = 0; i < init_capacity; i++) {
		hash_table->_hash_buckets[i]._head_node = NULL;
		hash_table->_hash_buckets[i]._num_items = 0;
	}
	return hash_table;
}

static _cap_ll_chain *_cap_ll_chain_init() {
	_cap_ll_chain *f_list =
	    (_cap_ll_chain *)CAP_ALLOCATOR(_cap_ll_chain, 1);
	if (!f_list) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	f_list->_head_node = NULL;
	f_list->_num_items = 0;
	return f_list;
}

static bool _cap_ll_chain_push_front(_cap_ll_chain *f_list, void *key,
				     size_t key_size, void *data) {
	assert(f_list != NULL && key != NULL && data != NULL);
	_cap_hash_node *current_head = f_list->_head_node;
	_cap_hash_node *hash_node =
	    (_cap_hash_node *)CAP_ALLOCATOR(_cap_hash_node, 1);
	if (!hash_node) {
		fprintf(stderr, "memory allocation failure\n");
		return false;
	}
	hash_node->data = (CAP_GENERIC_TYPE_PTR)data;
	hash_node->key = (CAP_GENERIC_TYPE_PTR)key;
	hash_node->next = current_head;
	f_list->_head_node = hash_node;
	return true;
}

static _cap_hash_node *_cap_ll_chain_find_if(_cap_ll_chain *f_list, void *key,
					     size_t key_size) {
	assert(f_list != NULL && key != NULL);
	_cap_hash_node *current_node = f_list->_head_node;
	while (current_node != NULL) {
		if (current_node->key != NULL &&
		    (memcmp(current_node->key, key, key_size) == 0))
			return current_node;
		current_node = current_node->next;
	}
	return NULL;
}

static bool _cap_ll_chain_remove_if(_cap_ll_chain *f_list, void *key,
				    size_t key_size, bool deep_free) {
	assert(f_list != NULL && key != NULL);
	_cap_hash_node *current_node = f_list->_head_node;
	_cap_hash_node *prev_node = NULL;
	while (current_node != NULL) {
		if (current_node->data != NULL &&
		    (memcmp(key, current_node->key, key_size) == 0)) {
			if (prev_node == NULL && current_node->next == NULL) {
				f_list->_head_node = NULL;
				f_list->_num_items = 0;
				if (deep_free) {
					free(current_node->data);
					free(current_node->key);
				}
				free(current_node);
				return true;
			} else if (prev_node == NULL &&
				   current_node->next != NULL) {
				f_list->_head_node = current_node->next;
				f_list->_num_items--;
				if (deep_free) {
					free(current_node->data);
					free(current_node->key);
				}
				free(current_node);
				return true;
			} else {
				prev_node->next = current_node->next;
				f_list->_num_items--;
				if (deep_free) {
					free(current_node->data);
					free(current_node->key);
				}
				free(current_node);
				return true;
			}
		} else {
			prev_node = current_node;
			current_node = current_node->next;
		}
	}
	return false;
}

static size_t _cap_ll_chain_size(_cap_ll_chain *f_list) {
	assert(f_list != NULL);
	return f_list->_num_items;
}

static void _cap_ll_chain_free(_cap_ll_chain *f_list) {
	assert(f_list != NULL);
	_cap_hash_node *current_node = f_list->_head_node;
	while (current_node != NULL) {
		_cap_hash_node *next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}

static void _cap_ll_chain_deep_free(_cap_ll_chain *f_list) {
	assert(f_list != NULL);
	_cap_hash_node *current_node = f_list->_head_node;
	while (current_node != NULL) {
		_cap_hash_node *next_node = current_node->next;
		free(current_node->key);
		free(current_node->data);
		free(current_node);
		current_node = next_node;
	}
}

static bool _cap_hash_table_default_compare(void *key_one, void *key_two,
					    size_t key_size) {
	assert(key_one != NULL && key_two != NULL);
	return (memcmp(key_one, key_two, key_size) == 0);
}

static size_t _hash_fn_default_hash(uint8_t *key, size_t key_size) {
	// Hash type: djb2
	// Reference: http://www.cse.yorku.ca/~oz/hash.html
	size_t hash = 5381;
	for (uint8_t byte = 0; byte < key_size; byte++) {
		hash = ((hash << 5) + hash) ^ key[byte];
	}
	return hash;
}

#endif // !CAP_HASHTABLE_SP_H
