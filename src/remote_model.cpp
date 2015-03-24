#include <iostream>
#include <zmq.hpp>

int main( int argc, const char *argv[] )
{
  if ( argc < 2 )
  {
    std::cerr <<
      "Usage: " << std::endl <<
      "  " << argv[ 0 ] << " <endpoint>" << std::endl;
    return 1;
  }

  try
  {
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, ZMQ_REQ };

    const char* endpoint( argv[ 1 ] );
    std::cout << "Using endpoint: " << endpoint << std::endl;
    socket.connect( endpoint );

    zmq::message_t message;
    socket.send( message );
    socket.recv( &message );
    std::cout << std::string( static_cast< const char * >( message.data() ) );
  }
  catch ( std::exception& e )
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

