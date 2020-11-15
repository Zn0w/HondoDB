/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>

using namespace hondo::net;

using asio::ip::tcp;


int main(int argc, char* argv[])
{
	try
	{
		asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints =
			resolver.resolve("127.0.0.1", "13");

		tcp::socket socket(io_context);
		asio::connect(socket, endpoints);

		for (;;)
		{
			char buf[256];
			asio::error_code error;

			size_t len = socket.read_some(asio::buffer(buf, 256), error);

			if (error == asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw asio::system_error(error); // Some other error.

			std::cout << "Server: ";
			std::cout.write(buf, len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");

	return 0;
}