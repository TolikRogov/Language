cmake_minimum_required(VERSION 3.5)

project(ReverseFrontend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (DEBUG_FLAGS   -std=c++17 -ggdb3 -O3 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wno-format-nonliteral -Wformat-security -Wformat=2 -Winline -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wno-unused-parameter -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fPIC -fPIE -fno-omit-frame-pointer -fPIE -Werror=vla -Wno-write-strings -fsanitize=address,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
set (RELEASE_FLAGS -std=c++17)

set (DEBUG ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../IdNameTable)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../KeyWords)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Utils)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Tree)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Standard)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Lexer)

if (${DEBUG})
	add_compile_definitions(PRINT_DEBUG)
endif()

add_executable(${PROJECT_NAME})

add_subdirectory(source)

target_compile_options (${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_compile_options (${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)

target_link_options (${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:${DEBUG_FLAGS}>)
target_link_options (${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:${RELEASE_FLAGS}>)
