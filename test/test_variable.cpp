#include <themodel/variable.hpp>
#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_variable)
{
  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    node.reset( new the::model::Node( node_name, *lua ) );
    variable.reset( new the::model::Variable< int >( variable_name, int( initial_value ), *node ) );
  }

  It( registers_itself_to_the_parent_node )
  {
    AssertThat( lua->assert_that( variable_path ), Equals( true ) );
  }

  It( implicitly_converts_to_T )
  {
    AssertThat( 5 + *variable, Equals( initial_value + 5 ) );
  }

  void assert_has_value( int expected_value )
  {
    AssertThat( int( *variable ), Equals( expected_value ) );
    AssertThat( lua->assert_that( variable_path + " == " + std::to_string( expected_value ) ), Equals( true ) );
  }

  It( has_an_initial_value )
  {
    assert_has_value( initial_value );
  }

  It( has_assignement_operator )
  {
    *variable = 123;
    assert_has_value( 123 );
  }

  It( allows_value_setting_from_lua )
  {
    const std::string command( variable_path + " = 912" );
    AssertThat( lua->run( command ), Equals( true ) );
    assert_has_value( 912 );
  }

  It( deregisters_itself_when_deleted )
  {
    variable.reset();
    AssertThat( lua->assert_that( variable_path ), Equals( false ) );
  }

  const int initial_value{ 10 };
  const std::string node_name{ "a_node" };
  const std::string variable_name{ "a_variable" };
  const std::string variable_path{ the::model::path_from( { node_name, variable_name } ) };
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::Node > node;
  std::unique_ptr< the::model::Variable< int > > variable;
};

