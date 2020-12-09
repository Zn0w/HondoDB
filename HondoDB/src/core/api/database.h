#pragma once

#include <vector>
#include <thread>

#include "collection.h"


namespace hondo {

class HondoDB
{
private:
	Client client;
	Connection connection;
	std::vector<Collection> collections;

	std::thread client_server_thread;
	bool quit = false;


public:
	HondoDB();	// set up a temporary db in computer memory
	HondoDB(Connection c);
	~HondoDB();
	
	Collection get(std::string name);

private:
	void process_client_server_interactions();
};

}