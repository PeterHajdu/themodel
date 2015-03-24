#pragma once

#include <zmq.hpp>
#include <functional>
#include <string>

namespace the
{

namespace model
{

class TreeNode;

using Exporter = std::function< std::string( const TreeNode& ) >;
class ZmqRemote
{
  public:
    explicit ZmqRemote(
        const char* endpoint,
        const TreeNode&,
        Exporter );

    void handle_requests();

  private:
    zmq::context_t m_context;
    zmq::socket_t m_socket;
    Exporter m_exporter;
    const TreeNode& m_root_node;
};

}

}

