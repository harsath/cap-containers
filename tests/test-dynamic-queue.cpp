#include <dynamic_queue.h>
#include <gtest/gtest.h>

TEST(Cap_Container, Cap_Container_dynamic_queue){
	{
		cap_dynamic_queue* dynamic_queue = cap_dynamic_queue_init();
		ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 0);
		int value_one = 10;
		int value_two = 20;
		int value_three = 30;
		// [30] [20] [10]
		//  B          F
		cap_dynamic_queue_push(dynamic_queue, &value_one);
		cap_dynamic_queue_push(dynamic_queue, &value_two);
		cap_dynamic_queue_push(dynamic_queue, &value_three);
		ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 3);
		ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_one);
		int* ptr_value_one = (int*)cap_dynamic_queue_pop(dynamic_queue);
		ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_two);
		int* ptr_value_two = (int*)cap_dynamic_queue_pop(dynamic_queue);
		ASSERT_TRUE(*(int*)cap_dynamic_queue_back(dynamic_queue) == value_three &&
				*(int*)cap_dynamic_queue_front(dynamic_queue) == value_three);
		// [30]
		ASSERT_EQ(cap_dynamic_queue_size(dynamic_queue), 1);
		ASSERT_TRUE(*ptr_value_one == value_one &&
				*ptr_value_two == value_two);
		ASSERT_TRUE(*(int*)cap_dynamic_queue_pop(dynamic_queue) == value_three &&
				cap_dynamic_queue_back(dynamic_queue) == NULL &&
				cap_dynamic_queue_back(dynamic_queue) == NULL &&
				cap_dynamic_queue_pop(dynamic_queue) == NULL);
	}
}
