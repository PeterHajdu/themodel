#include <themodel/helpers.hpp>



namespace the
{

namespace model
{

const std::string path_from( const std::vector< std::string >& names )
{
  std::string concatenated_name;
  for ( const auto& name : names )
  {
    concatenated_name += name + ".";
  }
  concatenated_name.pop_back();
  return concatenated_name;
}

}

}

