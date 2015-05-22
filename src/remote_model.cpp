#include <iostream>
#include <zmq.hpp>

int main( int argc, const char *argv[] )
{
  if ( argc < 2 )
  {
    std::cerr <<
      "Usage: " << std::endl <<
      "  " << argv[ 0 ] << " <endpoint> [path.to.call]" << std::endl;
    return 1;
  }

  try
  {
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, ZMQ_REQ };

    const int timeout_in_ms{ 3000 };
    socket.setsockopt( ZMQ_RCVTIMEO, &timeout_in_ms, sizeof( timeout_in_ms ) );

    const char* endpoint( argv[ 1 ] );
    socket.connect( endpoint );

    zmq::message_t message;
    if ( argc >= 3 )
    {
      const std::string request( std::string( "call " ) + argv[ 2 ] );
      message.rebuild( request.size() );
      memcpy( message.data(), request.c_str(), request.size() );
    }

    socket.send( message );
    if ( !socket.recv( &message ) )
    {
      std::cerr << "Did not receive response in " << timeout_in_ms << "ms." << std::endl;
      exit( 1 );
    }

    std::cout << std::string( static_cast< const char * >( message.data() ) );
  }
  catch ( std::exception& e )
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

