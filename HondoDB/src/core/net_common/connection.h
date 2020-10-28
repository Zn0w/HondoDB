#pragma once

#include <memory>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "message.h"
#include "queue.h"


namespace hondo { namespace net {

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection() {}
	~Connection() {}

	bool connect_to_server();
	bool disconnect();
	bool is_connected();

	bool send_message(const Message& message);

protected:
	// each connection has a unique socket to remote
	asio::ip::tcp::socket socket;

	// whis context is shared with the whole asio interface
	asio::io_context asio_context;

	// this queue holds all messages to be sent to the remote side of this connection
	Queue<Message> messages_out;

	// this queue holds all messages that have been received from the remote side of this connection
	// (this is a reference, since the owner of this connection has to provide a queue)
	Queue<OwnedMessage>& messages_in;
};

} }