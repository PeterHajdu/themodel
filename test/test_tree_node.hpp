#pragma once

#include <themodel/tree.hpp>
namespace test
{

class TreeNode : public the::model::TreeNode
{
  public:
    TreeNode( std::string name )
      : the::model::TreeNode( std::move( name ) )
    {
    }

    TreeNode(
        std::string name,
        the::model::TreeNode& parent,
        the::model::OwnerType owner_type = the::model::OwnerType::owner )
      : the::model::TreeNode( std::move( name ), parent, owner_type )
    {
    }

    virtual std::string dump() const override final
    {
      return name;
    }

};

}


