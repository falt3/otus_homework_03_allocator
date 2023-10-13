cmake_minimum_required(VERSION 3.2)


set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(allocator VERSION ${PROJECT_VESRION})


set(nameApp allocator)
add_executable(${nameApp} main.cpp)


set_target_properties(
    ${nameApp} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(
    ${nameApp} PRIVATE
    -Wall -Wextra -pedantic -Werror
)


add_subdirectory(test)
target_include_directories(test_allocator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(
    NAME test_allocator 
    COMMAND test_allocator
)


install(TARGETS ${nameApp} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

enable_testing()
