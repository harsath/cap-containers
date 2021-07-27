#include "map.h"

bool predicate_fn_for_iterator(void *key) {
	if (*(int *)key == 2) return true;
	return false;
}
int compare_fn(void *x, void *y) {
	if (*(int *)x > *(int *)y) return 1;
	if (*(int *)x < *(int *)y) return -1;
	return 0;
}

int main(int argc, const char *argv[]) {

	/*
	 *	##### Iterator Interface #####
	 *	-> cap_map_iterator* cap_map_iterator_init(cap_map*);
	 *		Create a 'iterator' object for the map container.
	 *	-> void cap_map_iterator_insert(cap_map_iterator*, void*);
	 *		Insert the item at the current iterator's place.
	 *	-> void cap_map_iterator_remove(cap_map_iterator*);
	 *		Remove the item which is stored at the current
	 *iterator's place.
	 *	-> void cap_map_iterator_increment(cap_map_iterator*);
	 *		Increment the iterator to next element. If reached end
	 *of container, 'value' is marked as NULL.
	 *	-> void* cap_map_iterator_next(cap_map_iterator*);
	 *		Peek the next 'value' but do not increment the iterator.
	 *	-> bool cap_map_iterator_equals_predicate(cap_map_iterator*,
	 *bool(*)(void*)); Calls a predicate call-back function on the current
	 *iterator and returns the bool result of the callback.
	 *	-> cap_map_iterator* cap_map_begin(cap_map*);
	 *		Get iterator to first element of the 'cap_map'
	 *container.
	 *	-> cap_map_iterator* cap_map_front(cap_map*);
	 *		Get iterator to first element of the 'cap_map'
	 *container.
	 *	-> void cap_map_iterator_free(cap_map_iterator*);
	 *		Free the 'cap_map_iterator' object DOES NOT frees the
	 *underlying 'value'.
	 */

	// Please Take a look at 'cap-containers/map.h' source file for all
	// interface

	cap_map *map =
	    cap_map_init(sizeof(int), compare_fn); // Initial capacity = 10

	int key_one = 1;
	int key_two = 2;
	int key_three = 3;
	int key_four = 4;
	int key_five = 5;
	float value_one = 10.10f;
	float value_two = 20.20f;
	float value_three = 30.30f;
	float value_four = 40.40f;
	float value_five = 50.50f;

	cap_map_insert(map, &key_one, &value_one);
	cap_map_insert(map, &key_two, &value_two);
	cap_map_insert(map, &key_three, &value_three);
	cap_map_insert(map, &key_four, &value_four);
	cap_map_insert(map, &key_five, &value_five);

	if (cap_map_size(map) == 5) {
		printf("Currently the size of the map is 5\n");
	}
	// Also call 'cap_map_iterator_init'
	// 	-> 'cap_map_iterator_init' will allocate an iterator and point
	// to first element to container(same as 'cap_map_begin')
	cap_map_iterator *iterator = cap_map_iterator_init(map);

	if (*(int *)iterator->key == key_one) {
		printf("Iterator is pointing to 1st element\n");
	}

	cap_map_iterator_increment(iterator);

	if (*(int *)iterator->key == key_two) {
		printf("Iterator is pointing to 2nd element\n");
	}

	int *three_ptr = cap_map_iterator_next(iterator);

	if (*three_ptr == key_three) {
		printf("Iterator is pointer is to 3rd element\n");
	}

	bool three_contains = cap_map_contains(map, &key_three);

	if (three_contains) { printf("Key contains on the map\n"); }

	cap_map_iterator *map_begin = cap_map_begin(map);

	if (map_begin) { printf("Pointing to 1st element\n"); }

	if (*(int *)iterator->key == key_two) {
		printf("Iterator is pointing to 2nd element\n");
	}

	bool iterator_eq_status = cap_map_iterator_equals_predicate(
	    iterator, predicate_fn_for_iterator);

	if (iterator_eq_status) { printf("Result of the predicate is true\n"); }

	int key_seven = 70;
	float value_seven = 70.70f;
	cap_map_insert(map, &key_seven, &value_seven);

	int remove_status_seven = cap_map_remove(map, &key_seven);

	if (remove_status_seven == 0) { printf("Key removed successfully\n"); }

	cap_map *temp_map =
	    cap_map_init(sizeof(int), compare_fn); // Initial capacity = 10
	int key_temp_one = 1;
	float value_temp_one = 10.10f;
	int key_temp_two = 2;
	float value_temp_two = 20.20f;

	cap_map_insert(temp_map, &key_temp_one, &value_temp_one);
	cap_map_insert(temp_map, &key_temp_two, &value_temp_two);

	if (cap_map_size(temp_map) == 2) {
		printf("Currently the size of the map is 2\n");
	}

	cap_map_iterator *temp_iterator = cap_map_iterator_init(temp_map);

	cap_map_iterator_increment(temp_iterator);

	int *temp_map_front = cap_map_front(temp_map);

	if (temp_map_front) { printf("Pointing to 1st element by front\n"); }

	if (cap_map_empty(temp_map) == 0) { printf("Empty temp map\n"); }

	cap_map_iterator_free(temp_iterator);
	cap_map_iterator_free(iterator);
	cap_map_free(temp_map);
	cap_map_free(map);
	return 0;
}