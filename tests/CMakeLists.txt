cmake_minimum_required(VERSION 3.10)
project(
	cap_containers_test
	LANGUAGES C CXX
	DESCRIPTION "Pure C Containers test"
)
include_directories(../cap-containers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
	SOURCE_FILES
	test-fixed-queue.cpp
)
add_subdirectory(vendor/googletest)
add_executable(
	${PROJECT_NAME}
	${SOURCE_FILES}
)
target_link_libraries(
	${PROJECT_NAME}
	gtest_main
)
