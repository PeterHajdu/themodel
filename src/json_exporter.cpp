#include <themodel/json_exporter.hpp>
#include <themodel/tree.hpp>
#include "json/json.h"

namespace the
{

namespace model
{

Json::Value
build_subtree( const TreeNode& subtree )
{
  Json::Value ret;
  if ( subtree.children().empty() )
  {
    return subtree.contains_meaningful_data() ?
        subtree.dump() :
        Json::Value();
  }

  for ( const auto& child : subtree.children() )
  {
    const std::string& subtree_name( child.first );
    ret[ subtree_name ] = build_subtree( child.second );
  }

  return ret;
}

std::string
export_json( const TreeNode& root )
{
  return build_subtree( root ).toStyledString();
}

}

}

