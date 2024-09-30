
cmake_minimum_required(VERSION 3.3)

# Projects name
project(Spyke_Node)

set( CMAKE_CXX_FLAGS  "-g -fsanitize=address")

# Conditionally define a preprocessor directive based on the option
if(SECOND_NODE_SETTINGS)

  add_definitions(-DSECOND_NODE_SETTINGS)

endif()

# Source files
add_executable(Spyke_Node 
  ${PROJECT_SOURCE_DIR}/node/connection.cpp
  ${PROJECT_SOURCE_DIR}/node/socket_helper.cpp
  ${PROJECT_SOURCE_DIR}/node/node_connection_manager.cpp
  ${PROJECT_SOURCE_DIR}/node/node.cpp
  ${PROJECT_SOURCE_DIR}/node/signal_handling.cpp

  ${PROJECT_SOURCE_DIR}/tests/node/main.cpp
)

target_include_directories(Spyke_Node
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include/node
)


if(SECOND_NODE_SETTINGS)

  set_target_properties(Spyke_Node PROPERTIES OUTPUT_NAME "Spyke_Node_2")

endif()
