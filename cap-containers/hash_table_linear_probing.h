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
#ifndef CAP_HASHTABLE_LP_H
#define CAP_HASHTABLE_LP_H
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CAP_GENERIC_TYPE unsigned char
#define CAP_HASHTABLE_LP_INIT_SIZE 5
#define CAP_DEFAULT_HASHTABLE_LP_MAX_LOAD_FACTOR 0.50
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements)                                \
	calloc(number_of_elements, sizeof(type))
typedef bool (*_compare_fn_type)(void *key_one, void *key_two);
typedef size_t (*_hash_fn_type)(uint8_t *key, size_t key_size);
typedef struct {
	CAP_GENERIC_TYPE_PTR value;
	CAP_GENERIC_TYPE_PTR key;
} _cap_hash_node;
typedef struct {
	size_t size;
	size_t capacity;
	size_t key_size;
	_compare_fn_type compare_fn;
	_hash_fn_type hash_fn;
	_cap_hash_node *_hash_buckets;
} cap_lp_hash_table;

/**
 * Initilize a cap_lp_hash_table container
 *
 * @param key_size Key-size that'd be used for the container, needed for
 * hash-function
 * @param compare_fn Compare function pointer that'd be called as a callback for
 * comparing two keys. The signacture of the callback is, it take two void* and
 * returns a boolean type, true if both keys matches and false otherwise.
 * @param hash_fn Hash function to be used within the implementation. Pass NULL
 * if you'd like to use the default hash function.
 * @return Newly allocated cap_lp_hash_table container.
 */
static cap_lp_hash_table *cap_lp_hash_table_init(size_t key_size,
						 _compare_fn_type compare_fn,
						 _hash_fn_type hash_fn);
/**
 * Check if a key contains within the cap_lp_hash_table container
 *
 * @param table cap_lp_hash_table container.
 * @param key The key to check.
 * @return True if the key contained within the container, false otherwise.
 */
static bool cap_lp_hash_table_contains(cap_lp_hash_table *table, void *key);
/**
 * Inserts a key and value pair onto the container. The key or value shouldn't
 * be NULL.
 *
 * @param table cap_lp_hash_table container.
 * @param The key to insert into the container.
 * @param The value to insert into the container.
 */
static void cap_lp_hash_table_insert(cap_lp_hash_table *table, void *key,
				     void *value);
/**
 * Lookup a value for a particular key in the container.
 *
 * @param table cap_lp_hash_table container.
 * @param key The key to look for within the container.
 * @return NULL if the key isn't found, or pointer to the value if it's found.
 */
static void *cap_lp_hash_table_lookup(cap_lp_hash_table *table, void *key);
/**
 * Remove a key and value pair from the container.
 *
 * @param table cap_lp_hash_table container.
 * @param key The key to erase.
 * @param True if the key exists and it has been erased, false otherwise.
 */
static bool cap_lp_hash_table_erase(cap_lp_hash_table *table, void *key);
/**
 * Remove the key and value pair from the container and free() the key and value
 * pair, assuming the key is dynamically allocated.
 *
 * @param table cap_lp_hash_table container.
 * @param key The key to deep erase.
 * @return True if the key exists and it has been erased, false otherwise.
 */
static bool cap_lp_hash_table_deep_erase(cap_lp_hash_table *table, void *key);
/**
 * Check if the container is empty.
 *
 * @param table cap_lp_hash_table container.
 * @return True if it's empty, false otherwise.
 */
static bool cap_lp_hash_table_empty(cap_lp_hash_table *table);
/**
 * Query the size of the cap_lp_hash_table container.
 *
 * @param table cap_lp_hash_table container.
 * @return Size of the underlying container.
 */
static size_t cap_lp_hash_table_size(cap_lp_hash_table *table);
/**
 * Free the cap_lp_hash_table container
 *
 * @param table cap_lp_hash_table container.
 */
static void cap_lp_hash_table_free(cap_lp_hash_table *table);
/**
 * Deep free the container's elements. This operation will calls free() on the
 * container's key and values
 *
 * @param table cap_lp_hash_table container.
 */
static void cap_lp_hash_table_deep_free(cap_lp_hash_table *table);
static size_t _hash_fn_defaut_hash_lp(uint8_t *key, size_t key_size);
static void _cap_lp_hash_table_rehash(cap_lp_hash_table *table);

static cap_lp_hash_table *cap_lp_hash_table_init(size_t key_size,
						 _compare_fn_type compare_fn,
						 _hash_fn_type hash_fn) {
	assert(compare_fn != NULL);
	cap_lp_hash_table *table =
	    (cap_lp_hash_table *)CAP_ALLOCATOR(cap_lp_hash_table, 1);
	if (!table) {
		fprintf(stderr, "memory allocation failure\n");
		return NULL;
	}
	table->size = 0;
	table->capacity = CAP_HASHTABLE_LP_INIT_SIZE;
	table->key_size = key_size;
	table->compare_fn = compare_fn;
	if (hash_fn)
		table->hash_fn = hash_fn;
	else
		table->hash_fn = _hash_fn_defaut_hash_lp;
	table->_hash_buckets =
	    (_cap_hash_node *)CAP_ALLOCATOR(_cap_hash_node, table->capacity);
	if (!table->_hash_buckets) {
		fprintf(stderr, "memory allocation failure\n");
		free(table);
		return NULL;
	}
	return table;
}

