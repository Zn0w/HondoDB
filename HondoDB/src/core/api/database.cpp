#include "database.h"


namespace hondo { 

/*
	TODO : once net is working, check if able to establish connection, if not, set a temporary db environment in computer memory
*/

HondoDB::HondoDB(Connection c, std::function<void(DatabaseObjectStatus)> handler)
	: connection(c)
{
	if (client.Connect(connection.address, std::stoi(connection.port)))
	{
		client_server_thread = std::thread([this]() { process_client_server_interactions(); });
	}
	else
		status = DatabaseObjectStatus::ConnectFail;

	// wait until rejected or authorized
	while (
		status != DatabaseObjectStatus::Denied &&
		status != DatabaseObjectStatus::Authorized &&
		status != DatabaseObjectStatus::ServerAuthFail &&
		status != DatabaseObjectStatus::ServerDown &&
		status != DatabaseObjectStatus::ConnectFail
	);

	handler(status);
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

	rapidjson::Value collection(rapidjson::kStringType);
	collection.SetString(collection_name.c_str(), allocator);
	create_request_json.AddMember("collection", collection, allocator);

	// !!! is this gonna work? UPD: yes it does!
	create_request_json.AddMember("object", json_object, allocator);

	// TODO : make it possible to send json object in binary form without having to translate them into strings

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	create_request_json.Accept(writer);

	client.send_request(std::string(strbuf.GetString()));
	//handlers_to_call.insert(std::pair<HandlerFunctionId, std::function<void(rapidjson::Document)>>(counter++, result_handler_function));
	handlers_to_call.push_back(result_handler_function);
}

void HondoDB::retrieve(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
{
	// TODO : check to see if object has property "cond" &  in other operation functions for their special properties (e.g. in update, 
	//			the object has "cond" property and "update_list" property, which is array of record's property names and new values)
	
	if (!json_object.IsObject())
		std::cout << "HondoDB: couldn't perform create, since the provided json_object data is not a valid json object";

	rapidjson::Document retrieve_request_json;
	retrieve_request_json.SetObject();

	rapidjson::Document::AllocatorType& allocator = retrieve_request_json.GetAllocator();

	retrieve_request_json.AddMember("operation", "retrieve", allocator);

	rapidjson::Value collection(rapidjson::kStringType);
	collection.SetString(collection_name.c_str(), allocator);
	retrieve_request_json.AddMember("collection", collection, allocator);
	// object is a condition (filter)
	retrieve_request_json.AddMember("object", json_object, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	retrieve_request_json.Accept(writer);

	client.send_request(std::string(strbuf.GetString()));
	handlers_to_call.push_back(result_handler_function);
}

void HondoDB::update(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
{
	if (!json_object.IsObject())
		std::cout << "HondoDB: couldn't perform create, since the provided json_object data is not a valid json object";

	rapidjson::Document update_request_json;
	update_request_json.SetObject();

	rapidjson::Document::AllocatorType& allocator = update_request_json.GetAllocator();

	update_request_json.AddMember("operation", "update", allocator);

	rapidjson::Value collection(rapidjson::kStringType);
	collection.SetString(collection_name.c_str(), allocator);
	update_request_json.AddMember("collection", collection, allocator);
	// object is a condition (filter) & list of pairs (property, new_value)
	update_request_json.AddMember("object", json_object, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	update_request_json.Accept(writer);

	client.send_request(std::string(strbuf.GetString()));
	handlers_to_call.push_back(result_handler_function);
}

void HondoDB::destroy(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
{
	if (!json_object.IsObject())
		std::cout << "HondoDB: couldn't perform create, since the provided json_object data is not a valid json object";

	rapidjson::Document delete_request_json;
	delete_request_json.SetObject();

	rapidjson::Document::AllocatorType& allocator = delete_request_json.GetAllocator();

	delete_request_json.AddMember("operation", "delete", allocator);

	rapidjson::Value collection(rapidjson::kStringType);
	collection.SetString(collection_name.c_str(), allocator);
	delete_request_json.AddMember("collection", collection, allocator);
	// object is a condition (filter)
	delete_request_json.AddMember("object", json_object, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	delete_request_json.Accept(writer);

	client.send_request(std::string(strbuf.GetString()));
	handlers_to_call.push_back(result_handler_function);
}

void HondoDB::retrieve_all(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
{}

void HondoDB::nuke(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function)
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