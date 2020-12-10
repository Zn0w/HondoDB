#pragma once

#include "../vendor/olc_net/olc_net.h"

#include "../vendor/rapidjson/document.h"
#include "../vendor/rapidjson/writer.h"
#include "../vendor/rapidjson/stringbuffer.h"

#include "../storage_engine/storage.h"


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

				std::string users = StorageEngine::get_file_contents("data/meta/users.hondodb");
				std::string cars = StorageEngine::get_file_contents("data/collections/cars.hondodb");

				std::cout << "[" << client->GetID() << "]: users: " << users << std::endl;
				std::cout << "[" << client->GetID() << "]: cars: " << cars << std::endl;

				olc::net::message<MessageType> response_msg;

				bool request_is_valid = false;
				bool user_found = false;
				bool password_is_right = false;
				bool has_grant = false;
				
				std::string auth_request = msg.body;
				rapidjson::Document auth_request_json;
				auth_request_json.Parse(auth_request.c_str());

				request_is_valid = 
					auth_request_json.HasMember("user") &&
					auth_request_json.HasMember("password") &&
					auth_request_json.HasMember("db_name");

				// TODO : do comparison not with strcmp, but with rapidjson mechanism
				
				if (request_is_valid)
				{
					rapidjson::Document users_json;
					users_json.Parse(users.c_str());

					auto users_array = users_json["users"].GetArray();

					for (rapidjson::Value::ConstValueIterator i = users_array.Begin(); i != users_array.End(); i++)
					{
						if (user_found)
							break;

						if (i->HasMember("user"))
						{
							if (strcmp((*i)["user"].GetString(), auth_request_json["user"].GetString()) == 0)
								user_found = true;

							if (user_found && i->HasMember("password"))
							{
								if (strcmp((*i)["password"].GetString(), auth_request_json["password"].GetString()) == 0)
									password_is_right = true;
							}

							if (password_is_right && i->HasMember("grants"))
							{
								auto user_grants = (*i)["grants"].GetArray();

								for (rapidjson::Value::ConstValueIterator j = user_grants.Begin(); j != user_grants.End(); j++)
									if (strcmp((*j).GetString(), auth_request_json["db_name"].GetString()) == 0)
									{
										has_grant = true;
										break;
									}
							}
						}
					}
				}
				
				if (request_is_valid && user_found && password_is_right && has_grant)
					response_msg.header.id = MessageType::ServerAuthSuccess;
				else
				{
					// TODO : add auth_fail_reason
					response_msg.header.id = MessageType::ServerAuthFailure;
					
					std::string auth_fail_reason;

					if (!request_is_valid)
						auth_fail_reason = "Auth Request JSON Object was not valid (must have 3 members: user, password, db_name)";
					else if (!user_found)
						auth_fail_reason = "User with provided login does not exist";
					else if (!password_is_right)
						auth_fail_reason = "Provided password doesn not match the user's actual password";
					else if (!has_grant)
						auth_fail_reason = "User does not have permission to access db with provided name";
					
					response_msg << auth_fail_reason;
				}

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

			case MessageType::DBQuery:
			{
				std::cout << "[" << client->GetID() << "]: DB Query\n";

				
				olc::net::message<MessageType> response_msg;
				response_msg.header.id = MessageType::DBQueryResult;

				std::cout << "db request json: " << msg.body << std::endl;
				rapidjson::Document request_json;
				request_json.Parse(msg.body.c_str());

				rapidjson::Document response_json;
				response_json.SetObject();

				rapidjson::Document::AllocatorType& allocator = response_json.GetAllocator();

				if (request_json.IsObject())
					response_json.AddMember("is_object", "true", allocator);
				else
					response_json.AddMember("is_object", "true", allocator);

				rapidjson::StringBuffer strbuf;
				rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
				response_json.Accept(writer);
				
				response_msg << std::string(strbuf.GetString());
				
				client->Send(response_msg);
			}
			break;
			}
		}
	};

}