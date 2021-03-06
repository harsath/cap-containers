#include <fixed_queue.h>
#include <gtest/gtest.h>

TEST(Cap_Container, Cap_Container_fixed_queue){
	{
		// [] [] [] [] []
		cap_fixed_queue* fixed_queue = cap_fixed_queue_init(5);
		ASSERT_EQ(cap_fixed_queue_get_capacity(fixed_queue), 5);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 0);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 5);

		int value_one = 10; // [10] [] [] [] []
		cap_fixed_queue_push(fixed_queue, &value_one);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 1);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 4);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one);

		int value_two = 20; // [20] [10] [] [] []
		cap_fixed_queue_push(fixed_queue, &value_two);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 2);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 3);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one);
		ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two);

		// [20] [] [] [] []
		int* pop_val_one = (int*)cap_fixed_queue_pop(fixed_queue);
		ASSERT_EQ(cap_fixed_queue_get_size(fixed_queue), 1);
		ASSERT_EQ(cap_fixed_queue_get_remaining_space(fixed_queue), 4);
		ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_two);
		ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two);
		ASSERT_EQ(*pop_val_one, value_one);

		// [] [] [] []
		cap_fixed_queue* fixed_queue_two = cap_fixed_queue_init(4);
		int value_one_ = 100;
		int value_two_ = 200;
		int value_three_ = 300;
		int value_four_ = 400;
		// [400] [300] [200] [100]
		cap_fixed_queue_push(fixed_queue_two, &value_one_);
		cap_fixed_queue_push(fixed_queue_two, &value_two_);
		cap_fixed_queue_push(fixed_queue_two, &value_three_);
		cap_fixed_queue_push(fixed_queue_two, &value_four_);
		ASSERT_TRUE((cap_fixed_queue_get_capacity(fixed_queue_two) == 4) && (cap_fixed_queue_get_size(fixed_queue_two) == 4)
				&& (cap_fixed_queue_get_remaining_space(fixed_queue_two) == 0));
		ASSERT_TRUE(
				(*((int*)cap_fixed_queue_front(fixed_queue_two)) == value_one_) &&
				(*((int*)cap_fixed_queue_back(fixed_queue_two)) == value_four_)
			   );
		ASSERT_EQ(*((int*)cap_fixed_queue_pop(fixed_queue_two)), value_one_);
		ASSERT_TRUE(
				(*((int*)cap_fixed_queue_front(fixed_queue_two)) == value_two_) &&
				(*((int*)cap_fixed_queue_back(fixed_queue_two)) == value_four_)
			   );
		cap_fixed_queue_push(fixed_queue_two, &value_one_);

		cap_fixed_queue_swap(fixed_queue, fixed_queue_two);
		ASSERT_TRUE((cap_fixed_queue_get_capacity(fixed_queue) == 4) && (cap_fixed_queue_get_size(fixed_queue) == 4)
				&& (cap_fixed_queue_get_remaining_space(fixed_queue) == 0));
		ASSERT_TRUE((cap_fixed_queue_get_capacity(fixed_queue_two) == 5) && (cap_fixed_queue_get_size(fixed_queue_two) == 1)
				&& (cap_fixed_queue_get_remaining_space(fixed_queue_two) == 4));
		
		cap_fixed_queue_free(fixed_queue);
		cap_fixed_queue_free(fixed_queue_two);
	}
}
