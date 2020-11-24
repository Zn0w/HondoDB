#pragma once

#include "common.h"
#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

class Connection : public std::enable_shared_from_this<Connection>
{
private:
	asio::io_context& io_context;
	asio::ip::tcp::socket socket;
	std::vector<std::string>& in;
	std::string message_in;
	std::string message_out;


public:
	Connection(asio::io_context& s_io_context, asio::ip::tcp::socket s_socket, std::vector<std::string>& s_in)
		: io_context(s_io_context), socket(std::move(s_socket)), in(s_in)
	{}

	void greet()
	{
		message_out = "Hello from Server!";
		asio::async_write(socket, asio::buffer(message_out),
			[this](std::error_code ec, size_t length)
		{
			if (!ec)
			{
				std::cout << "write success" << std::endl;
			}
			else
			{
				std::cout << "write fail" << std::endl;
			}
		});
	}

	// async
	void read()
	{
		asio::async_read(socket, asio::buffer(message_in), asio::transfer_at_least(1),
			[&](std::error_code ec, size_t length)
		{
			if (!ec)
			{
				in.push_back(message_in);
				read();
			}
			else
			{
				std::cout << "read fail" << std::endl;
				socket.close();
			}
		}
		);
	}
};

} }