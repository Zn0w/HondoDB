/*
	NOTE : Item is JSON data returned from the operations (for update, delete and create operations,
		JSON data representing the result of the operation will be returned)
*/

#pragma once

#include <string>

#include "../json/item.h"
#include "../client/client.h"

#include "connection.h"


namespace hondo {

class Collection
{
	std::string name;
	Connection* connection;


public:
	Collection(Connection* s_connection, std::string s_name);
	~Collection();

	Item create();
	Item retrieve();
	Item update();
	Item destroy();

	Item retrieve_all();

	Item nuke();
};

}