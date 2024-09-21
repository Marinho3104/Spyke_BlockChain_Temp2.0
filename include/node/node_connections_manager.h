

#ifndef NODE_NODE_CONNECTIONS_MANAGER_H
#define NODE_NODE_CONNECTIONS_MANAGER_H

#include "connection.h"
#include <memory>
namespace node {

  template < typename IP >
  struct Node_Connections_Manager_Settings {

    // The max amount of stable and temporary connections
    // that the node will accept
    unsigned char max_stable_connections, max_temporary_connections;

    // Server Ip information that will be
    // used by the node
    IP server_ip_information;

  };

  // Class responsible for handling every open connection that the Node have
  // and the server if applicable
  template < typename IP >
  class Node_Connections_Manager {

    private:

      Node_Connections_Manager_Settings< IP > settings; // Settings for setup phase

      // Possible connections open in the manager
      std::unique_ptr< Connection< Ip_V4 >[] > ip_v4_stable_connections;
      std::unique_ptr< Connection< Ip_V6 >[] > ip_v6_stable_connections;

      std::unique_ptr< Connection< Ip_V4 >[] > ip_v4_temporary_connections;
      std::unique_ptr< Connection< Ip_V6 >[] > ip_v6_temporary_connections;

      Connection< IP > server_connection; // Server connection

    public:

      // Default
      Node_Connections_Manager< IP >() = default;

      // Inicializes the manager with all necessary settings/information
      Node_Connections_Manager( Node_Connections_Manager_Settings< IP >& );

      // Execute all necessary tasks, prior to start the
      // main loop, including start server listening, trying to connect 
      // with given nodes ip information, etc ...
      bool setup();
    
      // Will initialize this manager "process", will not return
      // until is to close the manager activity.
      // This process will be basically a main loop
      // that will wait for new information from the current connections
      // , to came and handle them, including server connection, so new connections can be added to the
      // open connections.
      void start();


  };

}

#endif

