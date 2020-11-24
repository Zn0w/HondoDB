#pragma once

#include <vector>

#include "collection.h"


namespace hondo {

class HondoDB
{
private:
	Client client;
	Connection connection;
	std::vector<Collection> collections;


public:
	HondoDB();	// set up a temporary db in computer memory
	HondoDB(Connection c);
	~HondoDB();
	
	Collection get(std::string name);
};

}