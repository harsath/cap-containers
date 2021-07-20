#include "fixed_queue.h"

int main(int argc, const char* argv[]){
	// Please Take a look at 'cap-containers/fixed_queue.h' source file for all interface

	// Let's make the fixed queue of capacity 3
	cap_fixed_queue* queue = cap_fixed_queue_init(3);
	int value_one = 10;
	int value_two = 20;
	int value_three = 30;
	int value_four = 40;
	
	if(cap_fixed_queue_capacity(queue) == 3)
	{ printf("This fixed queue has a capacity of 3\n"); }

	cap_fixed_queue_push(queue, &value_one);

	if(cap_fixed_queue_remaining_space(queue) == 2)
	{ printf("We pushed one item, remaining space is 2\n"); }

	// [30] [20] [10]
	cap_fixed_queue_push(queue, &value_two);
	cap_fixed_queue_push(queue, &value_three);

	// Now the capacity is 0, since we already pushed 3
	bool push_status = cap_fixed_queue_push(queue, &value_four);
	if(!push_status)
	{ printf("push returns false since there is no space for new item"); }

	int* value_one_ptr = cap_fixed_queue_pop(queue);

	push_status = cap_fixed_queue_push(queue, &value_four);

	if(push_status)
	{ printf("Successfully pushed value_four onto fixed-queue"); }
	

	/* Other supported APIs:
	 * 	-> size_t cap_fixed_queue_capacity(cap_fixed_queue*);
	 * 		Returns the capacity of the queue
	 *
	 * 	-> size_t cap_fixed_queue_size(cap_fixed_queue*);
	 * 		Returns the current size/elements of the queue
	 *
	 * 	-> void* cap_fixed_queue_front(cap_fixed_queue*);
	 * 		Gives access to front of the queue
	 *
	 * 	-> void* cap_fixed_queue_back(cap_fixed_queue*);
	 * 		Give access to back of the queue
	 *
	 * 	-> void cap_fixed_queue_swap(cap_fixed_queue*, cap_fixed_queue*);
	 * 		Swaps two 'cap_fixed_queue' objects
	 *
	 * 	-> void cap_fixed_queue_free(cap_fixed_queue*);
	 * 		Only frees 'cap_fixed_queue' object and does not touch the pointers/elements it stores
	 *
	 * 	-> void cap_fixed_queue_deep_free(cap_fixed_queue*);
	 * 		Frees both the 'cap_fixed_queue' and the elements it stores(assuming the objects are dynamically allocated)
	 */

	return 0;
}
