#pragma once

#include <functional>
#include <vector>
#include <unordered_map>

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
    using my_type = Observable< Model >;
    using observer_type = Observer< Model >;
    class AutoObserver
    {
      public:
        AutoObserver()
          : m_observed{ nullptr }
          , m_id( 0 )
        {
        }

        AutoObserver( const my_type& observed, size_t id )
          : m_observed( &observed )
          , m_id( id )
        {
        }

        ~AutoObserver()
        {
          remove_observer_if_initialized();
        }

        AutoObserver( const AutoObserver& ) = delete;
        AutoObserver& operator=( const AutoObserver& ) = delete;

        AutoObserver( AutoObserver&& other )
          : m_observed( other.m_observed )
          , m_id( other.m_id )
        {
          other.m_observed = nullptr;
        }

        AutoObserver& operator=( AutoObserver&& other )
        {
          if ( this == &other )
          {
            return *this;
          }

          remove_observer_if_initialized();
          m_id = other.m_id;
          std::swap( m_observed, other.m_observed );
          return *this;
        }

      private:
        bool is_initialized() const
        {
          return m_observed != nullptr;
        }

        void remove_observer_if_initialized()
        {
          if ( !is_initialized() )
          {
            return;
          }

          m_observed->remove_auto_observer( m_id );
          m_observed = nullptr;
        }

        const my_type* m_observed;
        size_t m_id;
    };

    using auto_observer_type = AutoObserver;

    Observable()
      : m_auto_observers_next_index( 0u )
    {
    }

    void observe( observer_type observer ) const
    {
      m_observers.emplace_back( std::move( observer ) );
    }

    AutoObserver auto_observe( observer_type observer ) const
    {
      m_auto_observers.emplace( m_auto_observers_next_index++, observer );
      return AutoObserver( *this, m_auto_observers_next_index - 1 );
    }

  protected:
    void notify() const
    {
      for ( const auto& observer : m_observers )
      {
        observer( static_cast< const Model& >( *this ) );
      }

      for ( const auto& observer : m_auto_observers )
      {
        observer.second( static_cast< const Model& >( *this ) );
      }
    }

  private:
    void remove_auto_observer( size_t id ) const
    {
      m_auto_observers.erase( id );
    }

    mutable std::vector< observer_type > m_observers;
    mutable size_t m_auto_observers_next_index;
    mutable std::unordered_map< size_t, observer_type > m_auto_observers;
};

}

}

