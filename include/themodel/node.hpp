#pragma once

#include <sol.hpp>
#include <themodel/helpers.hpp>
#include <themodel/lua.hpp>
#include <string>
#include <memory>

namespace the
{

namespace model
{

class Owner final
{
  public:
    template< typename T >
    Owner( const std::string& name, T& table, sol::table& parent_table )
      : name( name )
      , parent_table( parent_table )
    {
      parent_table.set( name, table );
    }

    ~Owner()
    {
      parent_table.set( name, sol::nil );
    }

    static const OwnerType owner_type;
  private:
    const std::string& name;
    sol::table& parent_table;
};

class Reference final
{
  public:
    template< typename T >
    Reference( const std::string& name, T& table, sol::table& parent_table )
    {
      table = parent_table.get< T >( name );
    }
    static const OwnerType owner_type;
};

template < typename Parent >
Lua& retrieve_lua( Parent& parent )
{
  return parent.m_lua;
}

template <>
inline Lua& retrieve_lua< Lua >( Lua& parent )
{
  return parent;
}

template < typename Parent >
sol::table retrieve_table( Parent& parent )
{
  return parent.m_table;
}

template <>
inline sol::table retrieve_table< Lua >( Lua& parent )
{
  return parent.state().global_table();
}

template < class OwningPolicy >
class Node : public TreeNode
{
  public:
    template< typename Parent >
    Node( std::string name_, Parent& parent )
      : TreeNode( std::move( name_ ), parent, OwningPolicy::owner_type )
      , m_lua( retrieve_lua( parent ) )
      , m_table( m_lua.state().create_table() )
      , m_parent_table( retrieve_table( parent ) )
      , m_owning_policy( name, m_table, m_parent_table )
    {
    }

    Node( const Node& ) = delete;
    Node& operator=( const Node& ) = delete;

    virtual std::string dump() const override
    {
      return name;
    }

    virtual ~Node() = default;
  private:
    void register_to( sol::table& );

    Lua& m_lua;
    sol::table m_table;
    sol::table m_parent_table;

    template < typename T, typename OP >
    friend class Variable;
    friend class Function;


    template < typename Parent >
    friend sol::table retrieve_table( Parent& parent );
    template < typename Parent >
    friend Lua& retrieve_lua( Parent& parent );

    OwningPolicy m_owning_policy;
};

typedef Node< Owner > OwningNode;
typedef Node< Reference > ReferenceNode;

}

}

