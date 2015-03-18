#include <themodel/tree.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class TreeNode : public the::model::TreeNode
{
  public:
    TreeNode( std::string name )
      : the::model::TreeNode( std::move( name ) )
    {
    }

    TreeNode(
        std::string name,
        the::model::TreeNode& parent,
        the::model::OwnerType owner_type = the::model::OwnerType::owner )
      : the::model::TreeNode( std::move( name ), parent, owner_type )
    {
    }

    virtual std::string dump() const override final
    {
      return name;
    }

};

}

Describe( a_tree_node )
{
  void SetUp()
  {
    root_node = std::make_unique< test::TreeNode >( root_name );
  }

  It( has_a_name )
  {
    AssertThat( root_node->name, Equals( root_name ) );
  }

  It( has_children )
  {
    AssertThat( root_node->children(), IsEmpty() );
  }

  It( registers_to_its_parent )
  {
    const std::string child_name{ "a child" };
    test::TreeNode child_node( child_name, *root_node );
    AssertThat( root_node->children(), HasLength( 1u ) );
    AssertThat( &( root_node->children().at( child_name ).get() ), Equals( &child_node ) );
  }

  It( deregisters_it_self_when_destructed )
  {
    {
      test::TreeNode child_node( "a child", *root_node );
    }
    AssertThat( root_node->children(), IsEmpty() );
  }

  It( registers_only_if_it_is_an_owner )
  {
    const std::string child_name{ "a child" };
    test::TreeNode child_node( child_name, *root_node, the::model::OwnerType::reference );
    AssertThat( root_node->children(), IsEmpty() );
  }

  It( can_dump_its_content )
  {
    AssertThat( root_node->dump(), Equals( root_name ) );
  }

  const std::string root_name{ "root name" };
  std::unique_ptr< the::model::TreeNode > root_node;
};

Describe( on_each_tree_node )
{
  void build_tree()
  {
    root_node = std::make_unique< test::TreeNode >( root_name );
    for ( const auto& child_path : child_paths )
    {
      child_nodes.emplace_back( std::make_unique< test::TreeNode >( child_path.back(), *root_node ) );
    }
  }

  void SetUp()
  {
    visited_nodes.clear();
    build_tree();

    the::model::for_each_node( *root_node,
        [ &visited_nodes = visited_nodes ]( const the::model::NodeDescriptor& descriptor )
        {
          visited_nodes.push_back( descriptor.path );
        } );
  }

  It( calls_the_function_object_with_the_root_node )
  {
    AssertThat( visited_nodes, Contains( root_path ) );
  }

  It( calls_the_function_object_with_the_child_nodes )
  {
    for ( const auto& child_path : child_paths )
    {
      AssertThat( visited_nodes, Contains( child_path ) );
    }
  }

  const std::string root_name{ "root name" };
  const the::model::Path root_path{ root_name };
  std::unique_ptr< the::model::TreeNode > root_node;

  const std::vector< the::model::Path > child_paths{
    { root_name, "child 1" },
    { root_name, "child 2" } };

  std::vector< std::unique_ptr< the::model::TreeNode > > child_nodes;
  std::vector< the::model::Path > visited_nodes;
};

