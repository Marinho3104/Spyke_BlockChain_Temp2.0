

#ifndef NODE_CONNECTION_H
#define NODE_CONNECTION_H

namespace node {

  // For both Ip types, having the port set to 0, means that 
  // the information set should not be considered as valid information
  struct Ip_V4 {

    unsigned int address;
    unsigned short port;

  };

  struct Ip_V6 {

    unsigned short port;

  };

  struct Socket_Data {

    void* hint;
    int socket;

  };

  template < typename IP >
  class Connection {

    private:

      Socket_Data socket_data;    
      IP ip_information;

      unsigned char status;

    public:

      // Default constructor, will set the connection as invalid
      Connection< IP >();

      // Gets the IP information of this connection, and sets the connection as Not Connected
      Connection< IP >( IP& );

      // Will interpret this connection as a server
      // and boot up a server with ip_information data
      bool start_server();

      // Will try to connect with the 
      // ip_information data
      bool connect();

  };

}

/** TYPES OF POSSIBLE STATUS FOR THE CONNECTION CLASS **/
#define NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID 0 // Means nothing of the information set is valid
#define NODE_CONNECTION_H_CONNECTION_STATUS_NOT_CONNECTED 1 // Means the information set is correct but the connection is not made
#define NODE_CONNECTION_H_CONNECTION_STATUS_CONNECTED 2 // Means that the connection is open

#endif
