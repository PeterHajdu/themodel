#include <themodel/helpers.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_path_calculator)
{
  It( generates_path_from_a_vector_of_strings )
  {
    AssertThat( the::model::path_from( { "dog", "food" } ), Equals( "dog.food" ) );
  }
};

