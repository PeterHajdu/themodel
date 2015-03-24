#include <themodel/zmq_remote.hpp>
#include <iostream>

namespace the
{

namespace model
{

ZmqRemote::ZmqRemote(
    const char* endpoint,
    const TreeNode& root_node,
    Exporter exporter )
  : m_context( 1 )
  , m_socket( m_context, ZMQ_REP )
  , m_exporter( exporter )
  , m_root_node( root_node )
{
  m_socket.bind( endpoint );
}

void
ZmqRemote::handle_requests()
{
  zmq::message_t request;
  if ( !m_socket.recv( &request, ZMQ_DONTWAIT ) )
  {
    return;
  }

  const auto exported_tree( m_exporter( m_root_node ) );

  do
  {
    zmq::message_t response( exported_tree.length() + 1 );
    memcpy( response.data(), exported_tree.data(), exported_tree.length() );
    char* buffer_end{ static_cast< char* >( response.data() ) + exported_tree.length() };
    *buffer_end = 0;
    m_socket.send( response );
  } while( m_socket.recv( &request, ZMQ_DONTWAIT ) );

}

}

}

