
#ifndef NODE_SOCKET_HELPER_H
#define NODE_SOCKET_HELPER_H

#include "connection.h"
namespace node {

  // Will try to create a server
  // with given IP, return socket
  // data about the connection socket
  template < typename IP >
  const Socket_Data< IP > setup_server( const IP& );

  // Will try to connect with given IP
  // return socket data about the 
  // connection socket
  template < typename IP >
  const Socket_Data< IP > connect( const IP& );

  const Socket_Data< Ip_V6 > accept_connection_request( const int& );

  const Socket_Data< Ip_V4 > convert_ip_v6_into_ip_v4( const Socket_Data< Ip_V6>& );

};

#endif
