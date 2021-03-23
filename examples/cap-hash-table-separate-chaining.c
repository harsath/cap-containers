#include <stdio.h>
#include "hash_table_separate_chaining.h"

int main(int argc, const char *argv[]) {
	// Please Take a look at 'cap-containers/hash_table_separate_chaining.h' source file for all interface	
	
	//						 Key-Size,  Initial bucket size/capacity
	cap_hash_table *hash_table = cap_hash_table_init(sizeof(int), 5);
	int key_one = 12, key_two = 23, key_three = 34, key_four = 45, key_five = 56, key_six = 67;
	float val_one = 10.0f, val_two = 20.2f, val_three = 30.3, val_four = 40.4f, val_five = 50.5, val_six = 60.6f;

	if(cap_hash_table_bucket_size(hash_table) == 5)
	{ printf("Initial bucket size 5, expected\n"); }

	bool insert_status = cap_hash_table_insert(hash_table, &key_one, &val_one);

	if(insert_status)
	{ printf("val_one has been inserted\n"); }

	cap_hash_table_insert(hash_table, &key_two, &val_two);
	
	if(cap_hash_table_size(hash_table) == 2)
	{ printf("There are two items in the hash table\n"); }

	cap_hash_table_insert(hash_table, &key_three, &val_three);

	float *val_two_ptr = cap_hash_table_lookup(hash_table, &key_two);

	if(val_two_ptr != NULL && *val_two_ptr == val_two)
	{ printf("val_two is in the hash table and lookup is success\n"); }

	int invalid_key = 100;
	float *undefined_value_lookup = cap_hash_table_lookup(hash_table, &invalid_key);

	if(undefined_value_lookup == NULL)
	{ printf("hash-table lookup failed, there is no such key found in the hash table\n"); }

	bool erase_status = cap_hash_table_erase(hash_table, &key_one);
	if(erase_status)
	{ printf("key_one has been removed from the hash table\n"); }

	bool failed_erase_status = cap_hash_table_erase(hash_table, &invalid_key);
	if(!failed_erase_status)
	{ printf("removing invalid_key failed, there is no such key in hash-table\n"); }

	cap_hash_table_insert(hash_table, &key_one, &val_one);
	cap_hash_table_insert(hash_table, &key_four, &val_four);
	cap_hash_table_insert(hash_table, &key_five, &val_five);
	cap_hash_table_insert(hash_table, &key_six, &val_six);

	if(cap_hash_table_bucket_size(hash_table) == 10)
	{ printf("Hash table's bucket size is increased and items are re-hashed\n"); }

	bool value_five_contains_status = cap_hash_table_contains(hash_table, &key_five);
	if(value_five_contains_status)
	{ printf("Key with key_five contains in table\n"); }

	cap_hash_table_free(hash_table);

	/* Other supported APIs:
	 * 	-> bool cap_hash_table_deep_erase(cap_hash_table*, void* key);
	 * 		Remove the element 'key' in the hash-table and also free() the element's memory.
	 *	-> bool cap_hash_table_empty(cap_hash_table*);
	 *		Is the hash-table empty? 'true' if yes, 'false' if not.
	 *	-> void cap_hash_table_swap(cap_hash_table*, cap_hash_table*);
	 *		Swap two 'cap_hash_table' objects.
	 *	-> void cap_hash_table_deep_free(cap_hash_table*);
	 *		Free the 'cap_hash_table' object and also the underlying element's memory.
	 */
}
