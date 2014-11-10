#pragma once

#include <themodel/node.hpp>
#include <memory>
#include <unordered_map>

namespace the
{
namespace model
{

class NodeList : public Node
{
  public:
    NodeList( const std::string& name, Lua& lua );
    NodeList( const std::string& name, Node& parent );
    virtual ~NodeList() = default;

    void add_node( Node::Pointer&& new_node );
    void delete_node( const std::string& name );
    void clear();

  private:
    std::unordered_map< std::string, Node::Pointer > m_nodes;
};

}
}

