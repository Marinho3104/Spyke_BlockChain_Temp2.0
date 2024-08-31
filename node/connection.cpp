
#include "connection.h"

template < typename IP >
node::Connection< IP >::Connection() : status( NODE_CONNECTION_H_CONNECTION_STATUS_NOT_VALID ) {}

template < typename IP >
node::Connection< IP >::Connection( IP& ip_information ) : ip_information( ip_information ), status( NODE_CONNECTION_H_CONNECTION_STATUS_NOT_CONNECTED ) {}

template < typename IP >
bool node::Connection< IP >::start_server() { return 0; }

template < typename IP >
bool node::Connection< IP >::connect() { return 0; }

// Implicit instantiation for the possible templates
template class node::Connection< node::Ip_V4 >;
template class node::Connection< node::Ip_V6 >;
