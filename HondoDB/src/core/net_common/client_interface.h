#pragma once

#include "common.h"
#include "message.h"
#include "queue.h"
#include "connection.h"


namespace hondo { namespace net {

class ClientInterface
{
protected:
	asio::io_context context;
	std::unique_ptr<Connection> connection;
	std::vector<std::string> in;


public:
	ClientInterface()
	{}

	virtual ~ClientInterface()
	{
		disconnect();
	}

	bool connect(const std::string& host, const uint16_t port)
	{
		try
		{
			asio::ip::tcp::resolver resolver(context);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

			asio::ip::tcp::socket socket(context);

			
			asio::async_connect(socket, endpoints,
				[&, this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
				{
					if (!ec)
					{
						connection = std::make_unique<Connection>(context, std::move(socket), in);
						connection->read();
					}
				}
			);

			context.run();

			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "Client exception: " << e.what() << std::endl;
			return false;
		}

		return true;
	}

	void disconnect()
	{
		/*if (is_connected())
		{
			connection->disconnect();
		}*/

		context.stop();
		connection.release();
	}

	void update()
	{
		if (!in.empty())
		{
			for (std::string message : in)
				std::cout << "Server: " << message << std::endl;
			in.clear();
		}
	}
};

} }