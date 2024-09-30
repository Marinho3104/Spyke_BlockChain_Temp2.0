
#ifndef NODE_H
#define NODE_H

#include "node_connection_manager.h"

namespace node {

  template < typename IP >
  struct Node_Settings {
  
    const Node_Connection_Manager_Settings< IP > node_connection_manager_settings;

  };

  template < typename IP >
  class Node {

    private:

      const Node_Settings< IP > settings;

      Node_Connection_Manager< IP > connection_manager;

      unsigned char status;

    public:

      Node();

      Node( const Node< IP >& ) = delete;

      Node( const Node_Settings< IP >&, const Node_Connection_Manager< IP >& );

      const bool is_valid() const;

      const bool is_destructor_enable() const;

      // Will change the status value to enable the 
      // destructor to delete all heap memory used,
      // also will behave as a stop point for the 
      // Node operations, meanning
      // that this function will only return when
      // all operations stop execution, enabling for
      // a safe destructor call and object deallocation
      void enable_destructor();

      void start();

      // Will setup everything needed for a proper execution
      // of the Node and related processes. Setting the correct 
      // signals, signalmasks, etc ...
      // This function should be executed by the main thread
      static const bool setup_environment();

      static const Node create_node( const Node_Settings< IP >& );

  };

}

/** TYPES OF POSSIBLE STATUS FOR THE CONNECTION CLASS **/
#define NODE_H_NODE_STATUS_NOT_VALID 0 
#define NODE_H_NODE_STATUS_VALID 1 
#define NODE_H_NODE_STATUS_DESTRUCTOR_ENABLE 2 

#endif
