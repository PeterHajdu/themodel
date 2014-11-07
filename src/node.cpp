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

}

}

