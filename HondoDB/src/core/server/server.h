#pragma once

#include "../vendor/olc_net/olc_net.h"
#include "../vendor/cJSON/cJSON.h"


namespace hondo {

	enum MessageType
	{
		ServerAccept,
		ServerDeny,
		ServerAuthSuccess,
		ServerAuthFailure,
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
				std::cout << "[" << client->GetID() << "]: Body: " << msg.body << std::endl;

				olc::net::message<MessageType> response_msg;
				
				std::string auth_request = msg.body;
				cJSON* auth_request_json = cJSON_Parse(auth_request.c_str());
				// TODO : look up if user with such login and password exists
				if (true)
					response_msg.header.id = MessageType::ServerAuthSuccess;
				else
				{
					response_msg.header.id = MessageType::ServerAuthFailure;
					
					/*cJSON* response_json = cJSON_CreateObject();
					// TODO : check if response_json is null
					cJSON* auth_result = cJSON_CreateString("");*/
					//std::string response = "{'auth_fail_reason':''}";
					//response_msg << response;
				}
				cJSON_Delete(auth_request_json);

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
				/*olc::net::message<MessageType> msg;
				msg.header.id = MessageType::ServerMessage;
				msg << client->GetID();
				MessageAllClients(msg, client);*/

			}
			break;
			}
		}
	};

}