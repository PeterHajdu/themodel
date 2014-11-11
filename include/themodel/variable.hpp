#pragma once

#include <string>

#include <themodel/node.hpp>
#include <themodel/helpers.hpp>

namespace the
{

namespace model
{

template < typename T, typename OwningPolicy = Owner >
class Variable
{
  public:
    template < typename Parent >
    Variable( const std::string& name, T&& initial_value, Parent& parent )
      : m_value( initial_value )
      , m_parent_table( retrieve_table( parent ) )
      , m_name( name )
      , m_owning_policy( m_name, m_value, m_parent_table )
    {
      refresh_lua_value();
    }

    Variable( const Variable& ) = delete;
    Variable& operator=( const Variable& ) = delete;

    operator T() const
    {
      retrieve_value_from_lua();
      return m_value;
    }

    typedef Variable< T, OwningPolicy > MyType;
    MyType& operator=( const T& new_value )
    {
      m_value = new_value;
      refresh_lua_value();
      return *this;
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

    OwningPolicy m_owning_policy;
};

}

}

