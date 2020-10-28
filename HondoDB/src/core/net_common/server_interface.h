#pragma once

#include <thread>
#include <stdint.h>
#include <deque>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

class ServerInterface
{
protected:
	asio::io_context context;
	std::thread context_thread;
	asio::ip::tcp::acceptor acceptor;
	
	Queue<OwnedMessage> messages_in;

	std::deque<std::shared_ptr<Connection>> connections;

	// clients will be identified by ID
	uint32_t id_counter = 10000;


public:
	ServerInterface(uint16_t port)
		: acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
	{}

	virtual ~ServerInterface()
	{
		stop();
	}

	bool start()
	{
		try
		{
			wait_for_client_connection();

			context_thread = std::thread([this]() { context.run(); });
		}
		catch (std::exception& e)
		{
			std::cerr << "Server: Exception: " << e.what() << std::endl;
			return false;
		}

		std::cout << "Server: has started!" << std::endl;
	}

	bool stop()
	{
		context.stop();
		if (context_thread.joinable())
			context_thread.join();

		std::cout << "Server: has stopped!" << std::endl;
	}

	// async
	void wait_for_client_connection()
	{
		acceptor.async_accept(
			[this](std::error_code ec, asio::ip::tcp::socket socket)
			{
				if (!ec)
				{
					std::cout << "Server: New connection: " << socket.remote_endpoint() << std::endl;

					std::shared_ptr<Connection> new_connection = std::make_shared<Connection>(
						Connection::owner::server,
						context, std::move(socket), messages_in
					);

					if (on_client_connect(new_connection))
					{
						connections.push_back(std::move(new_connection));
						connections.back()->connect_to_client(id_counter++);

						std::cout << connections.back()->get_id() << ": Connection approved" << std::endl;
					}
					else
					{
						std::cout << "Server: Connection denied" << std::endl;
					}
				}
				else
				{
					std::cout << "Server: New connection error: " << ec.message() << std::endl;
				}

				// again simply wait for another connection
				wait_for_client_connection();
			}
		);
	}

	void message_client(std::shared_ptr<Connection> client, const Message& message)
	{
		if (client && client->is_connected())
		{
			client->send(message);
		}
		else
		{
			on_client_disconnect(client);
			client.reset();
			connections.erase(std::remove(connections.begin(), connections.end(), client), connections.end());
		}
	}

	void message_all_clients(const Message& message, std::shared_ptr<Connection> ignore_client = nullptr)
	{
		bool invalid_user_exists = false;
		
		for (auto& client : connections)
		{
			if (client && client->is_connected())
			{
				if (client != ignore_client)
					client->send(message);
			}
			else
			{
				on_client_disconnect(client);
				client.reset();
				invalid_user_exists = true;
			}
		}

		// erase function is costly, so instead of removing null clients one by one, remove them all at once at the end
		if (invalid_user_exists)
			connections.erase(std::remove(connections.begin(), connections.end(), nullptr), connections.end());
	}

	void update(size_t max_messages = -1)
	{
		size_t message_count = 0;
		for (size_t message_count = 0; message_count < max_messages && !messages_in.empty(); message_count++)
		{
			auto message = messages_in.pop_front();
			on_message(message.remote, message.message);
		}
	}

protected:
	virtual bool on_client_connect(std::shared_ptr<Connection> client)
	{
		return false;
	}

	virtual void on_client_disconnect(std::shared_ptr<Connection> client)
	{
		
	}

	virtual void on_message(std::shared_ptr<Connection> client, const Message& message)
	{
		
	}
};

} }