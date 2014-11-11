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

class NodeBase
{
  public:
    typedef std::unique_ptr< NodeBase > Pointer;
    NodeBase( const std::string& name )
      : m_name( name )
    {
    }

    const std::string& name() const
    {
      return m_name;
    }

    virtual ~NodeBase() = default;

  protected:
    const std::string m_name;
};


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
class Node : public NodeBase
{
  public:
    template< typename Parent >
    Node( const std::string& name, Parent& parent )
      : NodeBase( name )
      , m_lua( retrieve_lua( parent ) )
      , m_table( m_lua.state().create_table() )
      , m_parent_table( retrieve_table( parent ) )
      , m_owning_policy( m_name, m_table, m_parent_table )
    {
    }


    Node( const Node& ) = delete;
    Node& operator=( const Node& ) = delete;

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

