#pragma once

#include <themodel/node.hpp>
#include <themodel/helpers.hpp>
#include <string>

namespace the
{

namespace model
{

class Function : public TreeNode
{
  public:
    template < typename F, typename Parent >
    Function( std::string name_, Parent& parent, F function )
      : TreeNode( std::move( name_ ), parent )
      , m_parent_table( retrieve_table( parent ) )
      , m_deregister( name, m_parent_table )
    {
      m_parent_table.set_function( name, function );
    }

    Function( const Function& ) = delete;
    Function& operator=( const Function& ) = delete;

    virtual std::string dump() const override
    {
      return name;
    }

  private:
    sol::table m_parent_table;

    AutoDeregister m_deregister;
};

}

}

