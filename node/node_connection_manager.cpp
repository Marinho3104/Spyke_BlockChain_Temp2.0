

#include "node_connection_manager.h"
#include "connection.h"
#include "signal_handling.h"
#include "socket_helper.h"
#include <arpa/inet.h>
#include <bits/types/sigset_t.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <sys/select.h>
#include <iostream>
#include <type_traits>


template < typename IP >
node::Node_Connection_Manager< IP >::Node_Connection_Manager::~Node_Connection_Manager() 
  { if( ! is_destructor_enable() ) return; delete[] ip_v4_stable_connections; delete[] ip_v6_stable_connections; delete[] ip_v4_temporary_connections; delete[] ip_v6_temporary_connections; }

template < typename IP >
node::Node_Connection_Manager< IP >::Node_Connection_Manager() : settings{}, status( NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_NOT_VALID ) {}

template < typename IP >
node::Node_Connection_Manager< IP >::Node_Connection_Manager( Node_Connection_Manager< IP >&& node_connection_manager ) noexcept
  : settings( node_connection_manager.settings ), server_connection( ( Connection< IP >&& ) node_connection_manager.server_connection ), ip_v4_stable_connections( node_connection_manager.ip_v4_stable_connections ),
    ip_v6_stable_connections( node_connection_manager.ip_v6_stable_connections ), 
      ip_v4_temporary_connections( node_connection_manager.ip_v4_temporary_connections ), 
        ip_v6_temporary_connections( node_connection_manager.ip_v6_temporary_connections ),
          status( node_connection_manager.status ), file_descriptor_manager( node_connection_manager.file_descriptor_manager ) { 

            node_connection_manager.ip_v4_stable_connections = 0; node_connection_manager.ip_v6_stable_connections = 0; 
            node_connection_manager.ip_v6_stable_connections = 0; node_connection_manager.ip_v6_temporary_connections = 0; 

}

template < typename IP >
node::Node_Connection_Manager< IP >::Node_Connection_Manager( const Node_Connection_Manager_Settings< IP >& settings )
  : settings( settings ), ip_v4_stable_connections( new Connection< Ip_V4 >[ settings.max_stable_connections ] ), 
    ip_v6_stable_connections( new Connection< Ip_V6 >[ settings.max_stable_connections ] ), 
      ip_v4_temporary_connections( new Connection< Ip_V4 >[ settings.max_temporary_connections ] ),
        ip_v6_temporary_connections( new Connection< Ip_V6 >[ settings.max_temporary_connections ] ),
          status( NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_VALID ) { FD_ZERO( &file_descriptor_manager ); }

template < typename IP >
node::Node_Connection_Manager< IP >::Node_Connection_Manager( const Node_Connection_Manager_Settings< IP >& settings, const Connection< IP >& server_connection )
  : settings( settings ), server_connection( ( Connection< IP >&& ) server_connection ), ip_v4_stable_connections( new Connection< Ip_V4 >[ settings.max_stable_connections ] ), 
    ip_v6_stable_connections( new Connection< Ip_V6 >[ settings.max_stable_connections ] ), 
      ip_v4_temporary_connections( new Connection< Ip_V4 >[ settings.max_temporary_connections ] ),
        ip_v6_temporary_connections( new Connection< Ip_V6 >[ settings.max_temporary_connections ] ),
          status( NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_VALID ) 
            { FD_ZERO( &file_descriptor_manager ); FD_SET( server_connection.get_socket_data().socket, &file_descriptor_manager ); }

template < typename IP >
const bool node::Node_Connection_Manager< IP >::is_valid() const { return status == NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_VALID; }

template < typename IP >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::is_destructor_enable() const { return status == NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_DESTRUCTOR_ENABLE; }

template < typename IP >
void node::Node_Connection_Manager< IP >::Node_Connection_Manager::enable_destructor() {
  
  if( ! is_valid() ) return;

  for( int _ = 0; _ < settings.max_stable_connections; _ ++ ) {

    ip_v4_stable_connections[ _ ].close_connection();
    ip_v6_stable_connections[ _ ].close_connection();

  }

  for( int _ = 0; _ < settings.max_temporary_connections; _ ++ ) {

    ip_v4_temporary_connections[ _ ].close_connection();
    ip_v6_temporary_connections[ _ ].close_connection();

  }

  server_connection.close_connection();

  status = NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_DESTRUCTOR_ENABLE;

}

