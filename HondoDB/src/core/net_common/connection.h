#pragma once

#include "common.h"
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
				read_header();
			}
		}
	}
	
	void connect_to_server(asio::ip::tcp::resolver::results_type endpoints)
	{
		if (owner_type == owner::client)
		{
			asio::async_connect(socket, endpoints,
				[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
				{
					if (!ec)
					{
						read_header();
					}
				}
			);
		}
	}
	
	void disconnect()
	{
		if (is_connected())
			asio::post(context, [this]() { socket.close(); });
	}

	bool is_connected()
	{
		return socket.is_open();
	}

	// async
	void send(const Message& message)
	{
		asio::post(context,
			[this, message]()
			{
				bool writing_message = !messages_out.empty();
				messages_out.push_back(message);
				if (!writing_message)
				{
					write_header();
				}
			}
		);
	}

	uint32_t get_id()
	{
		return id;
	}

private:
	// async
	void read_header()
	{
		asio::async_read(socket, asio::buffer(&message_temporary_in.header.size, sizeof(MessageHeader)),
			[this](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					if (message_temporary_in.header.size > 0)
					{
						message_temporary_in.body.reserve(message_temporary_in.header.size);
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
		asio::async_read(socket, asio::buffer(//message_temporary_in.body.data(), message_temporary_in.body.size()),
			//message_temporary_in.body.raw(), message_temporary_in.body.get_length()),
			message_temporary_in.body.data(), message_temporary_in.body.size()),
			[this](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					add_to_incoming_message_queue();
				}
				else
				{
					std::cout << id << ": Read body fail" << std::endl;
					socket.close();
				}
			}
		);
	}

	// async
	void write_header()
	{
		asio::async_write(socket, asio::buffer(&messages_out.front().header, sizeof(MessageHeader)),
			[this](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					if (messages_out.front().header.size > 0)
					{
						write_body();
					}
					else
					{
						messages_out.pop_front();

						if (messages_out.empty())
						{
							// start a new async listening to a new outcoming message
							write_header();
						}
					}
				}
				else
				{
					std::cout << id << ": Write header fail" << std::endl;
					socket.close();
				}
			}
		);
	}

	// async
	void write_body()
	{
		asio::async_write(socket, asio::buffer(//messages_out.front().body.data(), messages_out.front().body.size()),
			//message_temporary_in.body.raw(), message_temporary_in.body.get_length()),
			message_temporary_in.body.data(), message_temporary_in.body.size()),
			[this](std::error_code ec, size_t length)
			{
				if (!ec)
				{
					messages_out.pop_front();

					if (messages_out.empty())
					{
						// start a new async listening to a new outcoming message
						write_header();
					}
				}
				else
				{
					std::cout << id << ": Write body fail" << std::endl;
					socket.close();
				}
			}
		);
	}

	void add_to_incoming_message_queue()
	{
		if (owner_type == owner::server)
			messages_in.push_back({ this->shared_from_this(), message_temporary_in });
		// if client, no reason to save connection as well, since every client has only one connection (connection to the server)
		else
			messages_in.push_back({ nullptr, message_temporary_in });

		// start a new async listening to a new incoming message
		read_header();
	}
};

} }