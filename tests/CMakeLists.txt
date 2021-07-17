cmake_minimum_required(VERSION 3.10)
project(
	cap_containers_test
	LANGUAGES C
	DESCRIPTION "Pure C Containers test"
)
include_directories(../cap-containers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
	SOURCE_FILES
	test-main.c
	test-fixed-queue.c
	test-list.c
	test-dynamic-queue.c
	test-vector.c
	test-stack.c
	test-forward-list.c
	test-hash-table-separate-chaining.c
	test-map.c
	test-circular-queue.c
	test-arena-allocator.c
	test-priority-queue.c
)
add_executable(
	${PROJECT_NAME}
	${SOURCE_FILES}
)
if(LOCAL_DEV)
	set(CMAKE_C_FLAGS "-Wall -O0 -g3 -ggdb -fno-omit-frame-pointer")
	set(CMAKE_CXX_FLAGS "-Wall -O0 -g3 -ggdb -fno-omit-frame-pointer")
elseif(LOCAL_DEV_ASAN)
	set(CMAKE_C_FLAGS "-Wall -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address")
	set(CMAKE_CXX_FLAGS "-Wall -O0 -g3 -ggdb -fno-omit-frame-pointer -fsanitize=address")
else()
	set(CMAKE_C_FLAGS "-O2 -Wall")
	set(CMAKE_CXX_FLAGS "-O2 -Wall")
endif()
