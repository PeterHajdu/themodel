#include <themodel/node_list.hpp>

namespace the
{
namespace model
{

NodeList::NodeList( const std::string& name, Retriever retriever )
  : Node( name, retriever )
{
}

void
NodeList::add_node( NodeBase::Pointer&& new_node )
{
  m_nodes.emplace( std::make_pair( new_node->name(), std::move( new_node ) ) );
}

void
NodeList::delete_node( const std::string& name )
{
  auto node_iterator( m_nodes.find( name ) );
  if ( node_iterator == std::end( m_nodes ) )
  {
    return;
  }

  m_nodes.erase( node_iterator );
}

void
NodeList::clear()
{
  m_nodes.clear();
}

}
}

