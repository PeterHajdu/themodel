#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_node)
{
  Describe( a_root_node )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
      root_node.reset( new the::model::OwningNode( node_name, *lua ) );
    }

    It( is_exported_to_lua )
    {
      AssertThat( lua->assert_that( node_name ), Equals( true ) );
    }

    const std::string node_name{ "a_node" };
    std::unique_ptr< the::model::Lua > lua;
    std::unique_ptr< the::model::OwningNode > root_node;
  };

  Describe( a_child_node )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
      root_node.reset( new the::model::OwningNode( root_name, *lua ) );
    }

    It( can_be_registered_under_a_node )
    {
      the::model::OwningNode child_node( node_name, *root_node );
      AssertThat( lua->assert_that( child_path ), Equals( true ) );
    }

    It( should_deregister_itself_when_deleted )
    {
      {
        the::model::OwningNode child_node( node_name, *root_node );
        AssertThat( lua->assert_that( child_path ), Equals( true ) );
      }

      AssertThat( lua->assert_that( child_path ), Equals( false ) );
    }

    It( exposes_its_name )
    {
      AssertThat( root_node->name(), Equals( root_name ) );
    }

    const std::string root_name{ "the_root" };
    const std::string node_name{ "a_node" };
    const std::string child_path{ the::model::path_from( { root_name, node_name } ) };
    std::unique_ptr< the::model::Lua > lua;
    std::unique_ptr< the::model::OwningNode > root_node;
  };

  Describe( non_owning_behavior )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
      original_node.reset( new the::model::OwningNode( node_name, *lua ) );
    }

    It( can_refer_to_an_existing_node_without_interfering )
    {
      the::model::ReferenceNode node_2( node_name, *lua );
      the::model::OwningNode child( child_name, *original_node );
      AssertThat( lua->assert_that( child_path ), Equals( true ) );
    }

    It( can_register_children )
    {
      the::model::ReferenceNode node_2( node_name, *lua );
      the::model::OwningNode child( child_name, node_2 );
      AssertThat( lua->assert_that( child_path ), Equals( true ) );
    }

    It( does_not_deregister_the_node_when_deleted )
    {
      {
        the::model::ReferenceNode node_2( node_name, *lua );
      }
      AssertThat( lua->assert_that( node_name ), Equals( true ) );
    }

    const std::string node_name{ "a_node" };
    const std::string child_name{ "a_child" };
    const std::string child_path{ the::model::path_from( { node_name, child_name } ) };
    std::unique_ptr< the::model::Lua > lua;
    std::unique_ptr< the::model::OwningNode > original_node;
  };

};