template < typename IP >
template < typename CONNECTION_IP_TYPE >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::add_stable_connection( const Connection< CONNECTION_IP_TYPE >& connection ) {

  const unsigned int length = settings.max_stable_connections;
  Connection< CONNECTION_IP_TYPE >* connections_list;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, Ip_V4 >::value ) connections_list = ip_v4_stable_connections;
  else connections_list = ip_v6_stable_connections;

  for( unsigned int _ = 0; _ < length; _ ++ ) {

    if( connections_list[ _ ].is_valid() ) continue;

    FD_SET( connection.get_socket_data().socket, &file_descriptor_manager );

    new( connections_list + _ ) Connection< CONNECTION_IP_TYPE >( ( Connection< CONNECTION_IP_TYPE >&& ) connection );
    
    return 1;

  }

  return 0;

}

template < typename IP >
template < typename CONNECTION_IP_TYPE >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::add_temporary_connection( const Connection< CONNECTION_IP_TYPE >& connection ) {

  const unsigned int length = settings.max_temporary_connections;
  Connection< CONNECTION_IP_TYPE >* connections_list;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, Ip_V4 >::value ) connections_list = ip_v4_temporary_connections;
  else connections_list = ip_v6_temporary_connections;

  for( unsigned int _ = 0; _ < length; _ ++ ) {

    if( connections_list[ _ ].is_valid() ) continue;

    FD_SET( connection.get_socket_data().socket, &file_descriptor_manager );

    new( connections_list + _ ) Connection< CONNECTION_IP_TYPE >( ( Connection< CONNECTION_IP_TYPE >&& ) connection );

    return 1;

  }

  return 0;

}

template < typename IP >
template < typename CONNECTION_IP_TYPE >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::remove_stable_connection( const Connection< CONNECTION_IP_TYPE >& connection ) {

  const unsigned int length = settings.max_stable_connections;
  Connection< CONNECTION_IP_TYPE >* connections_list;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, Ip_V4 >::value ) connections_list = ip_v4_stable_connections;
  else connections_list = ip_v6_stable_connections;

  for( unsigned int _ = 0; _ < length; _ ++ ) {

    if( ! ( connections_list[ _ ] == connection ) ) continue;

    FD_CLR( connection.get_socket_data().socket, &file_descriptor_manager );

    connections_list[ _ ].close_connection();

    return 1;

  }

  return 0;

}

template < typename IP >
template < typename CONNECTION_IP_TYPE >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::remove_temporary_connection( const Connection< CONNECTION_IP_TYPE >& connection ) {

  const unsigned int length = settings.max_temporary_connections;
  Connection< CONNECTION_IP_TYPE >* connections_list;

  if constexpr( std::is_same< CONNECTION_IP_TYPE, Ip_V4 >::value ) connections_list = ip_v4_temporary_connections;
  else connections_list = ip_v6_temporary_connections;

  for( unsigned int _ = 0; _ < length; _ ++ ) {

    if( ! ( connections_list[ _ ] == connection ) ) continue;

    FD_CLR( connection.get_socket_data().socket, &file_descriptor_manager );

    connections_list[ _ ].close_connection();

    return 1;

  }

  return 0;

}

template < typename IP >
void node::Node_Connection_Manager< IP >::Node_Connection_Manager::make_initial_connections() {

  for( const Ip_V4& ip_information: settings.initial_ip_v4 ) {

    const Connection< Ip_V4 > new_connection = Connection< Ip_V4 >::create_connection( ip_information );
    if( ! new_connection.is_valid() ) continue;

    add_temporary_connection( new_connection );    

  }

  for( const Ip_V6& ip_information: settings.initial_ip_v6 ) {

    const Connection< Ip_V6 > new_connection = Connection< Ip_V6 >::create_connection( ip_information );
    if( ! new_connection.is_valid() ) continue;

    add_temporary_connection( new_connection );    

  }

}

