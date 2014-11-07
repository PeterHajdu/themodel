#include <themodel/node.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

Describe(a_node)
{
  It( is_instantiable )
  {
    the::model::Node node;
    (void)node;
  }
};

