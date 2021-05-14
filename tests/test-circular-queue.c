#include "internal/test-helper.h"
#include <circular_queue.h>
#define _print_int_(val) printf("%d\n", val)

void test_circular_queue(void) {
	{
		cap_circular_queue *cqueue = cap_circular_queue_init(4);
		CAP_ASSERT_EQ(cap_circular_queue_capacity(cqueue), 4,
			      "CIRCULAR_QUEUE size after init");
		int one = 10;
		// one
		cap_circular_queue_push(cqueue, &one);
		int two = 20;
		// two one
		cap_circular_queue_push(cqueue, &two);
		// two
		CAP_ASSERT_EQ(*(int *)cap_circular_queue_pop(cqueue), one,
			      "CIRCULAR_QUEUE pop item one");
		int three = 30;
		int four = 40;
		int five = 50;
		// five four three one
		cap_circular_queue_push(cqueue, &one);
		cap_circular_queue_push(cqueue, &three);
		cap_circular_queue_push(cqueue, &four);
		cap_circular_queue_push(cqueue, &five);
		CAP_ASSERT_EQ(*(int *)cap_circular_queue_pop(cqueue), five,
			      "CIRCULAR_QUEUE 1st pop after 5 insert");
	}
}
