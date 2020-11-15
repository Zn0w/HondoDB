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

		char buffer[256];
		bool done = false;

		asio::async_read(socket, asio::buffer(buffer, 256), asio::transfer_at_least(1),
			[&](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					std::cout << "Server: " << buffer << std::endl;
				}
				else
				{
					std::cout << "read fail" << std::endl;
					socket.close();
				}

				done = true;
			}
		);

		io_context.run();
		
		while (!done);

		io_context.stop();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");

	return 0;
}