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


Describe_Only( an_observer )
{
  void SetUp()
  {
    observed = std::make_unique< test::Observed >();
  }

  It ( can_be_observed )
  {
    const test::Observed* was_called_with{ nullptr };
    observed->observe(
        [ &was_called_with ]( const test::Observed& observed )
        {
          was_called_with = &observed;
        } );
    AssertThat( was_called_with == nullptr, Equals( true ) );
    observed->notify();
    AssertThat( was_called_with, Equals( observed.get() ) );
  }

  std::unique_ptr< test::Observed > observed;
};

