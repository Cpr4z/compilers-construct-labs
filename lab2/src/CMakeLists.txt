cmake_minimum_required(VERSION 3.29)
project(lab2)

set(CMAKE_CXX_STANDARD 23)

#add_subdirectory(graph)
#add_subdirectory(grammar)
#add_subdirectory(types)
#add_subdirectory(utils)

set (HEADERS
        grammar/grammar.hpp
        graph/graph.hpp
        types/types.hpp
        types/alphabet.hpp
        types/graph.hpp
        utils/io_utils.hpp
        utils/setUtils.hpp
        utils/grammarIOAdapter.hpp
)

set (SOURCES
        grammar/grammar.cpp
        grammar/contextFreeGrammar.cpp
        graph/graph.cpp
        utils/grammarIOAdapter.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} main.cpp)


target_include_directories( ${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/grammar
        ${CMAKE_CURRENT_SOURCE_DIR}/graph
        ${CMAKE_CURRENT_SOURCE_DIR}/types
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(${PROJECT_NAME} INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:
        -Werror;
        -Wall;
        -Wextra;
        -Wpedantic-Wcast-align;
        -Wcast-qual;
        -Wconversion;
        -Wctor-dtor-privacy;
        -Wenum-compare;
        -Wextra-semi;
        -Wfloat-equal;
        -Wnon-virtual-dtor;
        -Wold-style-cast;
        -Woverloaded-virtual;
        -Wredundant-decls;
        -Wshadow;
        -Wsign-conversion;
        -Wsign-promo;
        -Wzero-as-null-pointer-constant>>"
        "$<${msvc_cxx}:$<BUILD_INTERFACE:-W4>>"
)
