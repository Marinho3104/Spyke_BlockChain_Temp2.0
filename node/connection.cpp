
#include "connection.h"
#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

template < typename IP >
node::Connection< IP >::Connection() : status( NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID ) {}

template < typename IP >
node::Connection< IP >::Connection( IP& ip_information ) : ip_information( ip_information ), status( NODE_CONNECTION_H_CONNECTION_STATUS_NOT_CONNECTED ) {

  // To avoid use of unistialized data
  memset( &socket_data.hint, 0, sizeof( socket_data.hint ) );

}

template < typename IP >
bool node::Connection< IP >::is_valid() { return status != NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID; }

template <>
bool node::Connection< node::Ip_V4 >::setup_server() {

  // Normal instruction flow for
  // the initialization of a server
  // socket, if success also changes the 
  // status to Connected, meaning that the
  // server is up
  
  if( ! is_valid() ) return 0;

  socket_data.socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_data.socket == -1 ) return 0;

  socket_data.hint.sin_family = AF_INET;
  socket_data.hint.sin_port = htons( ip_information.port );
  socket_data.hint.sin_addr.s_addr = htonl( ip_information.address );

  int sts = bind( socket_data.socket, ( sockaddr* ) &socket_data.hint, sizeof( socket_data.hint ) );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }

  sts = listen( socket_data.socket, 5 );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }
   
  status = NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED;

  return 1;

}

template <>
bool node::Connection< node::Ip_V6 >::setup_server() { 

  // Normal instruction flow for
  // the initialization of a server
  // socket, if success also changes the 
  // status to Connected, meaning that the
  // server is up

  if( ! is_valid() ) return 0;

  socket_data.socket = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
  if( socket_data.socket == -1 ) return 0;

  socket_data.hint.sin6_family = AF_INET;
  socket_data.hint.sin6_port = htons( ip_information.port );

  int sts = inet_pton( AF_INET6, ip_information.address, &socket_data.hint.sin6_addr );
  if( sts <= 0 ) { close( socket_data.socket ); }

  sts = bind( socket_data.socket, ( sockaddr* ) &socket_data.hint, sizeof( socket_data.hint ) );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }

  sts = listen( socket_data.socket, 5 );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }

  status = NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED;

  return 1;

}

template <>
bool node::Connection< node::Ip_V4 >::connect() { 

  // Normal instruction flow for
  // the initialization of a connection
  // socket, if success also changes the 
  // status to Connected
  
  if( ! is_valid() ) return 0;

  socket_data.socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_data.socket == -1 ) return 0;

  socket_data.hint.sin_family = AF_INET;
  socket_data.hint.sin_port = htons( ip_information.port );
  socket_data.hint.sin_addr.s_addr = htonl( ip_information.address );

  int sts = ::connect( socket_data.socket, ( sockaddr* ) &socket_data.hint, sizeof( socket_data.hint ) );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }

  status = NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED;

  return 1;

}

template <>
bool node::Connection< node::Ip_V6 >::connect() { 

  // Normal instruction flow for
  // the initialization of a connection
  // socket, if success also changes the 
  // status to Connected

  if( ! is_valid() ) return 0;

  socket_data.socket = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
  if( socket_data.socket == -1 ) return 0;

  socket_data.hint.sin6_family = AF_INET;
  socket_data.hint.sin6_port = htons( ip_information.port );

  int sts = inet_pton( AF_INET6, ip_information.address, &socket_data.hint.sin6_addr );
  if( sts <= 0 ) { close( socket_data.socket ); }

  sts = ::connect( socket_data.socket, ( sockaddr* ) &socket_data.hint, sizeof( socket_data.hint ) );
  if( sts == -1 ) { close( socket_data.socket ); return 0; }

  status = NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED;

  return 0; 

}


// Implicit instantiation for the possible templates
template class node::Connection< node::Ip_V4 >;
template class node::Connection< node::Ip_V6 >;
