#pragma once

#include <string>
#include <vector>
#include <thread>

#include "../vendor/rapidjson/document.h"
#include "../vendor/rapidjson/writer.h"
#include "../vendor/rapidjson/stringbuffer.h"

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
	
class HondoDB
{
private:
	Client client;
	Connection connection;

	std::thread client_server_thread;
	bool quit = false;

	DatabaseObjectStatus status;


public:
	HondoDB();	// set up a temporary db in computer memory
	HondoDB(Connection c);
	~HondoDB();
	
	rapidjson::Document create();
	rapidjson::Document retrieve();
	rapidjson::Document update();
	rapidjson::Document destroy();

	rapidjson::Document retrieve_all();

	rapidjson::Document nuke();

	DatabaseObjectStatus get_status();

private:
	void process_client_server_interactions();
};

}