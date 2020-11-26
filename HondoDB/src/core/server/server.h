#pragma once

#include "../vendor/olc_net/olc_net.h"
#include "../vendor/cJSON/cJSON.h"


namespace hondo {

	enum MessageType
	{
		ServerAccept,
		ServerDeny,
		ServerMessage,
		ServerPing,
		MessageAll,
		Authenticate,
		DBQuery,
		DBQueryResult
	};
	
	class Server : public olc::net::server_interface<MessageType>
	{
	public:
		Server(uint16_t port) : olc::net::server_interface<MessageType>(port)
		{}

	protected:
		virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<MessageType>> client)
		{
			olc::net::message<MessageType> msg;
			msg.header.id = MessageType::ServerAccept;
			client->Send(msg);
			return true;
		}

		// Called when a client appears to have disconnected
		virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<MessageType>> client)
		{
			std::cout << "Removing client [" << client->GetID() << "]\n";
		}

		// Called when a message arrives
		virtual void OnMessage(std::shared_ptr<olc::net::connection<MessageType>> client, olc::net::message<MessageType>& msg)
		{
			switch (msg.header.id)
			{
			case MessageType::Authenticate:
			{
				std::cout << "[" << client->GetID() << "]: Authenticate\n";

				// TODO : look up if user with such login and password exists
				
				olc::net::message<MessageType> response_msg;
				msg.header.id = MessageType::ServerAccept;
				std::string request;
				//msg >> request;
				//char* request_str = cJSON_Print(request);
				std::cout << msg.body.data() << std::endl << std::endl;
				//cJSON_Delete(request);
				// TODO : send json response, e.g.
				//msg << "{ result : 'accept', note : 'all good' }";

				client->Send(response_msg);
			}
			break;
			
			case MessageType::ServerPing:
			{
				std::cout << "[" << client->GetID() << "]: Server Ping\n";

				// Simply bounce message back to client
				client->Send(msg);
			}
			break;

			case MessageType::MessageAll:
			{
				std::cout << "[" << client->GetID() << "]: Message All\n";

				// Construct a new message and send it to all clients
				olc::net::message<MessageType> msg;
				msg.header.id = MessageType::ServerMessage;
				msg << client->GetID();
				MessageAllClients(msg, client);

			}
			break;
			}
		}
	};

}