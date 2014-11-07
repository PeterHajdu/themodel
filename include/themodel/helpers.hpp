#pragma once

#include <string>
#include <vector>
#include <sol.hpp>

namespace the
{

namespace model
{

const std::string path_from( const std::vector< std::string >& names );

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

