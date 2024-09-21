

#ifndef NODE_NODE_H
#define NODE_NODE_H

#include "node_connections_manager.h"

namespace node {

  // Settings that will be used for the Node setup phase
  template < typename IP >
  struct Node_Settings {

    // The max amount of stable and temporary connections
    // that the node will accept
    unsigned char max_stable_connections, max_temporary_connections;

    // Server Ip information that will be
    // used by the node
    IP server_ip_information;

  };

  template < typename IP >
  class Node {

    private:

      Node_Settings< IP > settings; // Settings used in Node setup phase

      Node_Connections_Manager< IP > connections_manager; // Manage all Node connections

    public:

      Node( Node_Settings< IP >& );

      // Makes sure everything is ready, for the node
      // to start running properly
      bool setup();
  
      // Will start all processes of this node,
      // only returns when the node stops running
      void run();

  };

}

#endif
