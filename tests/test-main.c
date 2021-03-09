#include <stdio.h>
#include "internal/test-helper.h"
extern void test_dynamic_queue(void);
extern void test_fixed_queue(void);
extern void test_list(void);

int main(int argc, const char*const argv[]){
	test_dynamic_queue();
	test_fixed_queue();
	test_list();
	
	return 0;
}
