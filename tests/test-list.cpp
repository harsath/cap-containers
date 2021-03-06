#include <gtest/gtest.h>
#include <list.h>

typedef struct { char user_name[100]; char user_addres[200]; } user_info_sample;
bool predicate_one(void* value){
	if(*(int*)value == 20){ return true; }
	return false;
}

bool predicate_two(void* value){
	if(strcmp(((user_info_sample*)value)->user_name, "John") == 0){ return true; }
	return false;
}

TEST(Cap_Containers, Cap_Containers_list){
	{
		cap_list* d_list_one = cap_list_init();
		int value_one = 10; // [head] [10] [tail]
		cap_list_push_back(d_list_one, &value_one);
		ASSERT_EQ(*((int*)cap_list_front(d_list_one)), value_one);
		ASSERT_EQ(*((int*)cap_list_back(d_list_one)), value_one);

		int value_two = 20; // [head] [20] [10] [tail]
		cap_list_push_front(d_list_one, &value_two);
		ASSERT_EQ(*((int*)cap_list_front(d_list_one)), value_two);
		ASSERT_EQ(*((int*)cap_list_back(d_list_one)), value_one);

		int value_three = 30; // [head] [30] [20] [10] [tail]
		cap_list_push_front(d_list_one, &value_three);
		ASSERT_EQ(*((int*)cap_list_front(d_list_one)), value_three);
		ASSERT_EQ(*((int*)cap_list_back(d_list_one)), value_one);

		ASSERT_EQ(*((int*)cap_list_pop_back(d_list_one)), value_one); // [head] [30] [20] [tail]
		ASSERT_EQ(*((int*)cap_list_back(d_list_one)), value_two);

		int value_four = 40; // [head] [30] [20] [40] [tail]
		cap_list_push_back(d_list_one, &value_four);
		void* find_node_value_two = cap_list_find_if(d_list_one, predicate_one);
		ASSERT_TRUE((find_node_value_two != NULL) && (*(int*)find_node_value_two == value_two));
		ASSERT_TRUE(cap_list_remove_if(d_list_one, predicate_one)); // [head] [30] [40] [tail]
		ASSERT_TRUE((*(int*)cap_list_back(d_list_one) == value_three) == (*(int*)cap_list_front(d_list_one) == value_four));
		cap_list_free(d_list_one);
	}

	{
		user_info_sample* value_one = (user_info_sample*)malloc(sizeof(user_info_sample));
		user_info_sample* value_two = (user_info_sample*)malloc(sizeof(user_info_sample));
		const char* one_user_name = "John"; const char* one_user_address = "400 Kennedy, New York, USA";
		const char* two_user_name = "Matt"; const char* two_user_address = "955 Aachen, Berlin, Germany";
		strncpy(value_one->user_name, one_user_name, strlen(one_user_name));
		strncpy(value_one->user_addres, one_user_address, strlen(one_user_address));
		strncpy(value_two->user_name, two_user_name, strlen(two_user_name));
		strncpy(value_two->user_addres, two_user_address, strlen(two_user_address));

		cap_list* d_list_two = cap_list_init();
		cap_list_push_back(d_list_two, value_one); // [head] [value_one] [tail]
		cap_list_push_front(d_list_two, value_two); // [head] [value_two] [value_one] [tail]
		user_info_sample* find_if_predicate_two = (user_info_sample*)cap_list_find_if(d_list_two, predicate_two);
		ASSERT_TRUE(find_if_predicate_two != NULL);
		bool find_if_predicate_two_is_address_correct = 
			strcmp(one_user_address, find_if_predicate_two->user_addres) == 0 ? true : false;
		ASSERT_TRUE(find_if_predicate_two_is_address_correct);
		cap_list_remove_if(d_list_two, predicate_two);
		ASSERT_TRUE(cap_list_find_if(d_list_two, predicate_two) == NULL);
		cap_list_deep_free(d_list_two);	
	}
}
