cmake_minimum_required(VERSION 3.28)
project(jftt_compiler)

set(CMAKE_CXX_STANDARD 20)
set(COMPILER_DIR ${CMAKE_CURRENT_BINARY_DIR}/compiler)


find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

FLEX_TARGET(Scanner compiler/lexer.l ../compiler/lexer.cpp)
BISON_TARGET(Parser compiler/parser.y ../compiler/parser.cpp
        DEFINES_FILE ../compiler/parser.h)
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

#add_library(
#)
#target_include_directories(calc_utils_headers INTERFACE $(CMAKE_CURRENT_SOURCE_DIR))

add_executable(compiler
        ${BISON_Parser_OUTPUTS}
        ../compiler/lexer.cpp
        ${FLEX_Scanner_OUTPUTS}
        ../compiler/parser.cpp
        ../compiler/parser.h
        compiler/parser_utils.cpp
        compiler/parser_utils.h
)

#target_link_libraries(calc m)
