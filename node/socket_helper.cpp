
#include "socket_helper.h"
#include "connection.h"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


template <>
const node::Socket_Data< node::Ip_V4 > node::setup_server( const Ip_V4& ip_information ) {

  // Normal instruction flow for
  // the initialization of a 
  // server socket

  const int socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return { .socket = -1 };

  const sockaddr_in hint = { 

    AF_INET, 
    htons( ip_information.port ), 
    htonl( ip_information.address ) 

  };

  const int sts_bind = bind( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_bind == -1 ) { close( socket_id ); return { .socket = -1 }; }

  const int sts_listen = listen( socket_id, 5 );
  if( sts_listen == -1 ) { close( socket_id ); return { .socket = -1 }; }

  return {

    .hint = hint,
    .socket =  socket_id

  };

}

template <>
const node::Socket_Data< node::Ip_V6 > node::setup_server( const Ip_V6& ip_information ) {

  // Normal instruction flow for
  // the initialization of a 
  // server socket

  const int socket_id = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return { .socket = -1 };

  const sockaddr_in6 hint = {
  
    AF_INET6,
    htons( ip_information.port ),

  };

  const int sts_pton = inet_pton( AF_INET6, ip_information.address, ( void* ) &hint.sin6_addr );
  if( sts_pton <= 0 ) { close( socket_id ); return { .socket = -1 }; }

  const int sts_bind = bind( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_bind == -1 ) { close( socket_id ); return { .socket = -1 }; }

  const int sts_listen = listen( socket_id, 5 );
  if( sts_listen == -1 ) { close( socket_id ); return { .socket = -1 }; }

  return {

    .hint = hint,
    .socket = socket_id

  };

}

template <>
const node::Socket_Data< node::Ip_V4 > node::connect( const Ip_V4& ip_information ) {

  // Normal instruction flow for
  // the initialization of a connection
  // socket 

  const int socket_id = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return { .socket = -1 };

  const sockaddr_in hint = {

    AF_INET,
    htons( ip_information.port ),
    htonl( ip_information.address )

  };

  const int sts_connect = ::connect( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_connect == -1 ) { close( socket_id ); return { .socket = -1 }; }

  return {

    .hint = hint,
    .socket = socket_id
      
  };

}

template <>
const node::Socket_Data< node::Ip_V6 > node::connect( const Ip_V6& ip_information ) {

  // Normal instruction flow for
  // the initialization of a connection
  // socket

  const int socket_id = socket( AF_INET6, SOCK_STREAM, IPPROTO_TCP );
  if( socket_id == -1 ) return { .socket = -1 };

  const sockaddr_in6 hint = {
  
    AF_INET6,
    htons( ip_information.port )

  };

  const int sts_pton = inet_pton( AF_INET6, ip_information.address, ( void* ) &hint.sin6_addr );
  if( sts_pton <= 0 ) { close( socket_id ); return { .socket = -1 }; }

  const int sts_connect = ::connect( socket_id, ( sockaddr* ) &hint, sizeof( hint ) );
  if( sts_connect == -1 ) { close( socket_id ); return { .socket = -1 }; }

  return {

    .hint = hint,
    .socket = socket_id
      
  };

}


const node::Socket_Data< node::Ip_V6 > node::accept_connection_request( const int& socket_id ) {

  sockaddr_in6 new_connection_hint; socklen_t new_connection_hint_length = sizeof( new_connection_hint );

  const int new_connection_socket_id = 
    accept(

      socket_id,
      ( sockaddr* ) &new_connection_hint,
      &new_connection_hint_length

    );

  if( new_connection_socket_id == -1 ) return {};

  return {

    .hint = new_connection_hint,
    .socket = new_connection_socket_id

  };

}

const node::Socket_Data< node::Ip_V4 > node::convert_ip_v6_into_ip_v4( const Socket_Data< node::Ip_V6 >& socket_data ) {

  const unsigned char* ip_v6_address = ( const unsigned char* ) &socket_data.hint.sin6_addr;

  return {

    AF_INET,
    socket_data.hint.sin6_port,
    htonl( ( unsigned int ) ip_v6_address[ 12 ] )

  };

}



