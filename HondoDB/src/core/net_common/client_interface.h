#pragma once

#include "common.h"
#include "message.h"
#include "queue.h"
#include "connection.h"


/*namespace hondo { namespace net {

class ClientInterface
{
private:
	asio::io_context context;
	std::thread context_thread;
	asio::ip::tcp::socket socket;
	std::unique_ptr<Connection> connection;
	std::deque<OwnedMessage> messages_in;


public:
	ClientInterface()
		: socket(context)
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

			connection = std::make_unique<Connection>(context, asio::ip::tcp::socket(context), messages_in);
			connection->connect_to_server(endpoints);

			context_thread = std::thread([this]() { context.run(); });
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
		if (is_connected())
		{
			connection->disconnect();
		}

		context.stop();
		if (context_thread.joinable())
			context_thread.join();

		connection.release();
	}

	bool is_connected()
	{
		if (connection)
			return connection->is_connected();
		else
			return false;
	}

	std::deque<OwnedMessage>& incoming()
	{
		return messages_in;
	}
};

} }*/