static bool cap_lp_hash_table_erase(cap_lp_hash_table *table, void *key) {
	assert(table != NULL && key != NULL);
	size_t index =
	    table->hash_fn((uint8_t *)key, table->key_size) % table->capacity;
	bool found_item = false;
	while (table->_hash_buckets[index].value != NULL) {
		if (table->compare_fn(table->_hash_buckets[index].key, key)) {
			found_item = true;
			break;
		}
	}
	if (found_item) {
		table->_hash_buckets[index].value = NULL;
		table->_hash_buckets[index].key = NULL;
		table->size--;
		index = (index + 1) % table->capacity;
		while (table->_hash_buckets[index].value != NULL) {
			void *value = table->_hash_buckets[index].value;
			void *key = table->_hash_buckets[index].key;
			table->_hash_buckets[index].key = NULL;
			table->_hash_buckets[index].value = NULL;
			table->size--;
			cap_lp_hash_table_insert(table, key, value);
			index = (index + 1) % table->capacity;
		}
	}
	return found_item;
}

static bool cap_lp_hash_table_deep_erase(cap_lp_hash_table *table, void *key) {
	assert(table != NULL && key != NULL);
	size_t index =
	    table->hash_fn((uint8_t *)key, table->key_size) % table->capacity;
	bool found_item = false;
	while (table->_hash_buckets[index].value != NULL) {
		if (table->compare_fn(table->_hash_buckets[index].key, key)) {
			found_item = true;
			break;
		}
	}
	if (found_item) {
		void *del_value = table->_hash_buckets[index].value;
		void *del_key = table->_hash_buckets[index].key;
		free(del_value);
		free(del_key);
		table->_hash_buckets[index].value = NULL;
		table->_hash_buckets[index].key = NULL;
		table->size--;
		index = (index + 1) % table->capacity;
		while (table->_hash_buckets[index].value != NULL) {
			void *value = table->_hash_buckets[index].value;
			void *key = table->_hash_buckets[index].key;
			table->_hash_buckets[index].key = NULL;
			table->_hash_buckets[index].value = NULL;
			table->size--;
			cap_lp_hash_table_insert(table, key, value);
			index = (index + 1) % table->capacity;
		}
	}
	return found_item;
}

static void cap_lp_hash_table_insert(cap_lp_hash_table *table, void *key,
				     void *value) {
	assert(table != NULL && key != NULL && value != NULL);
	if (table->size > 0 && (((float)table->size / (float)table->capacity) >
				CAP_DEFAULT_HASHTABLE_LP_MAX_LOAD_FACTOR)) {
		_cap_lp_hash_table_rehash(table);
	}
	size_t index =
	    table->hash_fn((uint8_t *)key, table->key_size) % table->capacity;
	while (table->_hash_buckets[index].value != NULL) {
		index = (index + 1) % table->capacity;
	}
	table->_hash_buckets[index].value = (CAP_GENERIC_TYPE_PTR)value;
	table->_hash_buckets[index].key = (CAP_GENERIC_TYPE_PTR)key;
	table->size++;
}

static void _cap_lp_hash_table_rehash(cap_lp_hash_table *table) {
	_cap_hash_node *old_hash_node = table->_hash_buckets;
	size_t old_capacity = table->capacity;
	table->_hash_buckets = (_cap_hash_node *)CAP_ALLOCATOR(
	    _cap_hash_node, table->capacity * 2);
	if (!table->_hash_buckets) {
		fprintf(stderr, "memory allocation falure on rehash\n");
		assert(false);
	}
	table->capacity *= 2;
	table->size = 0;
	for (size_t i = 0; i < old_capacity; ++i) {
		if (old_hash_node[i].value == NULL) continue;
		cap_lp_hash_table_insert(table, old_hash_node[i].key,
					 old_hash_node[i].value);
	}
}

static bool cap_lp_hash_table_contains(cap_lp_hash_table *table, void *key) {
	return (cap_lp_hash_table_lookup(table, key) != NULL);
}

static void *cap_lp_hash_table_lookup(cap_lp_hash_table *table, void *key) {
	assert(table != NULL && key != NULL);
	size_t index =
	    table->hash_fn((uint8_t *)key, table->key_size) % table->capacity;
	void *value = NULL;
	while (table->_hash_buckets[index].key != NULL) {
		if (table->compare_fn(key, table->_hash_buckets[index].key)) {
			value = table->_hash_buckets[index].value;
			break;
		} else {
			index = (index + 1) % table->capacity;
		}
	}
	return value;
}

static bool cap_lp_hash_table_empty(cap_lp_hash_table *table) {
	return (cap_lp_hash_table_size(table) == 0);
}

static size_t cap_lp_hash_table_size(cap_lp_hash_table *table) {
	assert(table != NULL);
	return table->size;
}

static void cap_lp_hash_table_free(cap_lp_hash_table *table) {
	if (table) {
		free(table->_hash_buckets);
		free(table);
	}
}

static void cap_lp_hash_table_deep_free(cap_lp_hash_table *table) {
	if (table) {
		for (size_t i = 0; i < table->capacity; ++i) {
			if (table->_hash_buckets[i].key &&
			    table->_hash_buckets[i].value) {
				free(table->_hash_buckets[i].key);
				free(table->_hash_buckets[i].value);
			}
		}
	}
}

static size_t _hash_fn_defaut_hash_lp(uint8_t *key, size_t key_size) {
	// Hash type: djb2
	// Reference: http://www.cse.yorku.ca/~oz/hash.html
	size_t hash = 5381;
	for (uint8_t byte = 0; byte < key_size; byte++) {
		hash = ((hash << 5) + hash) ^ key[byte];
	}
	return hash;
}

#endif // !CAP_HASHTABLE_LP_H
