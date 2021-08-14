#include "internal/test-helper.h"
#include <hash_table_linear_probing.h>

bool compare_fn_one(void *one, void *two) {
	return ((*(int *)one) == (*(int *)two));
}

void test_hash_table_linear_probing(void) {
	{ // Key-type: int; value type: ANY;
		cap_lp_hash_table *table =
		    cap_lp_hash_table_init(sizeof(int), compare_fn_one, NULL);
		CAP_ASSERT_EQ(table->size, 0,
			      "HASHTABLE_LP size after first init");
		int key_one = 10;
		float value_one = 10.10f;
		cap_lp_hash_table_insert(table, &key_one, &value_one);
		CAP_ASSERT_TRUE(
		    cap_lp_hash_table_size(table) == 1 &&
			cap_lp_hash_table_contains(table, &key_one),
		    "HASHTABLE_LP size and contains check after first insert");
		CAP_ASSERT_EQ(
		    *(float *)cap_lp_hash_table_lookup(table, &key_one),
		    value_one, "HASHTABLE_LP lookup after first insert");
		int key_two = 20;
		float value_two = 20.20f;
		int key_three = 30;
		float value_three = 30.30f;
		int key_four = 40;
		float value_four = 40.40f;
		int key_five = 50;
		float value_five = 50.50f;
		cap_lp_hash_table_insert(table, &key_two, &value_two);
		cap_lp_hash_table_insert(table, &key_three, &value_three);
		CAP_ASSERT_TRUE(
		    cap_lp_hash_table_contains(table, &key_two) &&
			cap_lp_hash_table_contains(table, &key_one) &&
			*(float *)cap_lp_hash_table_lookup(table, &key_two) ==
			    value_two &&
			*(float *)cap_lp_hash_table_lookup(table, &key_one) ==
			    value_one &&
			cap_lp_hash_table_size(table) == 3 &&
			!cap_lp_hash_table_contains(table, &key_five),
		    "HASHTABLE_LP checks after three inserts and rehash");
		cap_lp_hash_table_insert(table, &key_four, &value_four);
		cap_lp_hash_table_insert(table, &key_five, &value_five);
		int key_six = 60;
		float value_six = 60.60f;
		int key_seven = 70;
		float value_seven = 70.70f;
		cap_lp_hash_table_insert(table, &key_six, &value_six);
		cap_lp_hash_table_insert(table, &key_seven, &value_seven);
		CAP_ASSERT_TRUE(
		    cap_lp_hash_table_erase(table, &key_four) &&
			!cap_lp_hash_table_erase(table, &key_four) &&
			cap_lp_hash_table_size(table) == 6,
		    "HASHTABLE_LP erase key and re-erase check");
		cap_lp_hash_table_free(table);
	}
}
