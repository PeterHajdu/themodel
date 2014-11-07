#pragma once

#include <string>
#include <sol.hpp>

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

  private:
    Lua& m_lua;
    sol::table m_table;
};

}

}

