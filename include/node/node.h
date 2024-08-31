

#ifndef NODE_NODE_H
#define NODE_NODE_H

#include "node_connections_manager.h"

namespace node {

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

      Node_Connections_Manager< IP > connections_manager; // Manage all Node connections

    public:

      Node( Node_Settings< IP >& );

      // Will start all processes of this node,
      // only returns when the node stops running
      void run();

  };

}

#endif
