#include "internal/test-helper.h"
#include <priority_queue.h>

static int compare_fn_int(void *x, void *y) { return (*(int *)x - *(int *)y); }

static void print_pqueue(cap_priority_queue *pqueue) {
	for (int i = 1; i <= pqueue->_size; ++i) {
		printf("%d ", *(int *)pqueue->_internal_buffer[i]);
	}
	printf("\n");
}

void test_priority_queue(void) {
	{
		cap_priority_queue *pqueue =
		    cap_priority_queue_init(compare_fn_int);
		int one = 1;
		int two = 54;
		int three = 43;
		int four = 2;
		int five = 32;
		int six = 2;
		int seven = 83;
		CAP_ASSERT_EQ(0, cap_priority_queue_size(pqueue),
			      "PRIORITY_QUEUE size before any push");
		bool one_push = cap_priority_queue_push(pqueue, &one);
		CAP_ASSERT_TRUE(one_push,
				"PRIORITY_QUEUE first push return value");
		CAP_ASSERT_TRUE(cap_priority_queue_size(pqueue) == 1 &&
				    *(int *)cap_priority_queue_pop(pqueue) ==
					one &&
				    cap_priority_queue_size(pqueue) == 0,
				"PRIORITY_QUEUE pop first item with size");
		cap_priority_queue_push(pqueue, &one);
		cap_priority_queue_push(pqueue, &two);
		cap_priority_queue_push(pqueue, &three);
		cap_priority_queue_push(pqueue, &four);
		cap_priority_queue_push(pqueue, &five);
		CAP_ASSERT_EQ(cap_priority_queue_size(pqueue), 5,
			      "PRIORITY_QUEUE size after five insert");
		CAP_ASSERT_TRUE(
		    *(int *)cap_priority_queue_pop(pqueue) == two &&
			cap_priority_queue_size(pqueue) == 4,
		    "PRIORITY_QUEUE pop after 5 pushes and size check");
		cap_priority_queue_push(pqueue, &two);
		cap_priority_queue_push(pqueue, &six);
		cap_priority_queue_push(pqueue, &seven);
		CAP_ASSERT_EQ(cap_priority_queue_size(pqueue), 7,
			      "PRIORITY_QUEUE size after seven inserts");
		CAP_ASSERT_TRUE(
		    *(int *)cap_priority_queue_top(pqueue) == seven &&
			cap_priority_queue_size(pqueue) == 7,
		    "PRIORITY_QUEUE top after seven inserts insert checks");
		CAP_ASSERT_TRUE(
		    *(int *)cap_priority_queue_pop(pqueue) == seven &&
			*(int *)cap_priority_queue_pop(pqueue) == two &&
			*(int *)cap_priority_queue_pop(pqueue) == three &&
			*(int *)cap_priority_queue_pop(pqueue) == five &&
			cap_priority_queue_size(pqueue) == 3,
		    "PRIORITY_QUEUE four pop checks and size after pops");
		CAP_ASSERT_TRUE(
		    *(int *)cap_priority_queue_pop(pqueue) == six &&
			*(int *)cap_priority_queue_pop(pqueue) == four &&
			*(int *)cap_priority_queue_pop(pqueue) == one &&
			cap_priority_queue_size(pqueue) == 0,
		    "PRIORITY_QUEUE final pop");
		cap_priority_queue_free(pqueue);
	}
}
