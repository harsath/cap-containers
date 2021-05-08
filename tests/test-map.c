#include "internal/test-helper.h"
#include <map.h>

static int compare_fn_int(void *x, void *y) {
	if (*(int *)x > *(int *)y)
		return 1;
	else if (*(int *)x < *(int *)y)
		return -1;
	return 0;
}

void test_map(void) {
	{
		cap_map *map = cap_map_init(sizeof(int), compare_fn_int);
		CAP_ASSERT_EQ(cap_map_size(map), 0, "MAP size after init");
		CAP_ASSERT_EQ(cap_map_height(map), CAP_MAP_MAX_SKIPLIST_SIZE,
			      "MAP height after init");
		CAP_ASSERT_TRUE(cap_map_empty(map),
				"MAP empty check after init");
		int key_one = 1;
		float value_one = 10.0f;
		CAP_ASSERT_EQ(cap_map_insert(map, &key_one, &value_one), 0,
			      "MAP insert key one return");
		int key_two = 2;
		float value_two = 20.0f;
		CAP_ASSERT_EQ(cap_map_insert(map, &key_two, &value_two), 0,
			      "MAP insert key two return");
		CAP_ASSERT_EQ(cap_map_size(map), 2,
			      "MAP size after inserting two items");
		int key_three = 3;
		int key_four = 4;
		int key_five = 5;
		float value_three = 30.0f;
		float value_four = 40.0f;
		float value_five = 50.0f;
		cap_map_insert(map, &key_three, &value_three);
		cap_map_insert(map, &key_four, &value_four);
		cap_map_insert(map, &key_five, &value_five);
		int invalid_key = 99;
		CAP_ASSERT_EQ(cap_map_remove(map, &invalid_key), -1,
			      "MAP invalid key remove");
		CAP_ASSERT_EQ(cap_map_size(map), 5,
			      "MAP size after five insert, one invalid remove");
		float *four_ptr = cap_map_find(map, &key_four);
		CAP_ASSERT_TRUE(four_ptr != NULL && *four_ptr == value_four,
				"MAP find four");
		CAP_ASSERT_EQ(cap_map_remove(map, &key_four), 0,
			      "MAP remove key four");
		CAP_ASSERT_EQ(cap_map_remove(map, &key_four), -1,
			      "MAP re-remove key four");
		float *five_ptr = cap_map_find(map, &key_five);
		CAP_ASSERT_TRUE(five_ptr != NULL && *five_ptr == value_five,
				"MAP find key five");
		float *two_ptr = cap_map_find(map, &key_two);
		CAP_ASSERT_TRUE(two_ptr != NULL && *two_ptr == value_two,
				"MAP find key two");
		CAP_ASSERT_EQ(cap_map_remove(map, &key_two), 0,
			      "MAP removed key two");
		float *three_ptr = cap_map_find(map, &key_three);
		CAP_ASSERT_TRUE(three_ptr != NULL && *three_ptr == value_three,
				"MAP find key two");
		CAP_ASSERT_FALSE(cap_map_contains(map, &key_two),
				 "MAP contains on removed key, two");
		CAP_ASSERT_TRUE(cap_map_contains(map, &key_one),
				"MAP contains on key one");
		CAP_ASSERT_FALSE(cap_map_empty(map),
				 "MAP empty checks after operations");
		cap_map_free(map);
	}
}
