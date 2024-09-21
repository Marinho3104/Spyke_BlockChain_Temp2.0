
#include "node.h"
#include "connection.h"
#include "node_connections_manager.h"

template < typename IP >
node::Node< IP >::Node( Node_Settings< IP >& settings ) : settings( settings ) {}

template < typename IP >
bool node::Node< IP >::setup() {

  // Performs a setup in everything needed:
  //
  // Connections manager ( initialization and setup )

  // Initializes the Node connections manager to manage all upcoming connections
  Node_Connections_Manager_Settings< IP > node_connections_manager_settings = {
  
    settings.max_stable_connections,
    settings.max_temporary_connections,
    settings.server_ip_information

  };

  this->connections_manager = Node_Connections_Manager< IP >( node_connections_manager_settings );

  bool sts = this->connections_manager.setup();
  if ( ! sts ) return 0;

  return 1;

}

template < typename IP >
void node::Node< IP >::run() {

  // Starts a thread for each needed process
  // maintaining the main one for the execution
  // of the connections manager, meaning that this
  // function will only return when the connections manager functions
  // returns as well, hopefully only gracefully the node ends all processes :)
  
  connections_manager.start();

}

// Implicit instantiation for the possible templates
template class node::Node< node::Ip_V4 >;
template class node::Node< node::Ip_V6 >;
