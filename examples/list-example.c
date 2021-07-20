#include "list.h"

typedef struct {
	int one;
	float two;
	double three;
} Object;

// Predicates for doing operations while iterating the list
bool find_value_three(void* data){
	if(((Object*)data)->one == 43){
		return true;
	}else{
		return false;
	}
}

bool find_value_one(void* data){
	if(((Object*)data)->one == 10){
		return true;
	}else{
		return false;
	}
}

int main(int argc, const char* argv[]){
	// Please Take a look at 'cap-containers/list.h' source file for all interface
	cap_list* list = cap_list_init();
	Object value_one = {.one = 10, .two = 20.3f, .three = 30.32};
	Object value_two = {.one = 20, .two = 30.4f, .three = 43.43};
	Object value_three = {.one = 43, .two = 32.2f, .three = 984.43};
	
	// [two] <-> [one] <-> [three]
	// front                 back
	cap_list_push_front(list, &value_one);
	cap_list_push_back(list, &value_three);
	cap_list_push_front(list, &value_two);

	if(((Object*)cap_list_front(list))->one == value_two.one){
		printf("Object value_two is front of the list\n");
	}

	if(((Object*)cap_list_back(list))->one == value_three.one){
		printf("Object value_three is back of the list\n");
	}

	Object* value_three_ptr = cap_list_find_if(list, find_value_three);

	// [one] <-> [three]
	// front      back
	Object* value_two_ptr = cap_list_pop_front(list);
	
	// [one]
	// front/back
	Object* value_three_ptr_pop = cap_list_pop_back(list);

	// [three] <-> [one]
	cap_list_push_front(list, &value_three);	

	bool remove_one_status = cap_list_remove_if(list, find_value_one);

	if(remove_one_status){
		printf("value_one has been removed from the list\n");
	}

	cap_list_free(list);

	/* Other supported APIs:
	 * 	-> void cap_list_deep_free(cap_list*);
	 *		Frees the 'cap_list' object and the underlying elements(assuming the elements are dynamically allocated)
	 *	-> void cap_list_free(cap_list*);
	 *		Only frees the 'cap_list' object and doesn't touch the underlying elements
	 */

	return 0;
}
