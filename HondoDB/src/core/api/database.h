#pragma once

#include <vector>

#include "collection.h"


namespace hondo {

class HondoDB
{
private:
	Connection connection;
	std::vector<Collection> collections;


public:
	HondoDB();	// set up a temporary db in computer memory
	HondoDB(Connection c);
	HondoDB(std::string address, std::string port, std::string user, std::string password, std::string db_name);
	~HondoDB();
	
	Collection get(std::string name);
};

}