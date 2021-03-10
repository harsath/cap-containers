#include <vector.h>
#include "internal/test-helper.h"
#define _print_number(foo, val) printf("%s: %d\n", foo, val)
#define _print_all_numbers(vector, size)		\
	for(size_t i = 0; i < size; i++){	\
		printf("%d\n", *(int*)vector[i]);\
	}

bool predicate_fn_one(void* data){
	if(*(int*)data == 20){ return true; }
	else{ return false; }
}

bool predicate_fn_two(void* data){
	if(*(int*)data == 30){ return true; }
	else{ return false; }
}

void test_vector(void){
	{
		cap_vector* vector_one = cap_vector_init(5);
		CAP_ASSERT_TRUE(cap_vector_empty(vector_one), "VECTOR empty");
		int one = 10;
		// [10]
		cap_vector_push_back(vector_one, &one);
		CAP_ASSERT_TRUE(*(int*)cap_vector_front(vector_one) == one, "VECTOR front");
		CAP_ASSERT_TRUE(*(int*)cap_vector_back(vector_one) == one, "VECTOR back");
		int two = 20;
		// [10] [20]
		cap_vector_push_back(vector_one, &two);
		CAP_ASSERT_TRUE(*(int*)cap_vector_back(vector_one) == two && 
				*(int*)cap_vector_front(vector_one) == one && !cap_vector_empty(vector_one), 
				"VECTOR back, front, empty");
		CAP_ASSERT_EQ(cap_vector_size(vector_one), 2, "VECTOR size");
		CAP_ASSERT_EQ(cap_vector_capacity(vector_one), 5, "VECTOR capacity");
		CAP_ASSERT_EQ(cap_vector_remaining_space(vector_one), 3, "VECTOR remaining space");
		CAP_ASSERT_EQ(*(int*)cap_vector_pop_back(vector_one), two, "VETOR pop-back"); // [10]
		// [10] [20]
		cap_vector_push_back(vector_one, &two);
		int three = 30;
		// [10] [20] [30]
		cap_vector_push_back(vector_one, &three);
		void* find_two = cap_vector_find_if(vector_one, predicate_fn_one);
		CAP_ASSERT_TRUE(find_two != NULL, "VECTOR find_if return");
		CAP_ASSERT_TRUE(*(int*)find_two == two, "VECTOR find_if return value check");
		// [20] [30]
		CAP_ASSERT_EQ(*(int*)cap_vector_pop_front(vector_one), one, "VECTOR pop-front");
		CAP_ASSERT_EQ(cap_vector_size(vector_one), 2, "VECTOR size");
		CAP_ASSERT_EQ(*(int*)cap_vector_front(vector_one), two, "VECTOR front after pop");
		CAP_ASSERT_EQ(*(int*)cap_vector_back(vector_one), three, "VETOR back after pop");
		// [20] [30] [30]
		cap_vector_push_back(vector_one, &three);
		int four = 40;
		int five = 50;
		int six = 60;
		// [20] [30] [30] [40] [50] [60]
		cap_vector_push_back(vector_one, &four);
		cap_vector_push_back(vector_one, &five);
		cap_vector_push_back(vector_one, &six);
		CAP_ASSERT_EQ(cap_vector_capacity(vector_one), 10, "VECTOR capacity new");
		CAP_ASSERT_EQ(cap_vector_size(vector_one), 6, "VECTOR new size");
		// [20] [40] [50] [60]
		CAP_ASSERT_TRUE(cap_vector_remove_if(vector_one, predicate_fn_two), "VECTOR remove 30");
		CAP_ASSERT_TRUE(*(int*)cap_vector_front(vector_one) == two &&
				*(int*)cap_vector_back(vector_one) == six, "VECTOR front & back check");
		CAP_ASSERT_TRUE(cap_vector_size(vector_one) == 4, "VECTOR size after remove");
		CAP_ASSERT_EQ(cap_vector_capacity(vector_one), 10, "VECTOR capacity before resize");
		CAP_ASSERT_TRUE(cap_vector_resize(vector_one, 12), "VECTOR resize");
		CAP_ASSERT_EQ(cap_vector_capacity(vector_one), 12, "VECTOR after resize");
		CAP_ASSERT_TRUE(cap_vector_shrink_to_fit(vector_one), "VECTOR shrink to fit");
		CAP_ASSERT_TRUE(cap_vector_size(vector_one) == 4 && cap_vector_capacity(vector_one) == 4,
				"VECTOR size & capacity after shrink to fit");

		cap_vector* vector_two = cap_vector_init(30);
		int seven = 70;
		cap_vector_push_back(vector_two, &seven);
		cap_vector_push_back(vector_two, &six);
		cap_vector_push_back(vector_two, &five);
		cap_vector_push_back(vector_two, &four);
		cap_vector_push_back(vector_two, &three);
		cap_vector_push_back(vector_two, &two);
		cap_vector_push_back(vector_two, &one);
		CAP_ASSERT_TRUE(cap_vector_size(vector_two) == 7 && cap_vector_capacity(vector_two) == 30 &&
				*(int*)cap_vector_front(vector_two) == seven && *(int*)cap_vector_back(vector_two) == one,
				"VECTOR vector-two checks after init & push-back");
		cap_vector_swap(vector_one, vector_two);
		CAP_ASSERT_TRUE(cap_vector_size(vector_one) == 7 && cap_vector_capacity(vector_one) == 30 &&
				*(int*)cap_vector_front(vector_one) == seven && *(int*)cap_vector_back(vector_one) == one,
				"VECTOR vector-one checks after swap");
		CAP_ASSERT_TRUE(cap_vector_size(vector_two) == 4 && cap_vector_capacity(vector_two) == 4 &&
				*(int*)cap_vector_front(vector_two) == two && *(int*)cap_vector_back(vector_two) == six,
				"VECTOR vector-one checks after swap");
		cap_vector_free(vector_one);
		cap_vector_free(vector_two);
	}

	{ // Tests focused on Iterators
		cap_vector* vector = cap_vector_init(5);		
		int one = 10;
		int two = 20;
		int three = 30;
		int four = 40;
		int five = 50;
		int six = 60;
		cap_vector_push_back(vector, &one);
		cap_vector_push_back(vector, &two);
		cap_vector_push_back(vector, &three);
		cap_vector_push_back(vector, &four);
		cap_vector_push_back(vector, &five);
		cap_vector_push_back(vector, &six);
		cap_vector_iterator* vector_iterator = cap_vector_iterator_init(vector);
		CAP_ASSERT_TRUE(*(int*)vector_iterator->data == one, "VECTOR Iterator init check");
		cap_vector_iterator_decrement(vector_iterator);
		CAP_ASSERT_TRUE(vector_iterator->data == NULL, "VECTOR Iterator dec NULL check");
		cap_vector_iterator_increment(vector_iterator);
		CAP_ASSERT_TRUE(*(int*)vector_iterator->data == one, "VECTOR Iterator data check after error");
		cap_vector_iterator_increment(vector_iterator);
		CAP_ASSERT_TRUE(*(int*)vector_iterator->data == two, "VECTOR Iterator incr");
		cap_vector_iterator_increment(vector_iterator);
		cap_vector_iterator_decrement(vector_iterator);
		CAP_ASSERT_TRUE(*(int*)vector_iterator->data == two, "VECTOR Iterator incr");
		CAP_ASSERT_TRUE(*(int*)cap_vector_iterator_next(vector_iterator) == three &&
				*(int*)vector_iterator->data == two, "VECTOR Iterator next");
		CAP_ASSERT_TRUE(*(int*)cap_vector_iterator_previous(vector_iterator) == one &&
				*(int*)vector_iterator->data == two, "VECTOR Iterator previous");
		cap_vector_iterator* tmp_iter = cap_vector_iterator_init(vector);
		CAP_ASSERT_FALSE(cap_vector_iterator_equals(vector_iterator, tmp_iter), "VECTOR Iterator equals");
		cap_vector_iterator_increment(tmp_iter);
		CAP_ASSERT_TRUE(cap_vector_iterator_equals(vector_iterator, tmp_iter), "VECTOR Iterator equals");
		cap_vector_iterator_free(tmp_iter);
		CAP_ASSERT_TRUE(cap_vector_iterator_equals_predicate(vector_iterator, predicate_fn_one),
				"VECTOR Iterator true prediate");
		CAP_ASSERT_FALSE(cap_vector_iterator_equals_predicate(vector_iterator, predicate_fn_two),
				"VECTOR Iterator false predicate");
		cap_vector_iterator_increment(vector_iterator); // [30]
		CAP_ASSERT_EQ(cap_vector_iterator_index(vector_iterator), 2, "VECTOR Iterator index");
		cap_vector_iterator* tmp_begin_iter = cap_vector_begin(vector);
		cap_vector_iterator* tmp_end_iter = cap_vector_end(vector);
		CAP_ASSERT_TRUE(*(int*)tmp_end_iter->data == *(int*)cap_vector_back(vector)&&
				*(int*)tmp_begin_iter->data == *(int*)cap_vector_front(vector) &&
				tmp_end_iter->_current_index == cap_vector_size(vector)-1 &&
				tmp_begin_iter->_current_index == 0, "VECTOR Iterator begin & end checks");
		cap_vector_iterator_free(tmp_begin_iter);
		cap_vector_iterator_free(tmp_end_iter);
		CAP_ASSERT_EQ(*(int*)vector_iterator->data, three, "VECTOR Iterator before remove");
		cap_vector_iterator_remove(vector_iterator); // remove [30]
		CAP_ASSERT_EQ(*(int*)vector_iterator->data, four, "VECTOR Iterator remove check");
		cap_vector_iterator_increment(vector_iterator);
		CAP_ASSERT_EQ(*(int*)vector_iterator->data, five, "VECTOR Iterator remove check");
		int seven = 70;
		size_t _tmp_current_index = vector_iterator->_current_index;
		cap_vector_iterator_insert(vector_iterator, &seven);
		CAP_ASSERT_TRUE(*(int*)vector_iterator->data == seven &&
				vector_iterator->_current_index == _tmp_current_index,
				"VECTOR Iterator insert");
		cap_vector_free(vector);
		cap_vector_iterator_free(vector_iterator);
	}
}
