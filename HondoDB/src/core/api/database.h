#pragma once

#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <map>

#include "../json/item.h"

#include "../client/client.h"
#include "connection.h"


namespace hondo {

enum DatabaseObjectStatus
{
	Fail,
	ServerAuthFail,
	ServerDown,
	ConnectFail,
	Connected,
	Stopped,
	Denied,
	InRAM,
	Authorized,
};

typedef uint32_t HandlerFunctionId;
	
class HondoDB
{
private:
	Client client;
	Connection connection;

	std::thread client_server_thread;
	bool quit = false;

	DatabaseObjectStatus status;

	// TODO : maybe std::vector is good enough?? are messages always going to be processed in queue?
	//HandlerFunctionId counter = 1;
	//std::map<HandlerFunctionId, std::function<void(rapidjson::Document)>> handlers_to_call;
	std::deque<std::function<void(rapidjson::Document&)>> handlers_to_call;


public:
	HondoDB();	// set up a temporary db in computer memory
	HondoDB(Connection c);
	~HondoDB();
	
	// TODO : maybe make a universal function, so the user of api has to specify required operation inside the json_object
	//			(make it all up to the protrocol/specification)
	void create(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);
	void retrieve(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);
	void update(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);
	void destroy(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);

	void retrieve_all(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);

	void nuke(std::string collection_name, rapidjson::Document& json_object, std::function<void(rapidjson::Document&)> result_handler_function);

	DatabaseObjectStatus get_status();

private:
	void process_client_server_interactions();
};

}