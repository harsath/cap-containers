#include "caches/lru_cache.h"
#include "internal/test-helper.h"
#define _cast_value_float(item) *((float*)item)
#define _cast_value_int(item) *((int*)item)

void test_lru_cache(void){
	{ // Test internal d-linked list helper
		_cap_list *d_list = _cap_list_init(sizeof(float));
		CAP_ASSERT_TRUE(d_list->_key_size == sizeof(float) && d_list->_size == 0 &&
				d_list->_head_node->next == d_list->_tail_node && d_list->_head_node->prev == NULL &&
				d_list->_tail_node->prev == d_list->_head_node, "LRU_CACHE after init");
		float key_one = 10.10f;
		int value_one = 100;
		_cap_list_push_front(d_list, &key_one, &value_one, sizeof(value_one));
		CAP_ASSERT_TRUE(*((int*)d_list->_head_node->next->data) == value_one && 
			       (*(float*)d_list->_head_node->next->key) == key_one &&
			       d_list->_size == 1, "LRU_CACHE first node key & value");
		CAP_ASSERT_TRUE(*((int*)_cap_list_front(d_list)->data) == value_one && 
			       (*(float*)_cap_list_front(d_list)->key) == key_one &&
			       d_list->_size == 1, "LRU_CACHE front");
		CAP_ASSERT_TRUE(*((int*)_cap_list_back(d_list)->data) == value_one && 
			       (*(float*)_cap_list_back(d_list)->key) == key_one &&
			       d_list->_size == 1, "LRU_CACHE back");
		float key_two = 20.20f;
		int value_two = 200;
		_cap_list_push_front(d_list, &key_two, &value_two, sizeof(value_two));
		CAP_ASSERT_TRUE(*((int*)d_list->_head_node->next->data) == value_two && 
			       (*(float*)d_list->_head_node->next->key) == key_two &&
			       d_list->_size == 2, "LRU_CACHE first node key & value after node two");
		CAP_ASSERT_TRUE(*((int*)_cap_list_front(d_list)->data) == value_two && 
			       (*(float*)_cap_list_front(d_list)->key) == key_two &&
			       d_list->_size == 2, "LRU_CACHE front after node two");
		CAP_ASSERT_TRUE(*((int*)_cap_list_back(d_list)->data) == value_one && 
			       (*(float*)_cap_list_back(d_list)->key) == key_one &&
			       d_list->_size == 2, "LRU_CACHE back after node two");
		float key_three = 30.30f;
		int value_three = 300;
		_cap_list_push_front(d_list, &key_three, &value_three, sizeof(value_three));
		// [HEAD] <-> [1] <-> [THIS_NODE] <-> [3] <-> [TAIL]
		// In public interface we will have a hash-table of these pointers, so don't worry about this. It's just for testing impl
		_cap_list_node *node_to_move_front = d_list->_head_node->next->next;
		CAP_ASSERT_TRUE(*((int*)_cap_list_front(d_list)->data) == value_three && 
			       (*(float*)_cap_list_front(d_list)->key) == key_three &&
			       d_list->_size == 3, "LRU_CACHE front before move_to_front");
		_cap_list_move_front(d_list, node_to_move_front);
		CAP_ASSERT_TRUE(*((int*)_cap_list_front(d_list)->data) == value_two && 
			       (*(float*)_cap_list_front(d_list)->key) == key_two &&
			       d_list->_size == 3, "LRU_CACHE front after move_to_front");
		CAP_ASSERT_TRUE(*((int*)_cap_list_back(d_list)->data) == value_one && 
			       (*(float*)_cap_list_back(d_list)->key) == key_one &&
			       d_list->_size == 3, "LRU_CACHE back after move_to_front");
		_cap_list_node *pop_node = _cap_list_pop_back(d_list);
		CAP_ASSERT_TRUE(*((int*)pop_node->data) == value_one &&
				*((float*)pop_node->key) == key_one && 
				d_list->_size == 2, "LRU_CACHE pop_node");
		CAP_ASSERT_TRUE(*((int*)_cap_list_front(d_list)->data) == value_two && 
			       (*(float*)_cap_list_front(d_list)->key) == key_two &&
			       d_list->_size == 2, "LRU_CACHE front after pop");
		CAP_ASSERT_TRUE(*((int*)_cap_list_back(d_list)->data) == value_three && 
			       (*(float*)_cap_list_back(d_list)->key) == key_three &&
			       d_list->_size == 2, "LRU_CACHE back after pop");
		float key_four = 40.40f;
		int value_four = 400;
		float key_five = 50.50f;
		int value_five = 500;
		// [HEAD] <-> [FIVE] <-> [FOUR] <-> [TWO] <-> [THREE] <-> [TAIL]
		_cap_list_push_front(d_list, &key_four, &value_four, sizeof(value_four));
		_cap_list_push_front(d_list, &key_five, &value_five, sizeof(value_five));
		_cap_list_node *two_node = d_list->_head_node->next->next->next;
		_cap_list_move_front(d_list, two_node);
		// [HEAD] <-> [TWO] <-> [FIVE] <-> [FOUR] <-> [THREE] <-> [TAIL]
		_cap_list_node *five_node = d_list->_head_node->next->next;
		CAP_ASSERT_TRUE(
			_cast_value_float(d_list->_head_node->next->key) == key_two &&
			_cast_value_float(d_list->_head_node->next->next->key) == key_five &&
			_cast_value_float(d_list->_head_node->next->next->next->key) == key_four &&
			_cast_value_float(d_list->_head_node->next->next->next->next->key) == key_three &&
			d_list->_head_node->next->next->next->next->next->key == NULL &&
			d_list->_size == 4,
			"LRU_CACHE before_remove_this"
		);
		CAP_ASSERT_TRUE(*((float*)_cap_list_front(d_list)->key) == key_two,
				"LRU_CACHE front after move four_node");
		// [HEAD] <-> [TWO] <-> [FOUR] <-> [THREE] <-> [TAIL]
		_cap_list_remove_this(d_list, five_node);
		CAP_ASSERT_TRUE(
				_cast_value_float(d_list->_head_node->next->key) == key_two &&
				_cast_value_float(d_list->_head_node->next->next->key) == key_four &&
				_cast_value_float(d_list->_head_node->next->next->next->key) == key_three &&
				d_list->_head_node->next->next->next->next->data == NULL &&
				d_list->_size == 3,
				"LRU_CACHE after_remove_this"
		);
	}
}
