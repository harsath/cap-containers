#include <stack.h>
#include "internal/test-helper.h"

void test_stack(void){
	{
		cap_stack* stack_one = cap_stack_init();
		int one = 10;
		int two = 20;
		int three = 30;
		// [10] [20] [30]
		CAP_ASSERT_TRUE(cap_stack_push(stack_one, &one), "STACK push one");
		CAP_ASSERT_TRUE(cap_stack_push(stack_one, &two) && cap_stack_push(stack_one, &three), "STACK push two & three");
		CAP_ASSERT_TRUE(*(int*)cap_stack_top(stack_one) == three &&
				cap_stack_size(stack_one) == 3, "STACK top & size");
		CAP_ASSERT_TRUE(*(int*)cap_stack_pop(stack_one) == three &&
				*(int*)cap_stack_top(stack_one) == two &&
				cap_stack_size(stack_one) == 2, "STACK pop & top & size");
		cap_stack_pop(stack_one);
		CAP_ASSERT_TRUE(*(int*)cap_stack_pop(stack_one) == one &&
				cap_stack_pop(stack_one) == NULL && cap_stack_pop(stack_one) == NULL &&
				cap_stack_size(stack_one) == 0, "STACK 3xpop & size");
		// [30]
		CAP_ASSERT_TRUE(cap_stack_empty(stack_one) &&
				cap_stack_push(stack_one, &three) &&
				!cap_stack_empty(stack_one), "STACK empty & push");

		cap_stack* stack_two = cap_stack_init();
		// [10] [30] [20]
		cap_stack_push(stack_two, &one);
		cap_stack_push(stack_two, &three);
		cap_stack_push(stack_two, &two);
		CAP_ASSERT_TRUE(cap_stack_size(stack_two) == 3 &&
				*(int*)cap_stack_top(stack_two) == two, "STACK two's size & top");
		cap_stack_swap(stack_one, stack_two);
		CAP_ASSERT_TRUE(*(int*)cap_stack_top(stack_one) == two &&
				cap_stack_size(stack_one) == 3, "STACK one after swap");
		CAP_ASSERT_TRUE(*(int*)cap_stack_top(stack_two) == three &&
				cap_stack_size(stack_two) == 1, "STACK two after swap");
	}
}
