
#include "node.h"
#include "connection.h"
#include "node_connections_manager.h"

template < typename IP >
node::Node< IP >::Node( Node_Settings< IP >& settings ) {

  // Initializes the Node connections manager to manade all upcoming connections
  Node_Connections_Manager_Settings< IP > node_connections_manager_settings = {
  
    settings.max_stable_connections,
    settings.max_temporary_connections,
    settings.server_ip_information

  };

  this->connections_manager = Node_Connections_Manager< IP >( node_connections_manager_settings );

}

template < typename IP >
void node::Node< IP >::run() {
  
  connections_manager.start();

}

// Implicit instantiation for the possible templates
template class node::Node< node::Ip_V4 >;
template class node::Node< node::Ip_V6 >;
