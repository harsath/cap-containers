#include <fixed_queue.h>
#include <gtest/gtest.h>

TEST(Cap_Container, Cap_Container_fixed_queue){
	{
		cap_fixed_queue* fixed_queue = cap_fixed_queue_init(5);
		ASSERT_EQ(cap_fixed_queue_get_capacity(fixed_queue), 5);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 0);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 5);

		int value_one = 10;
		cap_fixed_queue_push(fixed_queue, &value_one);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 1);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 4);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one);

		int value_two = 20;
		cap_fixed_queue_push(fixed_queue, &value_two);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 2);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 3);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one);
		ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two);

		int* pop_val_one = (int*)cap_fixed_queue_pop(fixed_queue);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 1);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 4);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_two);
		ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two);
	}
}
