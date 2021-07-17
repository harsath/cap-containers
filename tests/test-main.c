#include "internal/test-helper.h"
extern void test_dynamic_queue(void);
extern void test_fixed_queue(void);
extern void test_list(void);
extern void test_vector(void);
extern void test_stack(void);
extern void test_forward_list(void);
extern void test_hash_table_separate_chain(void);
extern void test_map(void);
extern void test_circular_queue(void);
extern void test_arena_allocator(void);
extern void test_priority_queue(void);

int main(int argc, const char *const argv[]) {
	test_dynamic_queue();
	test_fixed_queue();
	test_list();
	test_vector();
	test_stack();
	test_forward_list();
	test_hash_table_separate_chain();
	test_map();
	test_circular_queue();
	test_arena_allocator();
	test_priority_queue();

	return 0;
}
