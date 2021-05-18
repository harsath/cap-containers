#include "internal/test-helper.h"
#include <arena_allocator.h>

void test_arena_allocator(void) {
	cap_arena_allocator *allocator = cap_arena_allocator_init(10);
	unsigned char *ptr_of_size_4 = cap_arena_alloc(allocator, 4);
	CAP_ASSERT_EQ(cap_arena_size(allocator), 4,
		      "ARENA_ALLOCATOR current size after first allocation");
	CAP_ASSERT_EQ(cap_arena_remaining_size(allocator), 6,
		      "ARENA_ALLOCATOR remaining space after first allocation");
	CAP_ASSERT_EQ(cap_arena_capacity(allocator), 10,
		      "ARENA_ALLOCATOR current capacity");
	unsigned char *ptr_of_size_4_again = cap_arena_alloc(allocator, 4);
	CAP_ASSERT_EQ(cap_arena_size(allocator), 8,
		      "ARENA_ALLOCATOR current size after first allocation");
	CAP_ASSERT_EQ(cap_arena_remaining_size(allocator), 2,
		      "ARENA_ALLOCATOR remaining space after first allocation");
	CAP_ASSERT_EQ(ptr_of_size_4_again - ptr_of_size_4, 4,
		      "ARENA_ALLOCATOR pointer arithmetic");
	CAP_ASSERT_FALSE(cap_arena_alloc(allocator, 10),
			 "ARENA_ALLOCATOR allocation failur on large buffer");
}
