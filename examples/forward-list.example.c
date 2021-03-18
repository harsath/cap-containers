#include "forward_list.h"

// Predicate function for getting the 'value_two' from linked list
bool find_value_two(void* data){
	if(*(int*)data == 20){
		return true;
	}else{
		return false;
	}
}

bool find_value_three(void* data){
	if(*(int*)data == 30){
		return true;
	}else{
		return false;
	}
}

int main(int argc, const char* argv[]){
	// Please Take a look at 'cap-containers/forward_list.h' source file for all interface
	
	cap_forward_list* list = cap_forward_list_init();
	int value_one = 10;
	int value_two = 20;
	int value_three = 30;
	// [30] -> [20] -> [10]
	// front            back
	cap_forward_list_push_front(list, &value_one);
	cap_forward_list_push_front(list, &value_two);
	cap_forward_list_push_front(list, &value_three);

	if(*(int*)cap_forward_list_front(list) == value_three){
		printf("30 is front of list\n");
	}

	if(cap_forward_list_size(list) == 3){
		printf("Size of the list is three elements\n");
	}

	int* value_two_ptr = cap_forward_list_find_if(list, find_value_two);
	if(value_two_ptr != NULL && *(int*)value_two_ptr == value_two){
		printf("value_two found on the list\n");
	}

	// Let's remove the item 30 or 'value_three' from the list
	// [20] -> [10]
	bool remove_status = cap_forward_list_remove_if(list, find_value_three);
	if(remove_status){
		printf("value_three or 30 removed\n");
	}

	// [10]
	int* value_two_ptr_pop = cap_forward_list_pop_front(list);
	if(*value_two_ptr_pop == value_two){
		printf("value_two has been poped\n");
	}

	if(cap_forward_list_size(list) == 1){
		printf("Two items removed, size is one\n");
	}

	cap_forward_list_free(list);

	/* Other supported APIs:
	 * 	-> void cap_forward_list_swap(cap_forward_list*, cap_forward_list*);
	 * 		Swap two 'cap_forward_list' objects
	 *	-> bool cap_forward_list_empty(cap_forward_list*);
	 *		Returns 'true' if the list is empty, otherwise returns 'false'
	 *	-> void cap_forward_list_deep_free(cap_forward_list*);
	 *		Frees the 'cap_forward_list' object and the underlying items(Assuming the items are dynamically allocated)
	 *	-> void cap_forward_list_free(cap_forward_list*);
	 *		Only frees the 'cap_forward_list' object and doesn't touch the underlying elements
	 */
	
	return 0;
}
