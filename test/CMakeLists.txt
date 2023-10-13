
add_executable(test_allocator test.cpp)

find_package(GTest)
target_include_directories(test_allocator PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(test_allocator PRIVATE ${GTEST_BOTH_LIBRARIES})

add_test(
    NAME test1 
    COMMAND test_allocator
)