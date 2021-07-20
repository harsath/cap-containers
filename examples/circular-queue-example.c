#include "circular_queue.h"

int main(int argc, const char *argv[]) {
	// Please Take a look at 'cap-containers/dynamic_queue.h' source file for all interface

	// Allocating a container of size 3
	cap_circular_queue *cqueue = cap_circular_queue_init(3);
	int one = 10;
	int two = 20;
	int three = 30;
	cap_circular_queue_push(cqueue, &one);
	cap_circular_queue_push(cqueue, &two);

	if (cap_circular_queue_size(cqueue) == 2) {
		printf("Currently the container stores 2 elements\n");
	}

	cap_circular_queue_push(cqueue, &three);

	int *one_ptr = cap_circular_queue_pop(cqueue);
	int *two_ptr = cap_circular_queue_pop(cqueue);
	int *three_ptr = cap_circular_queue_pop(cqueue);

	if (cap_circular_queue_size(cqueue) == 0) {
		printf("Currently the container stores 0 element\n");
	}

	if (*one_ptr == one) { printf("1st element poped was one\n"); }

	if (*two_ptr == two) { printf("2nd element if poped was two\n"); }

	if (*three_ptr == three) { printf("Final element poped was three\n"); }

	cap_circular_queue_free(cqueue);

	/* Other supported APIs:
	 *	-> size_t cap_circular_queue_capacity(cap_circular_queue*)
	 *		Returns the capacity of the container.
	 *	-> void cap_circular_queue_deep_free(cap_circular_queue*)
	 *		Free the container and also the underlying elements.
	 */

	return 0;
}
