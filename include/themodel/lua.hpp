#pragma once

#include <sol.hpp>
#include <string>

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

  private:
    sol::state m_lua_state;
    std::string m_last_error_message;
};

}

}

