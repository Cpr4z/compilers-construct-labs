project(Tests)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} tests.cpp)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::unit_test_framework)
#TO DO
target_link_libraries(${PROJECT_NAME} PRIVATE /Users/matvey_agarkov/Desktop/compilers-construct-labs/lab1/src/cmake-build-debug/libmyLib.a)
#target_link_libraries(${PROJECT_NAME} PRIVATE myLib)

enable_testing()
add_test(NAME lab1Tests COMMAND ${PROJECT_NAME})