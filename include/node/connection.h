

#ifndef NODE_CONNECTION_H
#define NODE_CONNECTION_H

#include <netinet/in.h>
#include <type_traits>

namespace node {

  // Represents information about the IP 
  // information of a possible connection
  // port is 0, when the information is invalid
  struct Ip_V4 {

    const unsigned int address;
    const unsigned short port;

  };

  struct Ip_V6 {
  
    const char address[ 16 ];
    const unsigned short port;

    Ip_V6();

    Ip_V6( const char [ 16 ], const unsigned short );

    Ip_V6( const Ip_V6& );

  };

  // Represents all socket related information
  // about a file descriptor ( socket connection )
  template < typename IP >
  struct Socket_Data {

      const typename std::conditional< std::is_same< IP, Ip_V4 >::value, sockaddr_in, sockaddr_in6 >::type hint;
      const int socket;

  };

  template < typename IP >
  class Connection {

    private:

      const Socket_Data< IP > socket_data;
      const IP ip_information;

      unsigned char status;

    public:

      Connection< IP >( const Connection< IP >& ) = delete;

      Connection< IP >( Connection< IP >&& );
      
      // Sets the connection as invalid
      Connection< IP >(); 

      // Tries to establish a connection with given IP
      Connection< IP >( const Socket_Data< IP >&, const IP& ); 

      const bool operator==( const Connection< IP >& ) const;

      const Socket_Data< IP >& get_socket_data() const;

      const bool is_valid() const;

      void close_connection();

      // Will try to connect to the given IP and return the connection
      static const Connection< IP > create_connection( const IP& );

      // Will try to setup a server with given IP and return the connection
      static const Connection< IP > create_server( const IP& );

  };

}

/** TYPES OF POSSIBLE STATUS FOR THE CONNECTION CLASS **/
#define NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID 0 // Means nothing of the information set is valid
#define NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED 1 // Means that the connection is open
#define NODE_CONNECTION_H_CONNECTION_STATUS_DESTRUCTOR_ENABLE 2 

#endif
