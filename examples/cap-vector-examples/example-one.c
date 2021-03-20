#include "vector.h"
#include <stdio.h>

bool find_element_five(void* data){
	if(*(int*)data == 50) return true;
	else return false;
}

bool remove_element_three(void* data){
	if(*(int*)data == 30) return true;
	else return false;
}

// Let's add 1 to all elements in the vector
void do_for_all_elements(void* data){
	(*(int*)data)++;
}

int main(int argc, const char*const argv[]){
	// Please Take a look at 'cap-containers/vector.h' source file for all interface

	cap_vector* vector = cap_vector_init(5); // Initial capacity = 5
	int one = 10; int two = 20; int three = 30; int four = 40; int five = 50; int six = 60;

	cap_vector_push_back(vector, &one);
	cap_vector_push_back(vector, &two);

	if(*(int*)cap_vector_front(vector) == two){ printf("two is at front\n"); }

	if(cap_vector_size(vector) == 2){ printf("size of vector is 2\n"); }

	int* one_ptr = cap_vector_pop_back(vector);

	if(cap_vector_size(vector) == 1){ printf("Size of vector is 1\n"); }

	if(*one_ptr == one){ printf("Poped value from vector is one\n"); }

	cap_vector_push_back(vector, &two);
	cap_vector_push_back(vector, &three);
	cap_vector_push_back(vector, &four);

	if(cap_vector_remaining_space(vector) == 1){ printf("4/5 elements filled\n"); }

	if(cap_vector_capacity(vector) == 5){ printf("Current capacity of vector is 5\n"); }

	cap_vector_push_back(vector,  &five);
	cap_vector_push_back(vector, &six);

	// Vector dynamically grows in the order of 2*CURRENT_CAPACITY
	if(cap_vector_capacity(vector) == 10){ printf("New capacity of vector is 10\n"); }

	// 'cap_vector_find_if' accepts a call-back predicate to find the first occurrence element you are looking for. If not found, it returns NULL
	int* find_item_five = cap_vector_find_if(vector, find_element_five);

	if(find_item_five != NULL && *find_item_five == five){ printf("five is found\n"); }

	bool remove_three_status = cap_vector_remove_if(vector, remove_element_three);

	if(remove_three_status){ printf("three has been removed\n"); }

	// 'cap_vector_for_each' will call a call-back function by passing the elements aka, does the operation on the callback function for all items
	// As per 'do_for_all_elements' function, this will add '1' to all elements
	cap_vector_for_each(vector, do_for_all_elements);

	// Let's create a copy of the 'vector'. This will NOT create a 'deep-copy' aka, both containers will point to same underlying elements.
	cap_vector* vector_copy = cap_vector_copy(vector);

	cap_vector_free(vector);
	cap_vector_free(vector_copy);
	
	/* Other supported APIs:
	 * 	-> void* cap_vector_at(cap_vector*, size_t);
	 * 		Get the element at a certain 'index'.
	 * 	-> cap_vector* cap_vector_deep_copy(cap_vector*);
	 *		Allocates memory for the items in the passed vector. DOES NOT point the same underlying memory as the container passed.
	 *	-> void cap_vector_swap(cap_vector*, cap_vector*);
	 *		Swap two 'cap_vector' objects.
	 *	-> bool cap_vector_resize(cap_vector*, size_t);
	 *		Resize the 'cap_vector' object to certain size. Returns 'true' if success.
	 *	-> bool cap_vector_shrink_to_fit(cap_vector*);
	 *		Shrink the capacity(memory) of the 'cap_vector' container to the current size. Returns true if success.
	 *	-> void cap_vector_deep_free(cap_vector*);
	 *		Free the underlying elements stored on the container along with the 'cap_vector' object.
	 *	-> bool cap_vector_empty(cap_vector*);
	 *		Check if the 'cap_vector' object is empty or not.
	 *
	 *	##### Iterator Interface #####
	 *	-> cap_vector_iterator* cap_vector_iterator_init(cap_vector*);
	 *		Create a 'iterator' object for the vector container.
	 *	-> void cap_vector_iterator_insert(cap_vector_iterator*, void*);
	 *		Insert the item at the current iterator's place.
	 *	-> void cap_vector_iterator_remove(cap_vector_iterator*);
	 *		Remove the item which is stored at the current iterator's place.
	 *	-> void cap_vector_iterator_increment(cap_vector_iterator*);
	 *		Increment the iterator to next element. If reached end of container, 'data' is marked as NULL.
	 *	-> void cap_vector_iterator_decrement(cap_vector_iterator*);
	 *		Decrement the iterator to the previous element. If eached '0', further call will make the 'data' to NULL.
	 *	-> void* cap_vector_iterator_next(cap_vector_iterator*);
	 *		Peek the next 'data' but do not increment the iterator.
	 *	-> void* cap_vector_iterator_previous(cap_vector_iterator*);
	 *		Peek into the previous 'data' but do not decrement the iterator.
	 *	-> bool cap_vector_iterator_equals(cap_vector_iterator*, cap_vector_iterator*);
	 *		Check if the 'data' pointed by the 'iterator one' and 'iterator two' are same. If so, returns 'true'.
	 *	-> bool cap_vector_iterator_equals_predicate(cap_vector_iterator*, bool(*)(void*));
	 *		Calls a predicate call-back function on the current iterator and returns the bool result of the callback.
	 *	-> size_t cap_vector_iterator_index(cap_vector_iterator*);
	 *		Get the current index of the iterator in the underlying 'cap_vector' container.
	 *	-> cap_vector_iterator* cap_vector_begin(cap_vector*);
	 *		Get iterator to first element of the 'cap_vector' container.
	 *	-> cap_vector_iterator* cap_vector_end(cap_vector*);
	 *		Get iterator to last element of the 'cap_vector' container.
	 *	-> void cap_vector_iterator_free(cap_vector_iterator*);
	 *		Free the 'cap_vector_iterator' object DOES NOT frees the underlying 'data'.
	 */
	
	return 0;
}
