#include <themodel/variable.hpp>
#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_variable)
{
  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    node.reset( new the::model::Node( node_name, *lua ) );
  }

  It( is_instantiable )
  {
    the::model::Variable variable;
    (void)variable;
  }

  const std::string node_name{ "a_node" };
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::Node > node;
};

