#include "map.h"

int compare_fn(void *x, void *y) {
	if (*(int *)x > *(int *)y) return 1;
	if (*(int *)x < *(int *)y) return -1;
	return 0;
}

int main(int argc, const char *argv[]) {
	cap_map *map = cap_map_init(sizeof(int), compare_fn);
	int key_one = 10;
	int key_two = 20;
	int key_three = 30;
	int key_four = 40;
	float value_one = 10.10f;
	float value_two = 20.20f;
	float value_three = 30.30f;
	float value_four = 40.40f;

	int push_status_one = cap_map_insert(map, &key_one, &value_four);
	int push_status_two = cap_map_insert(map, &key_two, &value_two);
	int push_status_three = cap_map_insert(map, &key_three, &value_three);
	int push_status_four = cap_map_insert(map, &key_four, &value_four);

	if (cap_map_size(map) == 4) {
		printf("Currently the size of the map is 4\n");
	}

	int *find_three = cap_map_find(map, &key_three);

	if (find_three != NULL && *find_three == value_three) {
		printf("Fount the value with key three\n");
	}

	bool two_contains = cap_map_contains(map, &key_two);

	if (two_contains) { printf("Key contains on the map\n"); }

	int remove_status_four = cap_map_remove(map, &key_four);

	if (remove_status_four == 0) { printf("Key removed successfully\n"); }

	cap_map_free(map);

	/* Other supported APIs:
	 *	-> int (*cap_map_compare_fn(cap_map *map))(void *, void *);
	 *		Get the compare function which used internally.
	 *	-> int cap_map_height(cap_map *map);
	 *		Get the max height supported by the internal skip-list implementation.
	 *	-> void cap_map_deep_free(cap_map *map);
	 *		Free the memory that the container owns and also the memory of the elements (assuming it's dynamically allocated)
	 *	-> bool cap_map_empty(cap_map *map);
	 *		Check if the container is empty or not.
	 */

	return 0;
}
