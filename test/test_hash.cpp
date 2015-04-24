#include <themodel/hash.hpp>
#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;


Describe(a_hash)
{
  void SetUp()
  {
    lua = std::make_unique< the::model::Lua >();
    node = std::make_unique< the::model::OwningNode >( node_name, *lua );
    hash = std::make_unique< the::model::Hash< std::string, std::string > >( hash_name, *node );
    (*hash)[ key ] = value;
  }

  void assert_has_value( const std::string& key, const std::string& value )
  {
    AssertThat( static_cast< std::string >( (*hash)[ key ] ) , Equals( value ) );
    AssertThat(
        lua->assert_equals( the::model::path_from( { node_name, hash_name, key } ), value ),
        Equals( true ) );
  }

  It( creates_the_key_if_it_did_not_exist )
  {
    AssertThat( lua->assert_that( hash_path ), Equals( true ) );
    AssertThat( lua->assert_that( key_path ), Equals( true ) );
  }

  It( allows_assignement_via_bracket_operator )
  {
    assert_has_value( key, value );
  }

  It( holds_many_elements )
  {
    const std::string another_key( "another_key" );
    const std::string another_value( "another value" );
    (*hash)[ another_key ] = another_value;
    assert_has_value( another_key, another_value );
  }

  It( can_check_key_existence )
  {
    AssertThat( hash->has( key ), Equals( true ) );
    AssertThat( hash->has( "unknownkey" ), Equals( false ) );
  }

  It( has_a_getter_function_that_returns_with_the_contained_value )
  {
    AssertThat( hash->get( key ), Equals( value ) );
  }

  It( allows_iteration_with_range_based_for_loop )
  {
    size_t iterations{ 0 };
    std::vector< std::string > keys;
    std::vector< std::string > values;
    for ( const auto& element : (*hash) )
    {
      ++iterations;
      keys.push_back( element.first );
      values.push_back( *element.second );
    }

    AssertThat( iterations, Equals( 1u ) );
    AssertThat( keys, Contains( key ) );
    AssertThat( values, Contains( value ) );
  }

  const std::string node_name{ "a_node" };
  const std::string hash_name{ "a_variable" };
  const std::string hash_path{ the::model::path_from( { node_name, hash_name } ) };
  const std::string key{ "key" };
  const std::string value{ "value" };
  const std::string key_path{ the::model::path_from( { node_name, hash_name, key } ) };
  std::unique_ptr< the::model::Lua > lua;
  std::unique_ptr< the::model::OwningNode > node;
  std::unique_ptr< the::model::Hash< std::string, std::string > > hash;
};

