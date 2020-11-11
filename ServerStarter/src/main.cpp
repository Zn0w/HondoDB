#include <iostream>

#include <core/server/server.h>




int main()
{
	/*hondo::Server server(27575);
	server.start();

	while (true)
	{
		server.update();
	}*/

	struct Message
	{
		int a;
		float b;
		char c;
	};

	try
	{
		asio::io_context io_context;

		asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 27575));

		for (;;)
		{
			asio::ip::tcp::socket socket(io_context);
			acceptor.accept(socket);

			Message message_out;
			message_out.a = 2;
			message_out.a = 5.0f;
			message_out.a = '7';

			asio::error_code ignored_error;
			asio::async_write(socket, asio::buffer(&message_out, sizeof(Message)),
				[&](std::error_code ec, size_t length)
			{
				if (ec)
				{
					std::cout << "Write message fail" << std::endl;
					socket.close();
				}
				else
					std::cout << "Write message success" << std::endl;
			}
			);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}