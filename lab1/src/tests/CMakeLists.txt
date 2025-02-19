find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(tests
        tests.cpp
)

target_include_directories(tests PRIVATE ../include)
target_include_directories(tests PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(tests PRIVATE ${Boost_LIBRARIES})

enable_testing()
add_test(NAME lab1Tests COMMAND tests)