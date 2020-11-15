#include <iostream>

#include <core/server/server.h>


using namespace hondo::net;


std::string make_daytime_string()
{
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	char buffer[256];
	ctime_s(buffer, 256, &now);
	return std::string(buffer);
}

class tcp_connection
	: public std::enable_shared_from_this<tcp_connection>
{
public:
	tcp_connection(asio::io_context& io_context, asio::ip::tcp::socket socket)
		: io_context_(io_context), socket_(std::move(socket))
	{
	}
	
	void start()
	{
		//message_ = make_daytime_string();

		message_ = "Hello from Server!";
		asio::async_write(socket_, asio::buffer(message_),
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

private:
	asio::io_context& io_context_;
	asio::ip::tcp::socket socket_;
	std::string message_;
};

class tcp_server
{
public:
	tcp_server(asio::io_context& io_context)
		: io_context_(io_context),
		acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		acceptor_.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::shared_ptr<tcp_connection> new_connection = std::make_shared<tcp_connection>(io_context_, std::move(socket));
				new_connection->start();

				connections.push_back(new_connection);

				std::cout << "client connected!" << std::endl;
			}

			start_accept();
		});
	}

	asio::io_context& io_context_;
	asio::ip::tcp::acceptor acceptor_;
	std::vector<std::shared_ptr<tcp_connection>> connections;
};

int main()
{
	try
	{
		asio::io_context io_context;
		tcp_server server(io_context);
		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}