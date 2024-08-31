
#include "connection.h"
#include "node.h"

int main (int argc, char *argv[]) {

  // Node settings
  node::Node_Settings< node::Ip_V4 > node_settings = {

    3, // Max stable connections
    3, // Max temporary connections
    {  // Ip V4 server connection information

      0x7F000001,
      3000

    },

  };

  node::Node node = node::Node( node_settings );

  // Start node processes
  node.run();

  return 0;
}
