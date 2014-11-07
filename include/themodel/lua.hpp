#pragma once

#include <sol.hpp>
#include <string>
#include <themodel/helpers.hpp>

namespace the
{

namespace model
{

class Lua
{
  public:
    Lua();
    Lua( const Lua& ) = delete;
    Lua& operator=( const Lua& ) = delete;

    bool run( const std::string& script );
    bool assert_that( const std::string& statement );
    sol::state& state();
    const std::string& error_message() const;

    template < typename T >
    bool assert_equals( const std::string& a, const T& with );

  private:
    sol::state m_lua_state;
    std::string m_last_error_message;
};

template < typename T >
bool Lua::assert_equals( const std::string& a, const T& with )
{
  return Lua::assert_that( a + " == " + to_lua_string( with ) );
}

}

}

