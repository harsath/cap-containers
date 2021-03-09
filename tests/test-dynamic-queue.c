#include <dynamic_queue.h>
#include "internal/test-helper.h"

void test_dynamic_queue(void){
	{
		cap_dynamic_queue* dynamic_queue = cap_dynamic_queue_init();
		CAP_ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 0, "DYNAMIC_QUEUE size");
		int value_one = 10;
		int value_two = 20;
		int value_three = 30;
		// [30] [20] [10]
		//  B          F
		cap_dynamic_queue_push(dynamic_queue, &value_one);
		cap_dynamic_queue_push(dynamic_queue, &value_two);
		cap_dynamic_queue_push(dynamic_queue, &value_three);
		CAP_ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 3, "DYNAMIC_QUEUE size");
		CAP_ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_one, "DYNAMIC_QUEUE front & back");
		int* ptr_value_one = (int*)cap_dynamic_queue_pop(dynamic_queue);
		CAP_ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_two, "DYNAMIC_QUEUE front & back");
		int* ptr_value_two = (int*)cap_dynamic_queue_pop(dynamic_queue);
		CAP_ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_three, "DYNAMIC_QUEUE front & back");
		// [30]
		CAP_ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 1, "DYNAMIC_QUEUE size");
		CAP_ASSERT_TRUE(*ptr_value_one == value_one &&
				*ptr_value_two == value_two, "DYNAMIC_QUEUE value_one & value_two check");
		CAP_ASSERT_TRUE(*(int*)cap_dynamic_queue_pop(dynamic_queue) == value_three &&
				cap_dynamic_queue_back(dynamic_queue) == NULL &&
				cap_dynamic_queue_back(dynamic_queue) == NULL &&
				cap_dynamic_queue_pop(dynamic_queue) == NULL, "DYNAMIC_QUEUE pop & null checks");
	}
}
