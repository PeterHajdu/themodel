#include <themodel/node.hpp>
#include <themodel/lua.hpp>

namespace the
{

namespace model
{

Node::Node( const std::string& name, Lua& lua )
  : m_lua( lua )
  , m_table( m_lua.state().create_table() )
{
  m_lua.state().set( name, m_table );
}


Node::Node( const std::string& name, Node& parent )
  : m_lua( parent.m_lua )
  , m_table( m_lua.state().create_table() )
{
  parent.m_table.set( name, m_table );
}

}

}

