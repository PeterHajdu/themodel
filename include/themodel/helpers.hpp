#pragma once

#include <string>
#include <vector>
#include <sol.hpp>

namespace the
{

namespace model
{

enum class OwnerType : int
{
  owner,
  reference
};

using Path = std::vector< std::string >;
const std::string path_from( const Path& names );

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

const std::string index_lua_table( const std::string& table, const std::string& key );

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

