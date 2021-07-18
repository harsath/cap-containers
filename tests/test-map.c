#include "internal/test-helper.h"
#include <map.h>

static int compare_fn_int(void *x, void *y) {
	if (*(int *)x > *(int *)y)
		return 1;
	else if (*(int *)x < *(int *)y)
		return -1;
	return 0;
}

static bool predicate_fn_one(void *key) {
	if (*(int *)key == 2) {
		return true;
	} else {
		return false;
	}
}

static bool predicate_fn_two(void *key) {
	if (*(int *)key == 3) {
		return true;
	} else {
		return false;
	}
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
		// head value_one
		CAP_ASSERT_EQ(cap_map_insert(map, &key_one, &value_one), 0,
			      "MAP insert key one return");
		int key_two = 2;
		float value_two = 20.0f;
		// head value_one value_two
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
		// head value_one value_two value_three value_four value_five
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
		// head value_one value_two value_three
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
		// head value_one value_three value_four
		CAP_ASSERT_EQ(cap_map_remove(map, &key_two), 0,
			      "MAP removed key two");
		CAP_ASSERT_EQ(cap_map_size(map), 3, "MAP size before return");
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
	{ //Tests on iterators
		cap_map *map = cap_map_init(sizeof(int),compare_fn_int);
		CAP_ASSERT_EQ(cap_map_size(map), 0, "MAP size after init");
		CAP_ASSERT_EQ(cap_map_height(map), CAP_MAP_MAX_SKIPLIST_SIZE,
			      "MAP height after init");
		CAP_ASSERT_TRUE(cap_map_empty(map),
				"MAP empty check after init");
		int key_one = 1;
		float value_one = 10.0f;
		cap_map_insert(map, &key_one, &value_one);
		int key_two = 2;
		float value_two = 20.0f;
		cap_map_insert(map, &key_two, &value_two);
		int key_three = 3;
		float value_three = 30.0f;
		cap_map_insert(map, &key_three, &value_three);
		int key_four = 4;
		float value_four = 40.0f;
		cap_map_insert(map, &key_four, &value_four);
		int key_five = 5;
		float value_five = 50.0f;
		cap_map_insert(map, &key_five, &value_five);
		int key_six = 6;
		float value_six = 60.0f;
		cap_map_insert(map, &key_six, &value_six);

		cap_map_iterator *map_iterator =
		    cap_map_iterator_init(map);
		CAP_ASSERT_TRUE(*(int *)map_iterator->key == key_one,
				"MAP Iterator init check");
		cap_map_iterator_increment(map_iterator);
		CAP_ASSERT_TRUE(*(int *)map_iterator->key == key_two,
				"MAP Iterator incr");
		CAP_ASSERT_TRUE(*(int *)cap_map_iterator_next(
				    map_iterator) == key_three &&
				    *(int *)map_iterator->key == key_two,
				"MAP Iterator next");
		CAP_ASSERT_TRUE(cap_map_iterator_equals_predicate(
				    map_iterator, predicate_fn_one),
				"MAP Iterator true predicate");
		CAP_ASSERT_FALSE(cap_map_iterator_equals_predicate(
				     map_iterator, predicate_fn_two),
				 "MAP Iterator false predicate");
		cap_map_iterator_increment(map_iterator); 
		cap_map_iterator *tmp_iter =
		    cap_map_iterator_init(map);
		cap_map_iterator_increment(tmp_iter);
		CAP_ASSERT_EQ(*(int *)tmp_iter->key, key_two,
			      "MAP increment after init");
		cap_map_iterator_free(tmp_iter);
		cap_map_iterator *tmp_begin_iter = cap_map_begin(map);
		cap_map_iterator_free(tmp_begin_iter);
		CAP_ASSERT_EQ(*(int *)map_iterator->key, key_three,
			      "MAP Iterator before remove");
		cap_map_remove(map,&key_three); // remove key_three, currently key_four
		cap_map_iterator_increment(map_iterator);
		CAP_ASSERT_EQ(*(int *)map_iterator->key, key_four,
			      "MAP Iterator after removing only element");
		CAP_ASSERT_EQ(*(int *)cap_map_iterator_next(map_iterator),
			      key_five, "MAP Iterator next after remove");
		cap_map_iterator_increment(map_iterator); // key_five
		CAP_ASSERT_EQ(*(int *)map_iterator->key, key_five,
			      "MAP Iterator remove check");
		int key_seven = 7;
		float value_seven = 70.0f;
		size_t _tmp_current_index = map_iterator->_current_index;
		cap_map_insert(map, &key_seven, &value_seven);
		CAP_ASSERT_TRUE(map_iterator->_current_index ==
					_tmp_current_index,
				"MAP Iterator insert");
		cap_map_free(map);
		cap_map_iterator_free(map_iterator);
	}
}
