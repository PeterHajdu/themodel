#include <themodel/lua.hpp>
#include <sol.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_lua)
{

  Describe( initialization )
  {

    It( has_default_constructor )
    {
      the::model::Lua lua;
      (void)lua;
    }

  };

  Describe( script_execution )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
    }

    It( can_run_lua_code )
    {
      lua->run( "something = 123" );
      AssertThat( lua->state().get< int >( "something" ), Equals( 123 ) );
    }

    It( returns_false_if_the_script_fails )
    {
      AssertThat( lua->run( "assert(false)" ), Equals( false ) );
    }

    It( stores_the_last_error_message )
    {
      AssertThat( lua->run( "assert(false)" ), Equals( false ) );
      AssertThat( lua->error_message(), Contains( "assertion failed" ) );
    }

    It( returns_true_if_the_script_succeeds )
    {
      AssertThat( lua->run( "assert(true)" ), Equals( true ) );
    }

    It( has_explicit_assert_execution )
    {
      AssertThat( lua->assert_that( "true" ), Equals( true ) );
      AssertThat( lua->assert_that( "false" ), Equals( false ) );
    }

    It( can_check_equation )
    {
      AssertThat( lua->assert_equals( "true", true ), Equals( true ) );
      AssertThat( lua->assert_equals( "false", true ), Equals( false ) );
    }

    std::unique_ptr< the::model::Lua > lua;
  };

  Describe( default_libraries )
  {
    void SetUp()
    {
      lua.reset( new the::model::Lua() );
    }

    It( loads_base_by_default )
    {
      lua->run( "assert( true )" );
    };

    std::unique_ptr< the::model::Lua > lua;
  };

};

