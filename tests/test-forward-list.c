#include <forward_list.h>
#include "internal/test-helper.h"

bool flist_predicate_fn_one(void* data){
	return *(int*)data == 20;
}

bool flist_predicate_fn_two(void* data){
	return *(int*)data == 40;
}

bool flist_invalid_predicate_fn(void* data){
	(void)data;
	return false;
}

void test_forward_list(void){
	{
		cap_forward_list* list_one = cap_forward_list_init();
		CAP_ASSERT_TRUE(cap_forward_list_empty(list_one) &&
				cap_forward_list_size(list_one) == 0, "FORWARD_LIST empty & size");
		int one = 10;
		// [10]
		cap_forward_list_push_front(list_one, &one);
		CAP_ASSERT_TRUE(!cap_forward_list_empty(list_one) &&
				*(int*)cap_forward_list_front(list_one) == one &&
				cap_forward_list_size(list_one) == 1, "FORWARD_LIST empty & front & size after init");
		int two = 20;
		// [20] -> [10]
		CAP_ASSERT_TRUE(cap_forward_list_push_front(list_one, &two), "FORWARD_LIST push_front");
		// [10]
		CAP_ASSERT_TRUE(cap_forward_list_size(list_one) == 2 &&
				*(int*)cap_forward_list_front(list_one) == two &&
				*(int*)cap_forward_list_pop_front(list_one) == two &&
				*(int*)cap_forward_list_front(list_one) == one &&
				cap_forward_list_size(list_one) == 1, "FORWARD_LIST pop_front & front");
		int three = 30;
		int four = 40;
		// [40] -> [30] -> [20] -> [10]
		cap_forward_list_push_front(list_one, &two);
		cap_forward_list_push_front(list_one, &three);
		cap_forward_list_push_front(list_one, &four);
		int* ptr_two = cap_forward_list_find_if(list_one, flist_predicate_fn_one);
		CAP_ASSERT_TRUE(ptr_two != NULL && *(int*)ptr_two == two, "FORWARD_LIST find_if 20");
		CAP_ASSERT_TRUE(cap_forward_list_size(list_one) == 4 &&
				*(int*)cap_forward_list_front(list_one) == four, "FORWARD_LIST size & front after find_if");
		int* invalid_ptr = cap_forward_list_find_if(list_one, flist_invalid_predicate_fn);
		CAP_ASSERT_TRUE(invalid_ptr == NULL, "FORWARD_LIST invalid ptr find_if");
		// [40] -> [30] -> [10]
		bool remove_if = cap_forward_list_remove_if(list_one, flist_predicate_fn_one);
		CAP_ASSERT_EQ(*(int*)cap_forward_list_front(list_one), four, "FORWARD_LIST front after remove");
		CAP_ASSERT_TRUE(remove_if && 
				cap_forward_list_size(list_one) == 3 &&
				*(int*)cap_forward_list_front(list_one) == four, "FORWARD_LIST remove_if");
		// [30] -> [10]
		bool remove_if_2 = cap_forward_list_remove_if(list_one, flist_predicate_fn_two);
		CAP_ASSERT_TRUE(remove_if_2 &&
				*(int*)cap_forward_list_front(list_one) == three &&
				cap_forward_list_size(list_one) == 2, "FORWARD_LIST remove_if on head node");
		cap_forward_list* list_two = cap_forward_list_init();
		cap_forward_list_push_front(list_two, &two);
		cap_forward_list_swap(list_one, list_two);
		CAP_ASSERT_TRUE(*(int*)cap_forward_list_front(list_one) == two &&
				cap_forward_list_size(list_one) == 1 &&
				*(int*)cap_forward_list_front(list_two) == three &&
				cap_forward_list_size(list_two) == 2, "FORWARD_LIST swap");
	}
}
