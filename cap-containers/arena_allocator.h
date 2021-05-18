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
#ifndef CAP_ARENA_ALLOCATOR
#define CAP_ARENA_ALLOCATOR
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	unsigned char *_mem_ptr;
	size_t _total_arena_size;
	size_t _current_arena_size;
} cap_arena_allocator;

/**
 * Create a new Arena allocator of the given size.
 *
 * @param init_size Size of the allocator
 * @return New arena allocator object
 */
static cap_arena_allocator *cap_arena_allocator_init(size_t init_size);
/**
 * Return a new pointer which can hold the given size capacity within the arena
 * allocator
 *
 * If the given size is larger the one given on the space available on the
 * allocator, it returns NULL
 *
 * @param arena Arena allocator object
 * @param size Size of the memory to be allocated from the arena allocator
 * @return Pointer to the start of the buffer
 */
static void *cap_arena_alloc(cap_arena_allocator *arena, size_t size);
/**
 * Reset the arena allocater to point the the starting memory region. One can
 * just overwrite the existing or previous memory/contents
 *
 * @param arena Arena allocator object
 */
static void cap_arena_reset(cap_arena_allocator *arena);
/**
 * Free the arena allocator object along with it's underlying memory
 *
 * @param arena Arena allocator object
 */
static void cap_arena_free(cap_arena_allocator *arena);
/**
 * Get the current size of the arena allocator
 *
 * @param arena Arena allocator object
 * @return Current size consumed by the allocator
 */
static size_t cap_arena_size(cap_arena_allocator *arena);
/**
 * Get the remaining space available on the allocator
 *
 * @param arena Arena allocator object
 * @return Remaining space left before the allocator runs out of memory
 */
static size_t cap_arena_remaining_size(cap_arena_allocator *arena);
/**
 * Get the capacity of the underlying arena allocator
 *
 * @param arena Arena allocator object
 * @return Capacity of the arena allocator object
 */
static size_t cap_arena_capacity(cap_arena_allocator *arena);

static cap_arena_allocator *cap_arena_allocator_init(size_t init_size) {
	cap_arena_allocator *arena =
	    (cap_arena_allocator *)malloc(sizeof(cap_arena_allocator));
	if (!arena) {
		fprintf(stderr, "memory allocation failur\n");
		return NULL;
	}
	arena->_mem_ptr = (unsigned char *)malloc(init_size);
	if (!arena->_mem_ptr) {
		fprintf(stderr, "memory allocation failur\n");
		free(arena);
		return NULL;
	}
	arena->_total_arena_size = init_size;
	arena->_current_arena_size = 0;
	return arena;
}

static void *cap_arena_alloc(cap_arena_allocator *arena, size_t size) {
	assert(arena != NULL);
	if ((arena->_current_arena_size + size) > arena->_total_arena_size)
		return NULL;
	arena->_current_arena_size += size;
	return (arena->_mem_ptr + arena->_current_arena_size);
}

static void cap_arena_reset(cap_arena_allocator *arena) {
	assert(arena != NULL);
	arena->_current_arena_size = 0;
}

static void cap_arena_free(cap_arena_allocator *arena) {
	assert(arena != NULL);
	if (arena->_mem_ptr) free(arena->_mem_ptr);
	free(arena);
}

static size_t cap_arena_size(cap_arena_allocator *arena) {
	assert(arena != NULL);
	return arena->_current_arena_size;
}

static size_t cap_arena_remaining_size(cap_arena_allocator *arena) {
	assert(arena != NULL);
	return (arena->_total_arena_size - arena->_current_arena_size);
}

static size_t cap_arena_capacity(cap_arena_allocator *arena) {
	assert(arena != NULL);
	return arena->_total_arena_size;
}

#endif // !CAP_ARENA_ALLOCATOR
