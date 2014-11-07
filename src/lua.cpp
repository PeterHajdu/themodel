#include <themodel/lua.hpp>

namespace the
{

namespace model
{

Lua::Lua()
{
  m_lua_state.open_libraries( sol::lib::base );
}

bool
Lua::run( const std::string& script )
{
  try
  {
    m_lua_state.script( script );
  }
  catch ( std::exception& e )
  {
    m_last_error_message = e.what();
    return false;
  }

  return true;
}

bool
Lua::assert_that( const std::string& statement )
{
  return run( std::string( "assert( " ) + statement + " )" );
}

sol::state&
Lua::state()
{
  return m_lua_state;
}

const std::string&
Lua::error_message() const
{
  return m_last_error_message;
}

}

}

