#include <themodel/node.hpp>
#include <themodel/lua.hpp>

namespace the
{

namespace model
{

Node::Node( const std::string& name, Lua& lua )
  : m_lua( lua )
  , m_table( m_lua.state().create_table() )
  , m_parent_table( m_lua.state().global_table() )
  , m_name( name )
  , m_deregister( m_name, m_parent_table )
{
  register_to( m_parent_table );
}


Node::Node( const std::string& name, Node& parent )
  : m_lua( parent.m_lua )
  , m_table( m_lua.state().create_table() )
  , m_parent_table( parent.m_table )
  , m_name( name )
  , m_deregister( m_name, m_parent_table )
{
  register_to( m_parent_table );
}

void
Node::register_to( sol::table& table )
{
  table.set( m_name, m_table );
}

const std::string&
Node::name() const
{
  return m_name;
}

}

}

