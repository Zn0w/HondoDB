#include "database.h"


namespace hondo { 

/*
	TODO : once net is working, check if able to establish connection, if not, set a temporary db environment in computer memory
*/

HondoDB::HondoDB(Connection c)
	: connection(c)
{
	if (client.Connect(connection.address, std::stoi(connection.port)))
	{
		client_server_thread = std::thread([this]() { process_client_server_interactions(); });
	}
	else
		status = DatabaseObjectStatus::ConnectFail;
}

HondoDB::~HondoDB()
{
	quit = true;

	if (client_server_thread.joinable())
		client_server_thread.join();
}


void HondoDB::create(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
{
	if (!json_object.IsObject())
		// TODO : make error messaging more clear (add details of what db object it is, etc.)
		std::cout << "HondoDB: couldn't perform create, since the provided json_object data is not a valid json object";
	
	rapidjson::Document create_request_json;
	create_request_json.SetObject();

	rapidjson::Document::AllocatorType& allocator = create_request_json.GetAllocator();

	create_request_json.AddMember("operation", "create", allocator);
	// !!! is this gonna work?
	create_request_json.AddMember("object", json_object, allocator);

	// TODO : make it possible to send json object in binary form without having to translate them into strings

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	create_request_json.Accept(writer);

	client.send_request(std::string(strbuf.GetString()));
	//handlers_to_call.insert(std::pair<HandlerFunctionId, std::function<void(rapidjson::Document)>>(counter++, result_handler_function));
	handlers_to_call.push_back(result_handler_function);
}

void HondoDB::retrieve(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document)> result_handle_function)
{}

void HondoDB::update(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document)> result_handle_function)
{}

void HondoDB::destroy(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document)> result_handle_function)
{}

void HondoDB::retrieve_all(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document)> result_handle_function)
{}

void HondoDB::nuke(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document)> result_handle_function)
{}


DatabaseObjectStatus HondoDB::get_status()
{
	return status;
}


void HondoDB::process_client_server_interactions()
{
	client.authenticate(connection.user, connection.password, connection.db_name);

	while (!quit)
	{
		if (client.IsConnected())
		{
			if (!client.Incoming().empty())
			{
				auto msg = client.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case hondo::MessageType::ServerAccept:
				{
					std::cout << "HondoDB Server Accepted Connection" << std::endl;
					status = DatabaseObjectStatus::Connected;
				}
				break;

				case hondo::MessageType::ServerDeny:
				{
					std::cout << "HondoDB Server Rejected Connection" << std::endl;
					quit = true;
					status = DatabaseObjectStatus::Denied;
				}
				break;

				case hondo::MessageType::ServerAuthSuccess:
				{
					std::cout << "HondoDB Auth Success" << std::endl;
					status = DatabaseObjectStatus::Authorized;
				}
				break;

				case hondo::MessageType::ServerAuthFailure:
				{
					std::cout << "HondoDB Auth Failure" << std::endl;
					std::cout << msg.body << std::endl;
					quit = true;
					status = DatabaseObjectStatus::ServerAuthFail;
				}
				break;

				case hondo::MessageType::DBQueryResult:
				{
					std::cout << "HondoDB Query Result" << std::endl;
					std::cout << msg.body << std::endl;
					
					auto handler_function = std::move(handlers_to_call.front());
					handlers_to_call.pop_front();

					rapidjson::Document response_json;
					response_json.Parse(msg.body.c_str());

					handler_function(response_json);
				}
				break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			quit = true;
			status = DatabaseObjectStatus::ServerDown;
		}
	}
}

}