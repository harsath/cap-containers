#include "internal/test-helper.h"
#include <hash_table_separate_chaining.h>
#define _DECLARE_AND_INIT(variable_name, value)                                \
	char variable_name[10];                                                \
	strncpy(variable_name, value, strlen(value))

void test_hash_table_separate_chain(void) {
	{ // Key-type: int; value type: ANY;
		cap_hash_table *hash_table =
		    cap_hash_table_init(sizeof(int), 5);
		CAP_ASSERT_TRUE(cap_hash_table_empty(hash_table),
				"HASHTABLE_SP empty after init");
		CAP_ASSERT_TRUE(cap_hash_table_size(hash_table) == 0,
				"HASHTABLE_SP size after init");
		int key_one = 1;
		float value_one = 10.01f;
		int key_two = 2;
		float value_two = 20.02f;
		float value_one_replace = 11.11f;
		cap_hash_table_insert(hash_table, &key_one, &value_one);
		cap_hash_table_insert(hash_table, &key_two, &value_two);
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_two) == value_two,
				"HASHTABLE_SP lookup after init with 2 items");
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_one) == value_one,
				"HASHTABLE_SP lookup key-one");
		CAP_ASSERT_FALSE(cap_hash_table_empty(hash_table),
				 "HASHTABLE_SP empty after inserts");
		cap_hash_table_insert(hash_table, &key_one, &value_one_replace);
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_one) == value_one_replace,
				"HASHTABLE_SP lookup key-one after replaced");
		CAP_ASSERT_TRUE(cap_hash_table_contains(hash_table, &key_one),
				"HASHTABLE_SP contains on valid key");
		int invalid_key = 999;
		CAP_ASSERT_FALSE(
		    cap_hash_table_contains(hash_table, &invalid_key),
		    "HASHTABLE_SP conains on invalid key");
		CAP_ASSERT_TRUE(
		    cap_hash_table_lookup(hash_table, &invalid_key) == NULL,
		    "HASHTABLE_SP lookup on invalid key");
		CAP_ASSERT_TRUE(cap_hash_table_size(hash_table) == 2,
				"HASHTABLE_SP size before rehash");
		int key_three = 3;
		float value_three = 30.03f;
		int key_four = 4;
		float value_four = 40.04f;
		int key_five = 5;
		float value_five = 50.05f;
		int key_six = 6;
		float value_six = 60.06f;
		CAP_ASSERT_TRUE(cap_hash_table_bucket_size(hash_table) == 5,
				"HASHTABLE_SP bucket-size before insert & "
				"rehash triggered");
		cap_hash_table_insert(hash_table, &key_three, &value_three);
		cap_hash_table_insert(hash_table, &key_four, &value_four);
		cap_hash_table_insert(hash_table, &key_five, &value_five);
		cap_hash_table_insert(hash_table, &key_six, &value_six);
		CAP_ASSERT_TRUE(
		    cap_hash_table_bucket_size(hash_table) == 10,
		    "HASHTABLE_SP bucket-size after insert & rehash triggered");
		CAP_ASSERT_TRUE(cap_hash_table_size(hash_table),
				"HASHTABLE_SP size after rehash");
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_four) == value_four,
				"HASHTABLE_SP lookup before erase");
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_five) == value_five,
				"HASHTABLE_SP lookup value_five");
		bool erase_return = cap_hash_table_erase(hash_table, &key_four);
		CAP_ASSERT_TRUE(erase_return,
				"HASHTABLE_SP erase item with key=key_four");
		CAP_ASSERT_TRUE(cap_hash_table_lookup(hash_table, &key_four) ==
				    NULL,
				"HASHTABLE_SP lookup after erase");
		CAP_ASSERT_TRUE(
		    *(float *)cap_hash_table_lookup(hash_table, &key_five) ==
			value_five,
		    "HASHTABLE_SP lookup after erase of another item");
		CAP_ASSERT_TRUE(cap_hash_table_size(hash_table) == 5,
				"HASHTABLE_SP size after erase");
		CAP_ASSERT_FALSE(cap_hash_table_contains(hash_table, &key_four),
				 "HASHTABLE_SP contains on removed key");
		CAP_ASSERT_TRUE(cap_hash_table_contains(hash_table, &key_one),
				"HASHTABLE_SP contains on non-removed key");
		CAP_ASSERT_FALSE(cap_hash_table_erase(hash_table, &key_four),
				 "HASHTABLE_SP erase on removed key");
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_five) == value_five,
				"HASHTABLE_SP lookup before replace");
		float new_value_five = 77.00f;
		cap_hash_table_insert(hash_table, &key_five, &new_value_five);
		CAP_ASSERT_TRUE(*(float *)cap_hash_table_lookup(
				    hash_table, &key_five) == new_value_five,
				"HASHTABLE_SP lookup after replace");
		cap_hash_table_free(hash_table);
	}

	{ // Key-type: char[10]; value: char*;
		_DECLARE_AND_INIT(key_one, "user-agent");
		_DECLARE_AND_INIT(key_two, "cookie");
		_DECLARE_AND_INIT(key_three, "host");
		_DECLARE_AND_INIT(key_four, "PHPSESSID");
		_DECLARE_AND_INIT(key_five, "Powered-By");
		char *value_one = "curl 12/32.32";
		char *value_two = "some_cook_val=21; somemore_val=3232;";
		char *value_three = "www.foo.com";
		char *value_four = "SOME_PHP_SESSION_ID_VALUE";
		char *value_five = "Proxygen/FB-CXX";
		size_t key_size = 10;
		size_t initial_capacity = 3;
		cap_hash_table *hash_table_http_headers =
		    cap_hash_table_init(key_size, initial_capacity);
		cap_hash_table_insert(hash_table_http_headers, key_one,
				      value_one);
		cap_hash_table_insert(hash_table_http_headers, key_two,
				      value_two);
		cap_hash_table_insert(hash_table_http_headers, key_three,
				      value_three);
		cap_hash_table_insert(hash_table_http_headers, key_four,
				      value_four);
		cap_hash_table_insert(hash_table_http_headers, key_five,
				      value_five);
		CAP_ASSERT_TRUE(
		    cap_hash_table_contains(hash_table_http_headers, key_three),
		    "HASHTABLE_SP contains after insert");
		CAP_ASSERT_TRUE(memcmp(cap_hash_table_lookup(
					   hash_table_http_headers, key_four),
				       value_four, strlen(value_four)) == 0,
				"HASHTABLE_SP lookup key four / PHPSESSID");
		char *invalid_key = "foobar_invalid";
		CAP_ASSERT_TRUE(cap_hash_table_lookup(hash_table_http_headers,
						      invalid_key) == NULL,
				"HASHTABLE_SP invalid key check");
		CAP_ASSERT_TRUE(
		    cap_hash_table_erase(hash_table_http_headers, key_three),
		    "HASHTABLE_SP erase key-three / host");
		CAP_ASSERT_FALSE(
		    cap_hash_table_erase(hash_table_http_headers, invalid_key),
		    "HASHTABLE_SP erase on invalid key");
		CAP_ASSERT_TRUE(cap_hash_table_lookup(hash_table_http_headers,
						      key_three) == NULL,
				"HASHTABLE_SP lookup key after erase");
		CAP_ASSERT_TRUE(memcmp(cap_hash_table_lookup(
					   hash_table_http_headers, key_four),
				       value_four, strlen(value_four)) == 0,
				"HASHTABLE_SP lookup value four 'P'");
		CAP_ASSERT_TRUE(memcmp(cap_hash_table_lookup(
					   hash_table_http_headers, key_five),
				       value_five, strlen(value_five)) == 0,
				"HASHTABLE_SP lookup for value five 'P'");
		_DECLARE_AND_INIT(key_six, "Proxied-By");
		char *value_six = "Java Anon Proxy/1.50";
		_DECLARE_AND_INIT(key_seven, "Ray-ID");
		char *value_seven = "SOME_LB_RAY_ID";
		cap_hash_table_insert(hash_table_http_headers, key_six,
				      value_six);
		cap_hash_table_insert(hash_table_http_headers, key_seven,
				      value_seven);
		cap_hash_table_insert(hash_table_http_headers, key_three,
				      value_three);
		CAP_ASSERT_TRUE(
		    cap_hash_table_bucket_size(hash_table_http_headers) == 12,
		    "HASHTABLE_SP bucker-size after 2x rehashing");
		cap_hash_table_free(hash_table_http_headers);
	}
	{ // deep_free test
		int *key_one = malloc(sizeof(int));
		*key_one = 10;
		int *key_two = malloc(sizeof(int));
		*key_two = 20;
		char *value_one = calloc(1, 10);
		strncpy(value_one, "value_one", strlen("value_one"));
		char *value_two = calloc(1, 10);
		strncpy(value_two, "value_two", strlen("value_two"));
		cap_hash_table *hash_table_heap_alloc =
		    cap_hash_table_init(sizeof(int), 10);
		cap_hash_table_insert(hash_table_heap_alloc, key_one,
				      value_one);
		cap_hash_table_insert(hash_table_heap_alloc, key_two,
				      value_two);
		cap_hash_table_deep_erase(hash_table_heap_alloc, key_two);
		char *value_two_re = calloc(1, 10);
		strncpy(value_two_re, "value_two", strlen("value_two"));
		cap_hash_table_insert(hash_table_heap_alloc, key_two,
				      value_two_re);
		cap_hash_table_deep_free(hash_table_heap_alloc);
	}
}
