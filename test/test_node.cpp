#include <themodel/node.hpp>
#include <themodel/lua.hpp>
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
};

