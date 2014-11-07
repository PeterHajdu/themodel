#pragma once

#include <string>

#include <themodel/node.hpp>
#include <themodel/helpers.hpp>

namespace the
{

namespace model
{

class Function
{
  public:
    template < typename F >
    Function( const std::string& name, Node& parent, F function )
      : m_parent_table( parent.m_table )
      , m_name( name )
      , m_deregister( m_name, m_parent_table )
    {
      m_parent_table.set_function( m_name, function );
    }

    Function( const Function& ) = delete;
    Function& operator=( const Function& ) = delete;

  private:
    sol::table m_parent_table;
    const std::string m_name;

    AutoDeregister m_deregister;
};

}

}

