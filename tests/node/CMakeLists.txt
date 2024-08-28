
cmake_minimum_required(VERSION 3.3)

# Projects name
project(Spyke_Node)

# Source files
add_executable(Spyke_Node 
  ${PROJECT_SOURCE_DIR}/node/node.cpp

  ${PROJECT_SOURCE_DIR}/tests/node/main.cpp
)

target_include_directories(Spyke_Node
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/node
)
