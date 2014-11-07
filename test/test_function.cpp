#include <themodel/function.hpp>
#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{
  //stupid sol bug
  const int return_value{ 10 };
  bool was_called;
  void call()
  {
    was_called = true;
  }
}

Describe(a_function)
{
  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    node.reset( new the::model::Node( node_name, *lua ) );

    was_called = false;
    function.reset( new the::model::Function(
         function_name,
         *node,
         []()
         {
           call();
           return return_value;
         } ) );
  }

  It( registers_itself_to_the_parent_node )
  {
    AssertThat( lua->assert_that( function_path ), Equals( true ) );
  }

  It( deregisters_itself_when_deleted )
  {
    function.reset();
    AssertThat( lua->assert_that( function_path ), Equals( false ) );
  }

  It( can_be_called_from_lua )
  {
    AssertThat( lua->run( function_path + "()" ), Equals( true ) );
    AssertThat( was_called, Equals( true ) );
  }

  It( works_with_parameters )
  {
    function.reset();
    function.reset( new the::model::Function(
         function_name,
         *node,
         []( int value, const std::string& a_string )
         {
           call();
           assert( value == 10 );
           assert( a_string == "something" );
         } ) );

    AssertThat( lua->run( function_path + "( 10, \"something\" )" ), Equals( true ) );
    AssertThat( was_called, Equals( true ) );
  }

  It( returns_a_value )
  {
    AssertThat( lua->run( std::string( "dogfood = " ) + function_path + "()" ), Equals( true ) );
    AssertThat( lua->assert_equals( "dogfood", return_value ), Equals( true ) );
  }

  const std::string node_name{ "a_node" };
  const std::string function_name{ "a_function" };
  const std::string function_path{ the::model::path_from( { node_name, function_name } ) };
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::Node > node;
  std::unique_ptr< the::model::Function > function;
};

