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
#ifndef CAP_HASHTABLE_SP_H
#define CAP_HASHTABLE_SP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#define CAP_CHECK_NULL(value) if(value == NULL) return NULL
#define CAP_GENERIC_TYPE unsigned char
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE*
#define CAP_ALLOCATOR(type, number_of_elements) calloc(number_of_elements, sizeof(type))

typedef struct _cap_hash_node {
	CAP_GENERIC_TYPE_PTR data;
	CAP_GENERIC_TYPE_PTR key;
	struct _cap_hash_node* next;
} _cap_hash_node;

typedef struct {
	_cap_hash_node* _head_node;
	size_t _num_items;
} _cap_ll_chain;

typedef struct {
	size_t size;
	size_t capacity;
	size_t key_size;
	bool (*compare_fn)(void*, void*);
} cap_hash_table;

// Prototypes(Public APIs)
// Init:
static inline cap_hash_table* cap_hash_table_init(size_t key_size, size_t init_capacity);

// Lookup & Update:
static inline bool cap_hash_table_contains(cap_hash_table* table, void* key);
static inline bool cap_hash_table_insert(cap_hash_table* table, void* key, void* value);
static inline void* cap_hash_table_lookup(cap_hash_table* table, void* key);
static inline bool cap_hash_table_erase(cap_hash_table*, void* key);
static inline bool cap_hash_table_deep_erase(cap_hash_table*, void* key);
static inline bool cap_hash_table_empty(cap_hash_table*);
static inline void cap_hash_table_swap(cap_hash_table*, cap_hash_table*);
static inline size_t cap_hash_table_bucket_size(cap_hash_table*, size_t);

// Prototypes(Internal helpers)
// Hash table:
static inline _cap_hash_node* _cap_hash_table_allocate_node(void* key, void* value);
static inline bool _cap_hash_table_default_compare();
static inline bool _cap_hash_table_check_for_rehash(cap_hash_table*);
static inline cap_hash_table* _cap_hash_table_rehash(cap_hash_table*);

// Linked list chain:
// Init:
static inline _cap_ll_chain* _cap_ll_chain_init();

// Lookups:
static inline void* _cap_ll_chain_find_if(_cap_ll_chain*, bool (*)(void* key));
static inline bool _cap_ll_chain_push_front(_cap_ll_chain*, void* key, size_t key_size, void* data);
static inline bool _cap_ll_chain_remove_if(_cap_ll_chain*, bool (*)(void* key));
static inline size_t _cap_ll_chain_size(_cap_ll_chain*);

// Memory:
static inline void _cap_ll_chain_free(_cap_ll_chain*);
static inline void _cap_ll_chain_deep_free(_cap_ll_chain*);

static inline _cap_ll_chain* _cap_ll_chain_init(){
	_cap_ll_chain* f_list = (_cap_ll_chain*) CAP_ALLOCATOR(_cap_ll_chain, 1);
	f_list->_head_node = NULL;
	f_list->_num_items = 0;
	return f_list;
}

static inline bool _cap_ll_chain_push_front(_cap_ll_chain* f_list, void* key, size_t key_size, void* data){
	assert(f_list != NULL && key != NULL && data != NULL);
	_cap_hash_node* current_head = f_list->_head_node;
	_cap_hash_node* hash_node = (_cap_hash_node*) CAP_ALLOCATOR(_cap_hash_node, 1);
	if(hash_node == NULL) return false;
	hash_node->data = (CAP_GENERIC_TYPE_PTR)data;
	hash_node->key = (CAP_GENERIC_TYPE_PTR) CAP_ALLOCATOR(CAP_GENERIC_TYPE, key_size);
	memcpy(hash_node->key, key, key_size);
	hash_node->next = current_head;
	return true;
}

__attribute__((always_inline))
static inline void* _cap_ll_chain_find_if(_cap_ll_chain* f_list, bool (*key_predicate)(void* key)){
	assert(f_list != NULL && key_predicate != NULL);
	_cap_hash_node* current_node = f_list->_head_node;
	while(current_node != NULL){
		if(current_node->key != NULL && key_predicate(current_node->key))
			return current_node->data;
		current_node = current_node->next;
	}
	return NULL;
}

__attribute__((always_inline))
static inline bool _cap_ll_chain_remove_if(_cap_ll_chain* f_list, bool (*key_predicate)(void*)){
	assert(f_list != NULL && key_predicate != NULL);
	_cap_hash_node* current_node = f_list->_head_node;
	_cap_hash_node* prev_node = NULL;
	size_t num_removed = 0;
	while(current_node != NULL){
		if(current_node->data != NULL && key_predicate(current_node->data)){
			if(prev_node == NULL && current_node->next == NULL){
				f_list->_head_node = NULL;
				f_list->_num_items = 0;
				return true;
			}else if(prev_node == NULL && current_node->next != NULL){
				f_list->_head_node = current_node->next;				
				f_list->_num_items--;
				free(current_node);
			}else{
				prev_node->next = current_node->next;
				f_list->_num_items--;
				free(current_node);
			}
			num_removed += 1;
		}else{
			prev_node = current_node;
			current_node = current_node->next;
		}
	}
	return (num_removed != 0);
}

__attribute__((always_inline))
static inline size_t _cap_ll_chain_size(_cap_ll_chain* f_list){
	assert(f_list != NULL);
	return f_list->_num_items;
}

static inline void _cap_ll_chain_free(_cap_ll_chain* f_list){
	assert(f_list != NULL);
	_cap_hash_node* current_node = f_list->_head_node;
	while(current_node != NULL){
		_cap_hash_node* next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}
}

static inline void _cap_ll_chain_deep_free(_cap_ll_chain* f_list){
	assert(f_list != NULL);
	_cap_hash_node* current_node = f_list->_head_node;
	while(current_node != NULL){
		_cap_hash_node* next_node = current_node->next;
		free(current_node);
		free(current_node->data);
		current_node = next_node;
	}
}

static inline bool _cap_hash_table_default_compare(cap_hash_table* hash_table, void* key_one, void* key_two){
	assert(hash_table != NULL && key_one != NULL && key_two != NULL);
	return (memcmp(key_one, key_two, hash_table->key_size) == 0);
}

size_t _hash_fn_default_hash(uint8_t* key, size_t key_size){
	// Hash type: djb2
	// Reference: http://www.cse.yorku.ca/~oz/hash.html
	size_t hash = 5381;
	for(uint8_t byte = 0; byte < key_size; byte++){
		hash = ((hash<<5) + hash) ^ key[byte];
	}
	return hash;
}

#endif // !CAP_HASHTABLE_SP_H
