
#include "connection.h"
#include "node.h"
#include "node_connection_manager.h"
#include <csignal>
#include <unistd.h>

#ifndef SECOND_NODE_SETTINGS

node::Node_Connection_Manager_Settings< node::Ip_V4 > node_connection_manager_settings = {

  .max_stable_connections = 3,
  .max_temporary_connections = 3,
  .server_ip_information = ( node::Ip_V4 ) {

    .address = 0x7F000001,
    .port = 3000

  }

};

#else

node::Node_Connection_Manager_Settings< node::Ip_V4 > node_connection_manager_settings = {

  .max_stable_connections = 3,
  .max_temporary_connections = 3,
  .server_ip_information = ( node::Ip_V4 ) {

    .address = 0x7F000001,
    .port = 3002

  },

  .initial_ip_v4 = {

    ( node::Ip_V4 ) { .address = 0x7F000001, .port = 3000 }

  }

};

#endif

int main (int argc, char *argv[]) {

  const int sts = node::Node< node::Ip_V4 >::setup_environment();
  if( ! sts ) return -1;

  node::Node_Settings< node::Ip_V4 > node_settings = {

    .node_connection_manager_settings = node_connection_manager_settings

  };

  node::Node< node::Ip_V4 > node = node::Node< node::Ip_V4 >::create_node( node_settings );

  if( ! node.is_valid() ) return -1;

  // Start node operations
  node.start();

  // Stop operations and enable destructor
  node.enable_destructor();

  return 0;

}
