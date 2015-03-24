#pragma once

#include <themodel/tree.hpp>
namespace test
{

class TreeNode : public the::model::TreeNode
{
  public:
    TreeNode( std::string name )
      : the::model::TreeNode( std::move( name ) )
      , value()
    {
    }

    TreeNode( std::string name, std::string value )
      : the::model::TreeNode( std::move( name ) )
      , value( std::move( value ) )
    {
    }

    TreeNode(
        std::string name,
        the::model::TreeNode& parent,
        the::model::OwnerType owner_type = the::model::OwnerType::owner )
      : the::model::TreeNode( std::move( name ), parent, owner_type )
      , value()
    {
    }

    TreeNode(
        std::string name,
        std::string value,
        the::model::TreeNode& parent,
        the::model::OwnerType owner_type = the::model::OwnerType::owner )
      : the::model::TreeNode( std::move( name ), parent, owner_type )
      , value( std::move( value ) )
    {
    }

    virtual std::string dump() const override final
    {
      return value;
    }

    virtual bool contains_meaningful_data() const override
    {
      return value != "";
    }

    std::string value;
};

}

