/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>


int main()
{
	/*hondo::Client client("daniil", "12345");
	client.connect("127.0.0.1", 27575);

	bool accepted = false;

	if (client.is_connected())
	{
		if (accepted)
		{
			client.authenticate();
		}

		if (!client.incoming().empty())
		{
			auto message = client.incoming().pop_front().message;
			switch (message.header.id)
			{
			case hondo::net::MessageType::ServerAccept:
			{
				std::cout << "Server Accepted Connection" << std::endl;
				accepted = true;
			} break;
			}
		}
	}*/

	/*asio::error_code error_code;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", error_code), 80);

	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, error_code);

	if (!error_code)
	{
		std::cout << "Successfully connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect: " << error_code.message() << std::endl;
	}

	if (socket.is_open())
	{
		std::string request = "GET /index.html HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";
		socket.write_some(asio::buffer(request.data(), request.size()), error_code);

		socket.wait(socket.wait_read);

		size_t bytes = socket.available();
		if (bytes)
		{
			std::vector<char> buffer(bytes);
			socket.read_some(asio::buffer(buffer.data(), buffer.size()));

			for (char c : buffer)
				std::cout << c;
		}
	}*/

	struct Message
	{
		int a;
		float b;
		char c;
	};

	try
	{
		asio::error_code error_code;
		asio::io_context io_context;
		asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1", error_code), 27575);

		asio::ip::tcp::socket socket(io_context);

		socket.connect(endpoint, error_code);

		Message message_in;

		asio::async_read(socket, asio::buffer(&message_in, sizeof(Message)),
			[&](std::error_code ec, size_t length)
			{
				if (ec)
				{
					std::cout << "Read message fail" << std::endl;
					socket.close();
				}
				else
					std::cout << message_in.a << "  " << message_in.b << "  " << message_in.c << std::endl;
			}
		);

		if (error_code == asio::error::eof)
			; // Connection closed cleanly by peer.
		else if (error_code)
			throw asio::system_error(error_code); // Some other error.
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	system("pause");

	return 0;
}