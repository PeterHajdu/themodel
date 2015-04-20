#pragma once

#include <themodel/variable.hpp>
#include <themodel/node.hpp>
#include <string>
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

    template < typename K >
    ValueType& operator[]( K&& key )
    {
      return *m_container[ key ];
    }

  private:
    using ValuePointer = std::unique_ptr< ValueType >;
    using Container = std::unordered_map< Key, ValuePointer >;

    Container m_container;
};

}

}

