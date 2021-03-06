#pragma once

#include <set>

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
		DBQueryResult,
		NoPermission
	};
	
	class Server : public olc::net::server_interface<MessageType>
	{
	private:
		struct DBUser
		{
			uint32_t id;
			std::string db_name;
		};
		
		std::vector<DBUser> auth_users;

		std::vector<DBUser>::iterator find_auth_user(uint32_t id)
		{
			std::vector<DBUser>::iterator i;
			
			for (i = auth_users.begin(); i != auth_users.end(); ++i)
				if (i->id = id)
					return i;

			return i;
		}

	
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
				//std::string cars = StorageEngine::get_file_contents("data/collections/cars.hondodb");

				std::cout << "[" << client->GetID() << "]: users: " << users << std::endl;
				//std::cout << "[" << client->GetID() << "]: cars: " << cars << std::endl;

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
				{
					response_msg.header.id = MessageType::ServerAuthSuccess;
					
					auth_users.push_back({ client->GetID(), auth_request_json["db_name"].GetString() });
				}
				else
				{
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

				rapidjson::Document response_json;
				response_json.SetObject();

				rapidjson::Document::AllocatorType& allocator = response_json.GetAllocator();
				
				auto user = find_auth_user(client->GetID());
				if (user != auth_users.end())
				{
					std::cout << "db request json: " << msg.body << std::endl;
					rapidjson::Document request_json;
					request_json.Parse(msg.body.c_str());

					if (request_json.IsObject())
					{
						if (request_json.HasMember("operation") && request_json["operation"].IsString() &&
							request_json.HasMember("collection") && request_json["collection"].IsString() &&
							request_json.HasMember("object") && request_json["object"].IsObject())
						{
							if (request_json["operation"] == "create")
							{
								// process create operation
								response_json.AddMember("success", "true", allocator);
								response_json.AddMember("message", "processing create operation", allocator);
							}
							else if (request_json["operation"] == "retrieve")
							{
								// process retrieve operation
								std::string resource_path = "data/db/" + user->db_name + "/collections/" + request_json["collection"].GetString() + ".hondodb";
								std::string collection_data_str = StorageEngine::get_file_contents(resource_path);
								
								rapidjson::Document data_json;
								data_json.Parse(collection_data_str.c_str());
								if (data_json.IsObject())
								{
									response_json.AddMember("success", "true", allocator);
									response_json.AddMember("message", "result of retrieve operation is in data element", allocator);
									response_json.AddMember("result", data_json, allocator);
								}
								else
								{
									response_json.AddMember("success", "false", allocator);
									response_json.AddMember("message", "the requested collection data file is corrupted", allocator);
								}
							}
						}
						else
						{
							response_json.AddMember("success", "false", allocator);
							response_json.AddMember("message", "invalid request object (must have operation:string, collection:string, object:object)", allocator);
						}
					}
					else
					{
						response_json.AddMember("success", "false", allocator);
						response_json.AddMember("message", "invalid request (is not an object)", allocator);
					}
				}
				else
				{
					response_json.AddMember("success", "false", allocator);
					response_json.AddMember("message", "No permission", allocator);
				}

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