
#include "node_connections_manager.h"
#include "connection.h"
#include "node.h"
#include <iostream>
#include <memory>

template < typename IP >
node::Node_Connections_Manager< IP >::Node_Connections_Manager( Node_Connections_Manager_Settings< IP >& settings ) : settings( settings ) {

  // Alloc enough space for all possible connections
  ip_v4_stable_connections = std::make_unique< Connection< Ip_V4 >[] >( settings.max_stable_connections );
  ip_v6_stable_connections = std::make_unique< Connection< Ip_V6 >[] >( settings.max_stable_connections );
  ip_v4_temporary_connections = std::make_unique< Connection< Ip_V4 >[] >( settings.max_temporary_connections );
  ip_v6_temporary_connections = std::make_unique< Connection< Ip_V6 >[] >( settings.max_temporary_connections );

  // Make sure all connections alloc before are invalid
  for( unsigned char _ = 0; _ < settings.max_stable_connections; _ ++ ) {

    ip_v4_stable_connections.get()[ _ ] = Connection< Ip_V4 >();
    ip_v6_stable_connections.get()[ _ ] = Connection< Ip_V6 >();

  }

  for( unsigned char _ = 0; _ < settings.max_temporary_connections; _ ++ ) {

    ip_v4_temporary_connections.get()[ _ ] = Connection< Ip_V4 >();
    ip_v6_temporary_connections.get()[ _ ] = Connection< Ip_V6 >();

  }

  // Creates the server connection object if given server connection is valid
  if ( settings.server_ip_information.port ) server_connection = Connection< IP >( settings.server_ip_information );

}

template < typename IP >
bool node::Node_Connections_Manager< IP >::setup() {

  // If server connection ip information is valid, setups
  // the connection information
  if( server_connection.is_valid() ) {

    bool sts = server_connection.setup_server();
    if( ! sts ) return 0;

  }

  

  return 0;

}

template < typename IP >
void node::Node_Connections_Manager< IP >::start() {

  

}

// Implicit instantiation for the possible templates
template class node::Node_Connections_Manager< node::Ip_V4 >;
template class node::Node_Connections_Manager< node::Ip_V6 >;
