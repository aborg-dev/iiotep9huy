cmake_minimum_required (VERSION 2.6)

project (oberon CXX)

set (CMAKE_CXX_COMPILER g++)

add_definitions (-std=c++0x -O3 -lm -lfl -I../)

add_custom_command(
  OUTPUT test.tab.cpp
  COMMAND bison -dv ${CMAKE_CURRENT_SOURCE_DIR}/test.ypp
  COMMENT "Bison" 
)

add_custom_command(
  OUTPUT lex.yy.cc
  COMMAND flex++ --c++ ${CMAKE_CURRENT_SOURCE_DIR}/test.l
  COMMENT "Flex" 
)

add_library (expression Expression.cpp)
add_library (selector Selector.cpp)
add_library (context Context.cpp)
add_library (type Type.cpp)
add_library (procedure Procedure.cpp)
add_library (statement Statement.cpp)
add_library (declarations Declarations.cpp)
add_library (value Value.cpp)

add_executable (oberon test.tab.cpp lex.yy.cc)
target_link_libraries(oberon expression selector context type procedure statement declarations value)
