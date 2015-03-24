#include "test_tree_node.hpp"
#include <themodel/json_exporter.hpp>
#include "../src/json/json.h"
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe_Only(a_json_exporter)
{

  Json::Value export_tree( const the::model::TreeNode& root )
  {
    try
    {
      std::stringstream exported_model( the::model::export_json( root ) );
      std::cout << "exported model is: " << exported_model.str() << std::endl;
      Json::Value json_root;
      exported_model >> json_root;
      return json_root;
    } catch ( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }
  }

  It( creates_json_documents )
  {
    auto exported_tree( export_tree( test::TreeNode( root_name ) ) );
  }

  It( exports_child_nodes_as_objects )
  {
    test::TreeNode root( root_name, a_value );
    test::TreeNode child( child_name, another_value, root );
    auto exported_tree( export_tree( root ) );
    AssertThat( exported_tree.isMember( child_name ), Equals( true ) );
    AssertThat( exported_tree[ child_name ].asString(), Equals( another_value ) );
  }

  It( exports_empty_non_variable_nodes_as_null )
  {
    auto exported_tree( export_tree( test::TreeNode( root_name ) ) );
    AssertThat( exported_tree.isNull(), Equals( true ) );
  }

  const std::string root_name{ "root" };
  const std::string a_value{ "a simple string value" };

  const std::string child_name{ "child" };
  const std::string another_value{ "another value" };
};

