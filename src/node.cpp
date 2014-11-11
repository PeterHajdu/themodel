#include <themodel/node.hpp>
#include <themodel/lua.hpp>

namespace the
{

namespace model
{

Node::Node( const std::string& name, Retriever retriever )
  : m_lua( retriever.lua )
  , m_table( m_lua.state().create_table() )
  , m_parent_table( retriever.parent )
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

