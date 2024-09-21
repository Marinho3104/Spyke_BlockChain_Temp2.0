
#include "node_connections_manager.h"
#include "connection.h"
#include <memory>
#include <iostream>

template < typename IP >
node::Node_Connections_Manager< IP >::Node_Connections_Manager( Node_Connections_Manager_Settings< IP >& settings ) : settings( settings ) {

  // Alloc enough space for all possible connections, and calls the
  // default constructor making all connections not valid
  ip_v4_stable_connections = std::make_unique< Connection< Ip_V4 >[] >( settings.max_stable_connections );
  ip_v6_stable_connections = std::make_unique< Connection< Ip_V6 >[] >( settings.max_stable_connections );
  ip_v4_temporary_connections = std::make_unique< Connection< Ip_V4 >[] >( settings.max_temporary_connections );
  ip_v6_temporary_connections = std::make_unique< Connection< Ip_V6 >[] >( settings.max_temporary_connections );

}

template < typename IP >
bool node::Node_Connections_Manager< IP >::setup() {

  // Initiates the server connection object if given server connection is valid,
  // and setups the connection
  if ( settings.server_ip_information.port ) { 

    server_connection = Connection< IP >( settings.server_ip_information ); 

    if( ! server_connection.setup_server() ) return 0;

  }
  
  return 1;

}

template < typename IP >
void node::Node_Connections_Manager< IP >::start() {

  

}

// Implicit instantiation for the possible templates
template class node::Node_Connections_Manager< node::Ip_V4 >;
template class node::Node_Connections_Manager< node::Ip_V6 >;
