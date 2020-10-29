#pragma once

#include "../net_common/server_interface.h"


namespace hondo {

class Server : public net::ServerInterface
{
public:
	Server(uint16_t port)
		: ServerInterface(port)
	{}

protected:
	virtual bool on_client_connect(std::shared_ptr<net::Connection> client)
	{
		net::Message message;
		message.header.id = net::MessageType::ServerAccept;
		client->send(message);
		// let server interface accept the client
		return true;
	}

	virtual void on_client_disconnect(std::shared_ptr<net::Connection> client)
	{
		std::cout << "Server: Removing client " << client->get_id() << std::endl;
	}

	virtual void on_message(std::shared_ptr<net::Connection> client, const net::Message& message)
	{
		switch (message.header.id)
		{
			case net::MessageType::Rename:
			{
				// rename the client (to do)
			} break;
		}
	}
};

}