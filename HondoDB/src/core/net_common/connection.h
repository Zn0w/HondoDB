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
	std::string message;


public:
	Connection(asio::io_context& s_io_context, asio::ip::tcp::socket s_socket)
		: io_context(s_io_context), socket(std::move(s_socket))
	{}

	void greet()
	{
		//message_ = make_daytime_string();

		message = "Hello from Server!";
		asio::async_write(socket, asio::buffer(message),
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
};

} }