#pragma once

#include <string>
#include <sol.hpp>
#include <themodel/helpers.hpp>

namespace the
{

namespace model
{

class Lua;

class Node
{
  public:
    Node( const std::string& name, Lua& lua );
    Node( const std::string& name, Node& parent );
    Node( const Node& ) = delete;
    Node& operator=( const Node& ) = delete;

  private:
    void register_to( sol::table& );

    Lua& m_lua;
    sol::table m_table;
    sol::table m_parent_table;
    const std::string m_name;

    template < typename T >
    friend class Variable;

    friend class Function;

    AutoDeregister m_deregister;
};

}

}

