// cap-containers for pure C
// Copyright © 2024 Harsath <harsath@fastmail.com>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define CAP_GENERIC_TYPE int
#define CAP_HASH_SIZE 10000
#define CAP_GENERIC_TYPE_PTR CAP_GENERIC_TYPE *
#define CAP_ALLOCATOR(type, number_of_elements) malloc(sizeof(type) * number_of_elements)

typedef struct _cap_lru_cache_node {
    CAP_GENERIC_TYPE_PTR key;
    CAP_GENERIC_TYPE_PTR value;
    struct _cap_lru_cache_node *prev;
    struct _cap_lru_cache_node *next;
} _cap_lru_cache_node;

typedef struct {
    int capacity;
    int size;
    _cap_lru_cache_node **table;
    _cap_lru_cache_node *head;
    _cap_lru_cache_node *tail;
} cap_lru_cache;
#endif // DOXYGEN_SHOULD_SKIP_THIS

/**
 * Initilize a cap_lru_cache
 * 
 * @param capacity Cache's capacity
 * @return Allocated cap_lru_cache container
 */
static cap_lru_cache *cap_lru_cache_init(int capacity);
/**
 * Deallocate cap_lru_cache container
 * 
 * @param cache cap_lru_cache pointer
 */
static void cap_lru_cache_free(cap_lru_cache *cache);
/**
 * Lookup a cache with key. Moves the lookuped value to front of cache.
 *
 * @param cache cap_lru_cache container
 * @param key key to lookup
 * @return Pointer to value if found. NULL otherwise.
 *
 */
static CAP_GENERIC_TYPE_PTR cap_lru_cache_get(cap_lru_cache *cache, CAP_GENERIC_TYPE_PTR key);
/**
 * Insert it into cache at front.
 *
 * @param cache cap_lru_cache container
 * @param key
 * @param value
 */
static void cap_lru_cache_put(cap_lru_cache *cache, CAP_GENERIC_TYPE_PTR key, CAP_GENERIC_TYPE_PTR value);

static int hash(CAP_GENERIC_TYPE_PTR key) {
    return (*key) % CAP_HASH_SIZE;
}

static _cap_lru_cache_node *create_node(CAP_GENERIC_TYPE_PTR key, CAP_GENERIC_TYPE_PTR value) {
    _cap_lru_cache_node *node = (_cap_lru_cache_node *)CAP_ALLOCATOR(_cap_lru_cache_node, 1);
    node->key = key;
    node->value = value;
    node->prev = node->next = NULL;
    return node;
}

static void move_to_head(cap_lru_cache *cache, _cap_lru_cache_node *node) {
    if (node == cache->head) return;
    
    if (node->prev) node->prev->next = node->next;
    if (node->next) node->next->prev = node->prev;
    if (node == cache->tail) cache->tail = node->prev;

    node->next = cache->head;
    node->prev = NULL;

    if (cache->head) cache->head->prev = node;
    cache->head = node;

    if (!cache->tail) cache->tail = node;
}

static void remove_tail(cap_lru_cache *cache) {
    if (!cache->tail) return;

    _cap_lru_cache_node *node = cache->tail;
    if (node->prev) node->prev->next = NULL;
    cache->tail = node->prev;

    if (!cache->tail) cache->head = NULL;

    cache->table[hash(node->key)] = NULL;
    free(node);
    cache->size--;
}

cap_lru_cache *cap_lru_cache_init(int capacity) {
    cap_lru_cache *cache = (cap_lru_cache *)CAP_ALLOCATOR(cap_lru_cache, 1);
    cache->capacity = capacity;
    cache->size = 0;
    cache->table = (_cap_lru_cache_node **)calloc(CAP_HASH_SIZE, sizeof(_cap_lru_cache_node *));
    cache->head = cache->tail = NULL;
    return cache;
}

void cap_lru_cache_free(cap_lru_cache *cache) {
    _cap_lru_cache_node *current = cache->head;
    while (current) {
        _cap_lru_cache_node *temp = current;
        current = current->next;
        free(temp);
    }
    free(cache->table);
    free(cache);
}

CAP_GENERIC_TYPE_PTR cap_lru_cache_get(cap_lru_cache *cache, CAP_GENERIC_TYPE_PTR key) {
    int index = hash(key);
    _cap_lru_cache_node *node = cache->table[index];
    
    if (node && *node->key == *key) {
        move_to_head(cache, node);
        return node->value;
    }
    
    return NULL;
}

void cap_lru_cache_put(cap_lru_cache *cache, CAP_GENERIC_TYPE_PTR key, CAP_GENERIC_TYPE_PTR value) {
    int index = hash(key);
    _cap_lru_cache_node *node = cache->table[index];

    if (node && *node->key == *key) {
        node->value = value;
        move_to_head(cache, node);
    } else {
        if (cache->size == cache->capacity) {
            remove_tail(cache);
        }

        _cap_lru_cache_node *new_node = create_node(key, value);
        new_node->next = cache->head;
        if (cache->head) cache->head->prev = new_node;
        cache->head = new_node;
        if (!cache->tail) cache->tail = new_node;

        cache->table[index] = new_node;
        cache->size++;
    }
}

#endif
