#pragma once

#include "common.h"
#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

class Connection : public std::enable_shared_from_this<Connection>
{
private:
	asio::io_context& context;
	asio::ip::tcp::socket socket;
	std::deque<Message> messages_out;
	std::deque<OwnedMessage>& messages_in;
	Message message_temp;	// temp container for outcoming messages

	uint32_t id = 0;


public:
	Connection(asio::io_context& s_context, asio::ip::tcp::socket s_socket, std::deque<OwnedMessage>& s_messages_in)
		: context(s_context), socket(std::move(s_socket)), messages_in(s_messages_in)
	{}

	uint32_t get_id()
	{
		return id;
	}

	void connect_to_client(uint32_t s_id = 0)
	{
		if (socket.is_open())
		{
			id = s_id;
			read_header();
		}
	}

	void connect_to_server(asio::ip::tcp::resolver::results_type endpoints)
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

private:
	// async
	void read_header()
	{
		asio::async_read(socket, asio::buffer(&message_temp.header, sizeof(MessageHeader)),
			[this](std::error_code ec, size_t length)
		{
			if (!ec)
			{
				if (message_temp.header.size > 0)
				{
					message_temp.body.reserve(message_temp.header.size);
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
		asio::async_read(socket, asio::buffer(message_temp.body),
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

	void add_to_incoming_message_queue()
	{
		messages_in.push_back({ this->shared_from_this(), message_temp });

		// start a new async listening to a new incoming message
		read_header();
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
		asio::async_write(socket, asio::buffer(message_temp.body),
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
};

} }