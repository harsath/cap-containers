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
)
add_executable(
	${PROJECT_NAME}
	${SOURCE_FILES}
)
