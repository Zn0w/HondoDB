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
	HondoDB(util::String address, util::String port, util::String user, util::String password, util::String db_name);
	~HondoDB();
	
	Collection get(util::String name);
};

}