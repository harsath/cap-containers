#include "internal/test-helper.h"
#include <fixed_queue.h>

void test_fixed_queue(void){
	{
		// [] [] [] [] []
		cap_fixed_queue* fixed_queue = cap_fixed_queue_init(5);
		CAP_ASSERT_EQ(cap_fixed_queue_capacity(fixed_queue), 5, "FIXED_QUEUE capacity");
		CAP_ASSERT_EQ(cap_fixed_queue_size(fixed_queue), 0, "FIXED_QUEUE size");
		CAP_ASSERT_EQ(cap_fixed_queue_remaining_space(fixed_queue), 5, "FIXED_QUEUE remaining space");

		int value_one = 10; // [10] [] [] [] []
		cap_fixed_queue_push(fixed_queue, &value_one);
		CAP_ASSERT_EQ(cap_fixed_queue_size(fixed_queue), 1, "FIXED_QUEUE size");
		CAP_ASSERT_EQ(cap_fixed_queue_remaining_space(fixed_queue), 4, "FIXED_QUEUE remaining space");
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one, "FIXED_QUEUE front");

		int value_two = 20; // [20] [10] [] [] []
		cap_fixed_queue_push(fixed_queue, &value_two);
		CAP_ASSERT_EQ(cap_fixed_queue_size(fixed_queue), 2, "FIXED_QUEUE size");
		CAP_ASSERT_EQ(cap_fixed_queue_remaining_space(fixed_queue), 3, "FIXED_QUEUE remaining space");
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_one, "FIXED_QUEUE front");
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two, "FIXED_QUEUE back");

		// [20] [] [] [] []
		int* pop_val_one = (int*)cap_fixed_queue_pop(fixed_queue);
		CAP_ASSERT_EQ(cap_fixed_queue_size(fixed_queue), 1, "FIXED_QUEUE size");
		CAP_ASSERT_EQ(cap_fixed_queue_remaining_space(fixed_queue), 4, "FIXED_QUEUE remaining space");
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_front(fixed_queue)), value_two, "FIXED_QUEUE front");
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_back(fixed_queue)), value_two, "FIXED_QUEUE back");
		CAP_ASSERT_EQ(*pop_val_one, value_one, "FIXED_QUEUE pop value check");

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
		CAP_ASSERT_TRUE((cap_fixed_queue_capacity(fixed_queue_two) == 4) && (cap_fixed_queue_size(fixed_queue_two) == 4)
				&& (cap_fixed_queue_remaining_space(fixed_queue_two) == 0), "FIXED_QUEUE various checks");
		CAP_ASSERT_TRUE(
				(*((int*)cap_fixed_queue_front(fixed_queue_two)) == value_one_) &&
				(*((int*)cap_fixed_queue_back(fixed_queue_two)) == value_four_), "FIXED_QUEUE front & back"
			   );
		CAP_ASSERT_EQ(*((int*)cap_fixed_queue_pop(fixed_queue_two)), value_one_, "FIXED_QUEUE pop");
		CAP_ASSERT_TRUE(
				(*((int*)cap_fixed_queue_front(fixed_queue_two)) == value_two_) &&
				(*((int*)cap_fixed_queue_back(fixed_queue_two)) == value_four_), "FIXED_QUEUE front & back"
			   );
		cap_fixed_queue_push(fixed_queue_two, &value_one_);

		cap_fixed_queue_swap(fixed_queue, fixed_queue_two);
		CAP_ASSERT_TRUE((cap_fixed_queue_capacity(fixed_queue) == 4) && (cap_fixed_queue_size(fixed_queue) == 4)
				&& (cap_fixed_queue_remaining_space(fixed_queue) == 0), "FIXED_QUEUE various checks");
		CAP_ASSERT_TRUE((cap_fixed_queue_capacity(fixed_queue_two) == 5) && (cap_fixed_queue_size(fixed_queue_two) == 1)
				&& (cap_fixed_queue_remaining_space(fixed_queue_two) == 4), "FIXED_QUEUE various checks");
		
		cap_fixed_queue_free(fixed_queue);
		cap_fixed_queue_free(fixed_queue_two);
	}
}
