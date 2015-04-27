#pragma once

#include <string>

#include <themodel/node.hpp>
#include <themodel/helpers.hpp>
#include <themodel/observer.hpp>

namespace the
{

namespace model
{

template < typename T, typename OwningPolicy = Owner >
class Variable :
  public TreeNode,
  public Observable< Variable< T, OwningPolicy > >
{
  public:
    using MyType = Variable< T, OwningPolicy >;

    template < typename Parent, typename U >
    Variable( std::string name_, U&& initial_value, Parent& parent )
      : TreeNode( std::move( name_ ), parent, OwningPolicy::owner_type )
      , m_value( std::forward<U>( initial_value ) )
      , m_parent_table( retrieve_table( parent ) )
      , m_owning_policy( name, m_value, m_parent_table )
    {
      refresh_lua_value();
    }

    Variable( const Variable& ) = delete;
    Variable& operator=( const Variable& ) = delete;

    T get() const
    {
      retrieve_value_from_lua();
      return m_value;
    }

    operator T() const
    {
      return get();
    }

    using Observable< MyType >::notify;

    template < typename U >
    MyType& operator=( U&& new_value )
    {
      m_value = std::forward< U >( new_value );
      refresh_lua_value();
      notify();
      return *this;
    }

    virtual std::string dump() const override
    {
      return to_lua_string( m_value );
    }

    virtual bool contains_meaningful_data() const override
    {
      return true;
    }

  private:
    void refresh_lua_value()
    {
      m_parent_table.set( name, m_value );
    }

    void retrieve_value_from_lua() const
    {
      m_value = m_parent_table.get< T >( name );
    }

    mutable T m_value;
    sol::table m_parent_table;

    OwningPolicy m_owning_policy;
};

}

}

