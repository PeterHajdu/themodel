#include <themodel/zmq_remote.hpp>
#include <themodel/tree.hpp>
#include <deque>

namespace
{

bool
is_call_request( const zmq::message_t& request )
{
  const std::string call_prefix{ "call " };
  if ( request.size() < call_prefix.size() )
  {
    return false;
  }

  const std::string request_text{
    static_cast< const char *>( request.data() ),
    request.size() };

  if ( request_text.substr( 0, call_prefix.size() ) != call_prefix )
  {
    return false;
  }

  return true;
}

std::deque< std::string >
split( const std::string& str )
{
  std::deque< std::string > ret;

  size_t previous_position{ 0 };
  size_t position{ str.find_first_of( "." ) };
  while ( position != std::string::npos )
  {
    ret.push_back( str.substr( previous_position, position - previous_position ) );
    previous_position = position + 1;
    position = str.find_first_of( ".", position + 1 );
  }

  ret.push_back( str.substr( previous_position ) );

  return ret;
}


std::deque< std::string >
extract_call_path( const zmq::message_t& request )
{
  const size_t call_prefix_size( 5u );
  const size_t parameter_size( request.size() - call_prefix_size );
  std::string request_text{
    static_cast< const char *>( request.data() ) + call_prefix_size,
    parameter_size };
  return split( request_text );
}

}

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
    call_function_if_requested( request );
    zmq::message_t response( exported_tree.length() + 1 );
    memcpy( response.data(), exported_tree.data(), exported_tree.length() );
    char* buffer_end{ static_cast< char* >( response.data() ) + exported_tree.length() };
    *buffer_end = 0;
    m_socket.send( response );
  } while( m_socket.recv( &request, ZMQ_DONTWAIT ) );

}


void
ZmqRemote::call_function_if_requested( const zmq::message_t& request )
{
  if ( !is_call_request( request ) )
  {
    return;
  }

  auto path( extract_call_path( request ) );
  if ( path.front() != m_root_node.name )
  {
    return;
  }

  path.pop_front();
  const TreeNode* last_node( &m_root_node );

  for ( const auto& node_name : path )
  {
    auto children( last_node->children() );
    auto child_iterator( children.find( node_name ) );
    if ( child_iterator == std::end( children ) )
    {
      return;
    }

    last_node = &child_iterator->second.get();
  }

  last_node->call();
  return;
}

}

}

