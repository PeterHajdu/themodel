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
    node.reset( new the::model::OwningNode( node_name, *lua ) );
    variable.reset( new the::model::Variable< int >( variable_name, int( initial_value ), *node ) );
  }

  It( holds_meaningful_data )
  {
    AssertThat( variable->contains_meaningful_data(), Equals( true ) );
  }

  It( can_refer_to_an_exported_value )
  {
    the::model::Variable< int, the::model::Reference > new_variable( variable_name, initial_value + 10, *node );
    assert_has_value( initial_value );
    new_variable = initial_value + 10;
    assert_has_value( initial_value + 10 );
  }

  It( registers_itself_to_the_global_table )
  {
    the::model::Variable< int > variable( variable_name, int( initial_value ), *lua );
    AssertThat( lua->assert_that( variable_name ), Equals( true ) );
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
    AssertThat( lua->assert_equals( variable_path, expected_value ), Equals( true ) );
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

  template < typename T >
  void assert_works_with( const T& initial_value, const T& test_value )
  {
    const std::string name{ "the_variable_name" };
    const std::string path{ the::model::path_from( { node_name, name } ) };
    the::model::Variable< T > the_variable( name, T( initial_value ), *node );
    AssertThat( T( the_variable ), Equals( initial_value ) );
    AssertThat( lua->assert_equals( path, initial_value ), Equals( true ) );

    the_variable = test_value;
    AssertThat( T( the_variable ), Equals( test_value ) );
    AssertThat( lua->assert_equals( path, test_value ), Equals( true ) );
  }

  It( works_with_string )
  {
    assert_works_with< std::string >( "an initial value", "a test value" );
  }

  It( works_with_double )
  {
    assert_works_with< double >( 123.123, -834.12 );
  }

  It( works_with_bool )
  {
    assert_works_with< bool >( true, false );
  }

  It( is_a_tree_node )
  {
    the::model::TreeNode& as_tree_node( *variable );
    (void)as_tree_node;
  }

  It( dumps_its_value_as_string )
  {
    AssertThat( variable->dump(), Equals( std::to_string( initial_value ) ) );
  }

  const int initial_value{ 10 };
  const std::string node_name{ "a_node" };
  const std::string variable_name{ "a_variable" };
  const std::string variable_path{ the::model::path_from( { node_name, variable_name } ) };
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::OwningNode > node;
  std::unique_ptr< the::model::Variable< int > > variable;
};