template < typename IP >
void node::Node_Connection_Manager< IP >::start() {

  if( ! is_valid() ) return;

  const bool sts_environment = setup_environment();
  if( ! sts_environment ) return;

  make_initial_connections();
  
  fd_set file_descriptor_manager_copy; int sts;

  while( 1 ) {

    file_descriptor_manager_copy = file_descriptor_manager;

    sts = select( FD_SETSIZE, &file_descriptor_manager_copy, 0, 0, 0 );
    if( sts == -1 ) break;

    if( FD_ISSET( server_connection.get_socket_data().socket, &file_descriptor_manager_copy ) ) handle_server();
  }

}

template < typename IP >
void node::Node_Connection_Manager< IP >::Node_Connection_Manager::handle_server() {

  const Socket_Data < Ip_V6 > new_connection_socket_data = accept_connection_request( server_connection.get_socket_data().socket );

  // Ipv4 Mapped 
  if( IN6_IS_ADDR_V4MAPPED( &new_connection_socket_data.hint.sin6_addr ) ) {
      
    const Socket_Data< Ip_V4 > socket_data_ip_v4 = convert_ip_v6_into_ip_v4( new_connection_socket_data );
    const Ip_V4 ip_information = { .address = socket_data_ip_v4.hint.sin_addr.s_addr, .port = socket_data_ip_v4.hint.sin_port };

    std::cout << "Ipv4 mapped: " << ip_information.address << " " << ip_information.port << std::endl;

    const Connection< Ip_V4 > connection = { socket_data_ip_v4,  ip_information };

    add_temporary_connection( connection );

  }

  // Ipv6
  else {

    char* ip_str = new char[INET6_ADDRSTRLEN]; // Allocate memory for the IP string
    if (inet_ntop(AF_INET6, &new_connection_socket_data.hint.sin6_addr, ip_str, INET6_ADDRSTRLEN) != nullptr) {
        std::cout << "IPv6 Address: " << ip_str << std::endl;
    } else {
        std::cerr << "Error converting address: " << strerror(errno) << std::endl;
    }

    const Ip_V6 ip_information = { ( const char* ) &new_connection_socket_data.hint.sin6_addr, new_connection_socket_data.hint.sin6_port };
    const Connection < Ip_V6 > connection = { new_connection_socket_data, ip_information };

    add_temporary_connection( connection );

  }

}

template < typename IP >
const bool node::Node_Connection_Manager< IP >::Node_Connection_Manager::setup_environment() {

  struct sigaction sa;
  sa.sa_handler = handle_SIGUSR1;
  sigemptyset(&sa.sa_mask);  
  sa.sa_flags = 0;          
  sigaction(SIGUSR1, &sa, NULL);

  sigset_t signal_mask; sigemptyset( &signal_mask ); sigaddset( &signal_mask, SIGUSR1 );
  const int sts_sigprocmask_unblock = sigprocmask( SIG_UNBLOCK, &signal_mask, NULL );
  if( sts_sigprocmask_unblock != 0 ) return 0;

  sigemptyset( &signal_mask ); sigaddset( &signal_mask, SIGINT );
  const int sts_sigprocmask_block = sigprocmask( SIG_BLOCK, &signal_mask, NULL );
  if( sts_sigprocmask_block != 0 ) return 0;

  return 1;

}

template < typename IP >
const node::Node_Connection_Manager< IP > node::Node_Connection_Manager< IP >::create_node_connection_manager( const Node_Connection_Manager_Settings< IP >& settings ) {

  if( settings.server_ip_information.port == 0 ) return { settings }; // Server is not provided, creates without server
  
  const Connection< IP > server_connection = Connection< IP >::create_server( settings.server_ip_information );
  if( ! server_connection.is_valid() ) return {};

  return { settings, server_connection };

}

// Implicit instantiation for the possible templates
template class node::Node_Connection_Manager< node::Ip_V4 >;
template class node::Node_Connection_Manager< node::Ip_V6 >;
template class node::Node_Connection_Manager_Settings< node::Ip_V4 >;
template class node::Node_Connection_Manager_Settings< node::Ip_V6 >;
