#pragma once

#include <themodel/helpers.hpp>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

namespace the
{

namespace model
{

class TreeNode
{
  public:
    TreeNode( std::string name )
      : name( std::move( name ) )
    {
    }

    TreeNode( std::string name, TreeNode& parent, OwnerType owner_type = OwnerType::owner )
      : name( std::move( name ) )
      , m_auto_register(
          owner_type == OwnerType::owner ?
            std::make_unique< AutoRegister >( *this, parent ) :
            nullptr )
    {
    }

    virtual ~TreeNode() = default;

    const std::string name;

    using Pointer = std::unique_ptr< TreeNode >;
    using Reference = std::reference_wrapper< TreeNode >;
    using Container = std::unordered_map< std::string, Reference >;

    const Container& children() const
    {
      return m_children;
    }

    virtual std::string dump() const = 0;

    //todo: Implement new value class to describe functions and nodes without value.
    //really ugly solution, does not scale well...
    virtual bool contains_meaningful_data() const
    {
      return false;
    }

  private:
    void register_child( TreeNode& child )
    {
      m_children.emplace( std::make_pair( child.name, Reference( child ) ) );
    }

    void remove_child( TreeNode& child )
    {
      m_children.erase( child.name );
    }

    Container m_children;

    class AutoRegister final
    {
      public:
        AutoRegister( TreeNode& child, TreeNode& parent )
          : child( child )
          , parent( parent )
        {
          parent.register_child( child );
        }

        ~AutoRegister()
        {
          parent.remove_child( child );
        }

        TreeNode& child;
        TreeNode& parent;
    };

    std::unique_ptr< AutoRegister > m_auto_register;
};

class NodeDescriptor
{
  public:
    Path path;
    TreeNode::Reference node;
};

using NodeInspector = std::function< void( const NodeDescriptor& ) >;

inline void
for_each_node( TreeNode& root_node, NodeInspector inspect )
{
  std::vector< NodeDescriptor > nodes_to_inspect{ {
    Path{ root_node.name },
    TreeNode::Reference( root_node ) } };

  while ( !nodes_to_inspect.empty() )
  {
    auto node_descritor( nodes_to_inspect.back() );
    nodes_to_inspect.pop_back();
    inspect( node_descritor );
    for ( const auto& child : node_descritor.node.get().children() )
    {
      Path child_path( node_descritor.path );
      child_path.push_back( child.second.get().name );
      nodes_to_inspect.emplace_back( NodeDescriptor{
           std::move( child_path ),
           TreeNode::Reference( child.second ) } );
    }

  }
}

}

}

