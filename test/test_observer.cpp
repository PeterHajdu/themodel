#include <themodel/observer.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class Observed : public the::model::Observable< Observed >
{
  public:
    using Observable< Observed >::notify;
};

}


Describe( an_observer )
{
  void SetUp()
  {
    observed = std::make_unique< test::Observed >();
    was_called_with = nullptr;
  }

  void assert_observable()
  {
    AssertThat( was_called_with == nullptr, Equals( true ) );
    observed->notify();
    AssertThat( was_called_with, Equals( observed.get() ) );
  }

  It ( can_be_observed )
  {
    observed->observe( observer );
    assert_observable();
  }

  It ( has_auto_observer )
  {
    auto auto_observer( observed->auto_observe( observer ) );
    assert_observable();
    (void)auto_observer;
  }

  It ( does_not_notify_deleted_auto_observers )
  {
    {
      auto auto_observer( observed->auto_observe( observer ) );
      (void)auto_observer;
    }
    observed->notify();
    AssertThat( was_called_with == nullptr, Equals( true ) );
  }

  It ( does_allow_default_auto_observer_creation )
  {
    test::Observed::auto_observer_type observer;
  }

  It ( does_allow_move_assignment_of_auto_observers )
  {
    test::Observed::auto_observer_type auto_observer;
    auto_observer = observed->auto_observe( observer );
    assert_observable();
  }

  It ( moves_properly_old_observer_when_move_constructed )
  {
    std::unique_ptr< test::Observed::auto_observer_type > second_observer;
    {
      auto first_observer( observed->auto_observe( observer ) );
      second_observer = std::make_unique< test::Observed::auto_observer_type >( std::move( first_observer ) );
    }
    assert_observable();
  }

  It ( removes_previous_auto_observer_when_new_one_is_assigned )
  {
    bool was_new_observer_called{ false };
    auto auto_observer( observed->auto_observe( observer ) );
    auto_observer = observed->auto_observe(
        [ &was_new_observer_called ]( const test::Observed& )
        {
          was_new_observer_called = true;
        } );

    observed->notify();
    AssertThat( was_new_observer_called, Equals( true ) );

    const bool was_old_observer_called{ was_called_with!=nullptr };
    AssertThat( was_old_observer_called, Equals( false ) );
  }

  std::unique_ptr< test::Observed > observed;
  const test::Observed* was_called_with{ nullptr };

  test::Observed::observer_type observer{
          [ this ]( const test::Observed& observed )
          {
            was_called_with = &observed;
          } };
};

