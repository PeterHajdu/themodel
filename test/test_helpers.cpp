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

Describe(to_lua_string_converter)
{
  It( converts_all_values_except_strings_to_its_string_representative )
  {
    AssertThat( the::model::to_lua_string( 123 ), Equals( "123" ) );
    AssertThat( the::model::to_lua_string( -12.3 ), Contains( "-12.3" ) );
    AssertThat( the::model::to_lua_string( true ), Contains( "true" ) );
  }

  It( adds_quotation_marks_around_strings )
  {
    AssertThat( the::model::to_lua_string( std::string( "dog" ) ), Equals( "\"dog\"" ) );
  }

};

Describe(table_element_indexer)
{
  It( constructs_the_string_from_the_table_name_and_the_key )
  {
    AssertThat(
        the::model::index_lua_table( "table", "1234" ),
        Equals( "table[\"1234\"]" ) );
  }
};

