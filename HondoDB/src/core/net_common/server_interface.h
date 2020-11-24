#pragma once

#include "common.h"
#include "message.h"
#include "queue.h"
#include "connection.h"


namespace hondo { namespace net {

class ServerInterface
{
protected:
	asio::io_context io_context;
	asio::ip::tcp::acceptor acceptor;
	std::vector<std::shared_ptr<Connection>> connections;
	std::vector<std::string> in;


public:
	ServerInterface(uint16_t port)
		: acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
	{}

	virtual ~ServerInterface()
	{
		stop();
	}

	void start()
	{
		try
		{
			wait_for_client_connection();

			io_context.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	void stop()
	{
		io_context.stop();
	}

	// async
	void wait_for_client_connection()
	{
		acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
		{
			if (!ec)
			{
				std::shared_ptr<Connection> new_connection = std::make_shared<Connection>(io_context, std::move(socket), in);
				new_connection->greet();

				connections.push_back(new_connection);

				std::cout << "client connected!" << std::endl;
			}

			wait_for_client_connection();
		});
	}

protected:
	virtual bool on_client_connect(std::shared_ptr<Connection> client)
	{
		return false;
	}

	virtual void on_client_disconnect(std::shared_ptr<Connection> client)
	{}

	virtual void on_message(std::shared_ptr<Connection> client, const Message& message)
	{}
};

} }