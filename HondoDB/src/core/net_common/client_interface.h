#pragma once

#include <thread>
#include <stdint.h>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

struct ClientInterface
{
protected:
	// asio context handles the data transfer
	asio::io_context context;
	std::thread context_thread;
	asio::ip::tcp::socket socket;
	std::unique_ptr<Connection> connection;

private:
	// a thread safe queue for incoming messages from server
	Queue<OwnedMessage> messages_in;


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
			connection = std::make_unique<Connection>();
			asio::ip::tcp::resolver resolver(context);
			asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
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

	Queue<OwnedMessage>& incoming()
	{
		return messages_in;
	}
};

} }