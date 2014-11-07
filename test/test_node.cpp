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
      root_node.reset( new the::model::Node( node_name, *lua ) );
    }

    It( is_exported_to_lua )
    {
      AssertThat( lua->assert_that( node_name ), Equals( true ) );
    }

    const std::string node_name{ "a_node" };
    std::unique_ptr< the::model::Lua > lua;
    std::unique_ptr< the::model::Node > root_node;
  };

  Describe( a_child_node )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
      root_node.reset( new the::model::Node( root_name, *lua ) );
    }

    It( can_be_registered_under_a_node )
    {
      the::model::Node child_node( node_name, *root_node );
      AssertThat( lua->assert_that( child_path ), Equals( true ) );
    }

    It( should_deregister_itself_when_deleted )
    {
      {
        the::model::Node child_node( node_name, *root_node );
        AssertThat( lua->assert_that( child_path ), Equals( true ) );
      }

      AssertThat( lua->assert_that( child_path ), Equals( false ) );
    }

    const std::string root_name{ "the_root" };
    const std::string node_name{ "a_node" };
    const std::string child_path{ the::model::path_from( { root_name, node_name } ) };
    std::unique_ptr< the::model::Lua > lua;
    std::unique_ptr< the::model::Node > root_node;
  };
};

