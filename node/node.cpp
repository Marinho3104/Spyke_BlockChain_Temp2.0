
#include "node.h"
#include "connection.h"
#include "node_connection_manager.h"
#include "signal_handling.h"
#include <csignal>
#include <thread>
#include <vector>

template < typename IP >
node::Node< IP >::Node() : status( NODE_H_NODE_STATUS_NOT_VALID ), settings{} {}

template < typename IP >
node::Node< IP >::Node( const Node_Settings< IP >& settings, const Node_Connection_Manager< IP >& node_connection_manager ) : 
    settings( settings ), connection_manager( ( Node_Connection_Manager< IP >&& ) node_connection_manager ), status( NODE_H_NODE_STATUS_VALID ) {}

template < typename IP >
const bool node::Node< IP >::Node::is_valid() const { return status == NODE_H_NODE_STATUS_VALID; }

template < typename IP >
const bool node::Node< IP >::Node::is_destructor_enable() const { return status == NODE_H_NODE_STATUS_DESTRUCTOR_ENABLE; }

template < typename IP >
void node::Node< IP >::Node::enable_destructor() {

  if( ! is_valid() ) return;

  connection_manager.enable_destructor();

  status = NODE_H_NODE_STATUS_DESTRUCTOR_ENABLE;

}

template < typename IP >
void node::Node< IP >::Node::start() {

  if( ! is_valid() ) return;

  std::vector< std::thread > processes;
  processes.push_back( std::thread( &node::Node_Connection_Manager< IP >::start, &connection_manager ) );

  for( auto& process : processes ) process.join();

}

template < typename IP >
const bool node::Node< IP >::Node::setup_environment() {

  sigset_t main_signal_mask; sigfillset( &main_signal_mask ); sigdelset( &main_signal_mask, SIGINT );
  const int sts_sigprocmask = sigprocmask( SIG_BLOCK, &main_signal_mask, NULL );
  if( sts_sigprocmask != 0 ) return 0;

  struct sigaction sa; sa.sa_handler = handle_SIGINT;
  sigemptyset( &sa.sa_mask ); sa.sa_flags = 0;
  const int sts_sigaction = sigaction( SIGINT, &sa, nullptr ); 
  if( sts_sigaction != 0 ) return 0;

  return 1;

}

template < typename IP >
const node::Node< IP > node::Node< IP >::Node::create_node( const Node_Settings< IP >& settings ) {

  const Node_Connection_Manager< IP > node_connection_manager = Node_Connection_Manager< IP >::create_node_connection_manager( settings.node_connection_manager_settings );
  if( ! node_connection_manager.is_valid() ) return {};

  return {

    settings,
    node_connection_manager

  };

}

// Implicit instantiation for the possible templates
template class node::Node_Settings< node::Ip_V4 >;
template class node::Node_Settings< node::Ip_V6 >;
template class node::Node< node::Ip_V4 >;
template class node::Node< node::Ip_V6 >;
