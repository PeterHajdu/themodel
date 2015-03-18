#pragma once

#include <themodel/node.hpp>
#include <memory>
#include <unordered_map>

namespace the
{
namespace model
{

template< typename OwningPolicy = Owner >
class NodeList : public Node< OwningPolicy >
{
  public:
    template < typename Parent >
    NodeList( std::string name, Parent& parent )
      : Node< OwningPolicy >( std::move( name ), parent )
    {
    }

    virtual ~NodeList() = default;

    void add_node( TreeNode::Pointer&& new_node )
    {
      m_nodes.emplace( std::make_pair( new_node->name, std::move( new_node ) ) );
    }

    void delete_node( const std::string& name )
    {
      auto node_iterator( m_nodes.find( name ) );
      if ( node_iterator == std::end( m_nodes ) )
      {
        return;
      }

      m_nodes.erase( node_iterator );
    }


    void clear()
    {
      m_nodes.clear();
    }

  private:
    std::unordered_map< std::string, TreeNode::Pointer > m_nodes;
};

typedef NodeList< Owner > OwningNodeList;

}
}

