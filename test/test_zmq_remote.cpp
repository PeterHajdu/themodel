#include "test_tree_node.hpp"
#include <cstring>
#include <thread>
#include <themodel/zmq_remote.hpp>
#include <igloo/igloo_alt.h>

using namespace igloo;

namespace test
{

class ZmqClient
{
  public:
    ZmqClient( const char* endpoint )
    {
      socket.connect( endpoint );
    }

    void send( const std::string& message )
    {
      zmq::message_t request( message.length() + 1 );
      socket.send( request );
      using namespace std::literals;
      std::this_thread::sleep_for( 100ms );
    }

    const std::string response()
    {
      zmq::message_t resp;
      socket.recv( &resp );
      return std::string( static_cast< const char * >( resp.data() ) );
    }

    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, ZMQ_REQ };
};

}

Describe(a_zmq_remote)
{
  void SetUp()
  {
    was_exporter_called = false;
    exporter_was_called_with = nullptr;
    number_of_exporter_calls = 0u;

    try
    {
    zmq_remote = std::make_unique< the::model::ZmqRemote >(
        endpoint,
        root,
        [ &was_called = was_exporter_called,
          &was_called_with = exporter_was_called_with,
          exported_tree = exported_tree,
          &number_of_calls = number_of_exporter_calls
        ]( const the::model::TreeNode& node ) -> std::string
        {
          was_called = true;
          was_called_with = &node;
          ++number_of_calls;
          return exported_tree;
        } );
    } catch( std::exception& e )
    {
      std::cout << e.what() << std::endl;
      throw e;
    }

    zmq_client = std::make_unique< test::ZmqClient >( endpoint );
  }

  It( does_not_call_the_exporter_function_if_there_are_no_requests )
  {
    zmq_remote->handle_requests();
    AssertThat( was_exporter_called, Equals( false ) );
  }

  It( calls_the_exporter_function_with_the_root_node_when_a_request_arrives )
  {
    zmq_client->send( "dogfood" );
    zmq_remote->handle_requests();
    AssertThat( was_exporter_called, Equals( true ) );
    AssertThat( exporter_was_called_with, Equals( &root ) );
  }

  It( sends_the_exported_tree_as_answer )
  {
    zmq_client->send( "dogfood" );
    zmq_remote->handle_requests();
    AssertThat( zmq_client->response(), Equals( exported_tree ) );
  }

  It( responds_all_requests_during_handle_requests )
  {
    test::ZmqClient another_client( endpoint );
    another_client.send( "dogfood" );
    zmq_client->send( "dogfood" );

    zmq_remote->handle_requests();
    AssertThat( zmq_client->response(), Equals( exported_tree ) );
    AssertThat( another_client.response(), Equals( exported_tree ) );
  }

  It( calls_the_exporter_only_once_even_for_many_requests )
  {
    test::ZmqClient another_client( endpoint );
    another_client.send( "dogfood" );
    zmq_client->send( "dogfood" );
    zmq_remote->handle_requests();
    AssertThat( number_of_exporter_calls, Equals( 1u ) );
  }

  std::unique_ptr< the::model::ZmqRemote > zmq_remote;

  const char *endpoint{ "tcp://127.0.0.1:5555" };
  const std::string root_name{ "root" };
  const test::TreeNode root{ root_name };
  const std::string exported_tree{ "exported tree" };

  std::unique_ptr< test::ZmqClient > zmq_client;

  bool was_exporter_called;
  const the::model::TreeNode* exporter_was_called_with;
  size_t number_of_exporter_calls;
};

