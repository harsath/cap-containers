#include "vector.h"
#include <stdio.h>

bool predicate_fn_for_iterator(void* data){
	if(*(int*)data == 20) return true;
	return false;
}

int main(int argc, const char* argv[]){

	/*
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

	// Please Take a look at 'cap-containers/vector.h' source file for all interface

	cap_vector* vector = cap_vector_init(10); // Initial capacity = 10
	int one = 10; int two = 20; int three = 30; int four = 40; int five = 50; int six = 60;
	cap_vector_push_back(vector, &one);
	cap_vector_push_back(vector, &two);
	cap_vector_push_back(vector, &three);
	cap_vector_push_back(vector, &four);
	cap_vector_push_back(vector, &five);
	cap_vector_push_back(vector, &six);

	// Also call 'cap_vector_end' or 'cap_vector_iterator_init'
	// 	-> 'cap_vector_end' will allocate an iterator and point to last element of the container.
	// 	-> 'cap_vector_iterator_init' will allocate an iterator and point to first element to container(same as 'cap_vector_begin')
	cap_vector_iterator* iterator = cap_vector_begin(vector);
	int new_element = 500;

	if(*(int*)iterator->data == one){ printf("iterator is pointing to first element\n"); }

	cap_vector_iterator_increment(iterator);

	if(*(int*)iterator->data == one){ printf("iiterator is pointing to 2nd element\n"); }

	int* three_ptr = cap_vector_iterator_next(iterator);

	if(*three_ptr == three){ printf("This pointer is to 3rd element\n"); }

	size_t current_iterator_index = cap_vector_iterator_index(iterator);

	if(current_iterator_index == 2){ printf("Current index of iterator is 2\n"); }

	cap_vector_iterator_free(iterator);

	bool iterator_eq_status = cap_vector_iterator_equals_predicate(iterator, predicate_fn_for_iterator);

	if(iterator_eq_status){ printf("Result of the predicate is true\n"); }

	for(size_t i = cap_vector_iterator_index(iterator); i < cap_vector_size(vector); i++){
		printf("Current element: %d\n", *(int*)iterator->data);
		cap_vector_iterator_increment(iterator);
	}

	cap_vector_free(vector);
	cap_vector_iterator_free(iterator);
	
	return 0;
}
