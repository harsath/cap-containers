#include "dynamic_queue.h"

int main(int argc, const char* argv[]){
	// Please Take a look at 'cap-containers/dynamic_queue.h' source file for all interface
	
	cap_dynamic_queue* queue = cap_dynamic_queue_init();
	int value_one = 10;
	int value_two = 20;
	int value_three = 30;
	// Elements in queue: [30] [20] [10]
	cap_dynamic_queue_push(queue, &value_one);
	cap_dynamic_queue_push(queue, &value_two);
	cap_dynamic_queue_push(queue, &value_three);

	if(*(int*)cap_dynamic_queue_front(queue) == value_one)
	{ printf("It's front of the queue!\n"); }

	if(*(int*)cap_dynamic_queue_back(queue) == value_three)
	{ printf("It's back of the queue!\n"); }

	int* value_one_ptr = cap_dynamic_queue_pop(queue);

	if(cap_dynamic_queue_size(queue) == 2)
	{ printf("Queue's size is 2, since we poped\n"); }

	cap_dynamic_queue_free(queue);

	/* Other supported APIs:
	 * 	-> size_t cap_dynamic_queue_size(cap_dynamic_queue*);
	 * 		Returns the size of the queue	
	 *
	 * 	-> void cap_dynamic_queue_swap(cap_dynamic_queue*, cap_dynamic_queue*);
	 * 		Swaps two 'cap_dynamic_queue' objects
	 *
	 * 	-> void cap_dynamic_queue_free(cap_dynamic_queue*);
	 * 		Only frees the 'cap_dynamic_queue' object and does not touch the underlying elements
	 *
	 * 	-> void cap_dynamic_queue_deep_free(cap_dynamic_queue*);
	 * 		Frees both 'cap_dynamic_queue' and underlying elements(assyming the elements are dynamically allocated)
	 */
	
	return 0;
}
