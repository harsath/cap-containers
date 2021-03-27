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
#ifndef CAP_LRU_CACHE_H
#define CAP_LRU_CACHE_H
#include <assert.h>
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

// Internal:
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
static void _cap_list_free_node(_cap_list_node*);

static _cap_list *_cap_list_init(size_t key_size) {
	assert(key_size != 0);
	_cap_list_node *head_and_tail_nodes =
	    (_cap_list_node *)CAP_ALLOCATOR(_cap_list_node, 2);
	_cap_list *d_list = (_cap_list *)CAP_ALLOCATOR(_cap_list, 1);
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
	if (new_node == NULL) assert(0);
	new_node->key = (CAP_GENERIC_TYPE *)CAP_ALLOCATOR(CAP_GENERIC_TYPE,
							  d_list->_key_size);
	new_node->data =
	    (CAP_GENERIC_TYPE *)CAP_ALLOCATOR(CAP_GENERIC_TYPE, data_size);
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

static void _cap_list_remove_this(_cap_list *d_list, _cap_list_node *node_to_remove){
	assert(d_list != NULL && node_to_remove != NULL);
	node_to_remove->prev->next = node_to_remove->next;
	node_to_remove->next->prev = node_to_remove->prev;
	_cap_list_free_node(node_to_remove);
}

static void _cap_list_free_node(_cap_list_node *node_to_free){
	assert(node_to_free != NULL);
	if(node_to_free->data) free(node_to_free->data);
	if(node_to_free->key) free(node_to_free->key);
	free(node_to_free);
}

#endif // !CAP_LRU_CACHE_H
