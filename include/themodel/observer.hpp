#pragma once

#include <functional>

namespace the
{

namespace model
{

template < typename Model >
using Observer = std::function< void( const Model& ) >;

template < typename Model >
class Observable
{
  public:
    using observer_type = Observer< Model >;
    void observe( observer_type observer ) const
    {
      m_observers.emplace_back( std::move( observer ) );
    }

  protected:
    void notify() const
    {
      for ( const auto& observer : m_observers )
      {
        observer( static_cast< const Model& >( *this ) );
      }
    }

  private:
    mutable std::vector< observer_type > m_observers;
};

}

}

