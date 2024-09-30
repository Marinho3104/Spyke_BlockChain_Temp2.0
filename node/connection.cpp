
#include "connection.h"
#include "socket_helper.h"
#include <cstring>
#include <unistd.h>


node::Ip_V6::Ip_V6() : address{}, port( 0 ) {}

node::Ip_V6::Ip_V6( const char address[ 16 ], const unsigned short port ) : address{}, port( port ) 
  { std::memcpy( ( void* ) address, this->address, sizeof( this->address ) ); }

node::Ip_V6::Ip_V6( const Ip_V6& ip_information ) : address{}, port( ip_information.port ) 
  { std::memcpy( ( void* ) ip_information.address, address, sizeof( address ) ); }

template < typename IP >
node::Connection< IP >::Connection( Connection< IP >&& connection ) 
  : socket_data( connection.socket_data ), ip_information( connection.ip_information ), status( connection.status ) { connection.status = NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID; }

template < typename IP >
node::Connection< IP >::Connection() : socket_data{}, ip_information{}, status( NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID ) {}

template < typename IP >
node::Connection< IP >::Connection( const Socket_Data< IP >& socket_data, const IP& ip_information ) 
  : socket_data( socket_data ), ip_information( ip_information ), status( NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED ) {}

template < typename IP >
const bool node::Connection< IP >::Connection::operator==( const Connection< IP >& connection ) const 
  { return is_valid() && connection.is_valid() && socket_data.socket == connection.get_socket_data().socket; }

template < typename IP >
const node::Socket_Data< IP >& node::Connection< IP >::get_socket_data() const { return socket_data; }

template < typename IP >
const bool node::Connection< IP >::is_valid() const { return status == NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED; }

template < typename IP >
void node::Connection< IP >::Connection::close_connection() {

  if( ! is_valid() ) return;

  close( socket_data.socket );

  status = NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID;

}

template < typename IP >
const node::Connection< IP > node::Connection< IP >::Connection::create_connection( const IP& ip_information ) {

  // Tries to connect with given ip_information
  // and return a Connection object with the 
  // correct information, and the corresponding 
  // status

  const Socket_Data< IP >& socket_data = connect( ip_information );
  if( socket_data.socket == -1 ) return {};

  return {

    socket_data,
    ip_information

  };   

}

template < typename IP >
const node::Connection< IP > node::Connection< IP >::create_server( const IP& ip_information ) {
  
  // Tries to create a server with
  // given ip_information and return
  // a Connection object with the
  // correct information, and the 
  // corresponding status

  const Socket_Data< IP >& socket_data = setup_server( ip_information );
  if( socket_data.socket == -1 ) return {};

  return {

    socket_data,
    ip_information

  };

}

// Implicit instantiation for the possible templates
template class node::Connection< node::Ip_V4 >;
template class node::Connection< node::Ip_V6 >;
