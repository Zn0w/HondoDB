#pragma once

#include <memory>
#include <stdint.h>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	enum class owner
	{
		server,
		client
	};


protected:
	// each connection has a unique socket to remote
	asio::ip::tcp::socket socket;

	// whis context is shared with the whole asio interface
	asio::io_context& context;

	// this queue holds all messages to be sent to the remote side of this connection
	Queue<Message> messages_out;
	Message message_temporary_in;

	// this queue holds all messages that have been received from the remote side of this connection
	// (this is a reference, since the owner of this connection has to provide a queue)
	Queue<OwnedMessage>& messages_in;

	owner owner_type = owner::server;
	uint32_t id = 0;


public:
	Connection(owner parent, asio::io_context& s_context, asio::ip::tcp::socket s_socket, Queue<OwnedMessage>& s_messages_in)
		: context(s_context), socket(std::move(s_socket)), messages_in(s_messages_in)
	{
		owner_type = parent;
	}
	
	virtual ~Connection()
	{}

	void connect_to_client(uint32_t s_id = 0)
	{
		if (owner_type == owner::server)
		{
			if (socket.is_open())
			{
				id = s_id;
			}
		}
	}
	
	bool connect_to_server();
	bool disconnect();

	bool is_connected()
	{
		return socket.is_open();
	}

	bool send(const Message& message);

	uint32_t get_id()
	{
		return id;
	}

private:
	// async
	void read_header()
	{
		asio::async_read(socket, asio::buffer(&message_temporary_in.header, sizeof(MessageHeader)),
			[this](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					if (message_temporary_in.header.size > 0)
					{
						message_temporary_in.body.resize(message_temporary_in.header.size);
						read_body();
					}
					else
					{
						add_to_incoming_message_queue();
					}
				}
				else
				{
					std::cout << id << ": Read header fail" << std::endl;
					socket.close();
				}
			}
		);
	}

	// async
	void read_body()
	{

	}

	// async
	void write_header()
	{

	}

	// async
	void write_body()
	{

	}
};

} }