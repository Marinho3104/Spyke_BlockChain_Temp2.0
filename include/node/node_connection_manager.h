
#ifndef NODE_CONNECTION_MANAGER_H
#define NODE_CONNECTION_MANAGER_H

#include "connection.h"
#include <vector>

namespace node {

  template < typename IP >
  struct Node_Connection_Manager_Settings {

    // Max concurrent connections possible for each type
    const unsigned int max_stable_connections, max_temporary_connections;

    // Server Ip information
    const IP server_ip_information;

    // List of initial connections
    // that the Node will attempt to connect
    std::vector< Ip_V4 > initial_ip_v4; std::vector< Ip_V6 > initial_ip_v6;

  };
  
  template < typename IP >
  class Node_Connection_Manager {

    private:

      const Node_Connection_Manager_Settings< IP > settings;

      Connection < Ip_V4 >* ip_v4_stable_connections;
      Connection < Ip_V6 >* ip_v6_stable_connections;
      Connection < Ip_V4 >* ip_v4_temporary_connections;
      Connection < Ip_V6 >* ip_v6_temporary_connections;
      Connection< IP > server_connection;

      unsigned char status;

      fd_set file_descriptor_manager;

    public:

      ~Node_Connection_Manager();

      Node_Connection_Manager();
    
      Node_Connection_Manager( const Node_Connection_Manager& ) = delete;

      Node_Connection_Manager( Node_Connection_Manager< IP >&& ) noexcept;

      Node_Connection_Manager( const Node_Connection_Manager_Settings< IP >& );

      Node_Connection_Manager( const Node_Connection_Manager_Settings< IP >&, const Connection< IP >& );

      const bool is_valid() const;

      const bool is_destructor_enable() const;

      template < typename CONNECTION_IP_TYPE >
      const bool add_stable_connection( const Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool add_temporary_connection( const Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool remove_stable_connection( const Connection< CONNECTION_IP_TYPE >& );

      template < typename CONNECTION_IP_TYPE >
      const bool remove_temporary_connection( const Connection< CONNECTION_IP_TYPE >& );

      // Will change the status value to enable the 
      // destructor to delete all heap memory used,
      // also will behave as a stop point for the 
      // Node_Connection_Manager operations, meanning
      // that this function will only return when
      // all operations stop execution, enabling for
      // a safe destructor call and object deallocation
      void enable_destructor();

      // Will attempt to connect with all
      // given initial connections
      void make_initial_connections();

      void start();
  
      void handle_server();

      // Will setup everything needed for a proper execution
      // of the Node and related processes. Setting the correct 
      // signals, signalmasks, etc ...
      // This function should be executed by the Node Connection Manager thread
      static const bool setup_environment();

      static const Node_Connection_Manager< IP > create_node_connection_manager( const Node_Connection_Manager_Settings< IP >& );

  };

};

/** TYPES OF POSSIBLE STATUS FOR THE CONNECTION CLASS **/
#define NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_NOT_VALID 0 
#define NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_VALID 1 
#define NODE_CONNECTION_MANAGER_H_NODE_CONNECTION_MANAGER_STATUS_DESTRUCTOR_ENABLE 2

#endif
