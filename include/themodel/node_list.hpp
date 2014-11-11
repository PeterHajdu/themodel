#pragma once

#include <themodel/node.hpp>
#include <memory>
#include <unordered_map>

namespace the
{
namespace model
{

class NodeList : public OwningNode
{
  public:
    template < typename Parent >
    NodeList( const std::string& name, Parent& parent )
      : Node( name, parent )
    {
    }

    virtual ~NodeList() = default;

    void add_node( NodeBase::Pointer&& new_node );
    void delete_node( const std::string& name );
    void clear();

  private:
    std::unordered_map< std::string, NodeBase::Pointer > m_nodes;
};

}
}

