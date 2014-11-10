#include <themodel/node_list.hpp>
#include <themodel/node.hpp>
#include <themodel/lua.hpp>
#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace
{

const std::string
table_element( const std::string& table_name, const std::string& key )
{
  return table_name + "[\"" + key + "\"]";
}

}

Describe( a_node_list )
{

  void SetUp()
  {
    lua.reset( new the::model::Lua() );
    nodelist.reset( new the::model::NodeList( model_name, *lua ) );
    nodelist->add_node( the::model::Node::Pointer( new the::model::Node( new_node_name, *nodelist ) ) );
    nodelist->add_node( the::model::Node::Pointer( new the::model::Node( another_new_node_name, *nodelist ) ) );
  }

  void TearDown()
  {
    nodelist.reset();
  }

  It( is_a_node )
  {
    the::model::Node& node_reference( *nodelist );
    (void)node_reference;
  }

  It( exports_itself_as_a_node )
  {
    AssertThat( lua->assert_that( model_name ), Equals( true ) );
  }

  It( deregisters_itself_when_deleted )
  {
    nodelist.reset();
    AssertThat( lua->assert_that( model_name ), Equals( false ) );
  }

  It( stores_other_nodes )
  {
    AssertThat( lua->assert_that( table_element( model_name, new_node_name ) ), Equals( true ) );
    AssertThat( lua->assert_that( table_element( model_name, another_new_node_name ) ), Equals( true ) );
  }

  It( can_delete_node_by_name )
  {
    nodelist->delete_node( new_node_name );
    AssertThat( lua->assert_that( table_element( model_name, new_node_name ) ), Equals( false ) );
  }

  It( handles_nonexisting_key_deletion )
  {
    nodelist->delete_node( "unknown_key" );
  }

  It( deletes_all_nodes_when_cleared )
  {
    nodelist->clear();
    AssertThat( lua->assert_that( table_element( model_name, new_node_name ) ), Equals( false ) );
    AssertThat( lua->assert_that( table_element( model_name, another_new_node_name ) ), Equals( false ) );
  }

  std::unique_ptr< the::model::NodeList > nodelist;
  std::unique_ptr< the::model::Lua > lua;
  const std::string model_name{ "the_nodelist" };

  const std::string new_node_name{ "a_new_node" };
  const std::string another_new_node_name{ "another_new_node" };
};

