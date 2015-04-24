#pragma once

#include <themodel/variable.hpp>
#include <themodel/node.hpp>
#include <string>
#include <cassert>
#include <unordered_map>

namespace the
{

namespace model
{

template < typename Key, typename Value >
class Hash : public OwningNode
{
  public:
    template< typename Parent >
    Hash( std::string name_, Parent& parent )
      : OwningNode( std::move( name_ ), parent )
    {
    }

    using ValueType = Variable< Value >;

    ValueType& operator[]( const Key& key )
    {
      auto element_iterator( m_container.find( key ) );
      if ( element_iterator == std::end( m_container ) )
      {
        bool was_inserted{ false };
        std::tie( element_iterator, was_inserted ) = m_container.emplace( std::make_pair(
              key,
              std::make_unique< ValueType >(
                key,
                Value(),
                *this ) ) );
        assert( was_inserted );
      }

      return *element_iterator->second;
    }

    Value get( const Key& key ) const
    {
      auto element_iterator( m_container.find( key ) );
      if ( element_iterator == std::end( m_container ) )
      {
        throw std::runtime_error( "Key does not exist!" );
      }

      return element_iterator->second->get();
    }

    bool has( const Key& key ) const
    {
      return m_container.find( key ) != std::end( m_container );
    }

    auto begin()
    {
      return std::begin( m_container );
    }

    auto end()
    {
      return std::end( m_container );
    }

    auto cbegin() const
    {
      return m_container.cbegin();
    }

    auto cend() const
    {
      return m_container.cend();
    }

  private:
    using ValuePointer = std::unique_ptr< ValueType >;
    using Container = std::unordered_map< Key, ValuePointer >;

    Container m_container;
};

}

}

