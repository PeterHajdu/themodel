#pragma once

#include <string>

#include <themodel/node.hpp>
#include <themodel/helpers.hpp>

namespace the
{

namespace model
{

template < typename T >
class Variable
{
  public:
    Variable( const std::string& name, T&& initial_value, Node& parent )
      : m_value( initial_value )
      , m_parent_table( parent.m_table )
      , m_name( name )
      , m_deregister( m_name, m_parent_table )
    {
      refresh_lua_value();
    }

    operator T() const
    {
      retrieve_value_from_lua();
      return m_value;
    }

    T& operator=( const T& new_value )
    {
      m_value = new_value;
      refresh_lua_value();
      return m_value;
    }

  private:
    void refresh_lua_value()
    {
      m_parent_table.set( m_name, m_value );
    }

    void retrieve_value_from_lua() const
    {
      m_value = m_parent_table.get< T >( m_name );
    }

    mutable T m_value;
    sol::table m_parent_table;
    const std::string m_name;

    AutoDeregister m_deregister;
};

}

}

