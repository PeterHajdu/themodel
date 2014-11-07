#pragma once

#include <string>
#include <vector>
#include <sol.hpp>

namespace the
{

namespace model
{

const std::string path_from( const std::vector< std::string >& names );

template < typename T >
const std::string to_lua_string( const T& value )
{
  return std::to_string( value );
}

template <>
inline const std::string to_lua_string< bool >( const bool& value )
{
  return value ? "true" : "false";
}

template <>
inline const std::string to_lua_string< std::string >( const std::string& value )
{
  return std::string( "\"" ) + value + "\"";
}

class AutoDeregister final
{
  public:
    AutoDeregister( const std::string& name, sol::table& parent_table )
      : name( name )
      , parent_table( parent_table )
    {
    }

    ~AutoDeregister()
    {
      parent_table.set( name, sol::nil );
    }

  private:
    const std::string& name;
    sol::table& parent_table;
};

}

}

