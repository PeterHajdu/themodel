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

class Retriever;

class Node
{
  public:
    typedef std::unique_ptr< Node > Pointer;

    Node( const std::string& name, Retriever );
    Node( const Node& ) = delete;
    Node& operator=( const Node& ) = delete;
    const std::string& name() const;

    virtual ~Node() = default;
  private:
    void register_to( sol::table& );

    Lua& m_lua;
    sol::table m_table;
    sol::table m_parent_table;
    const std::string m_name;

    template < typename T >
    friend class Variable;
    friend class Function;
    friend class Retriever;

    AutoDeregister m_deregister;
};

class Retriever
{
  public:
    Retriever( Lua& lua )
      : lua( lua )
      , parent( lua.state().global_table() )
    {
    }

    Retriever( Node& node )
      : lua( node.m_lua )
      , parent( node.m_table )
    {
    }


    Lua& lua;
    sol::table parent;
};

}

}

