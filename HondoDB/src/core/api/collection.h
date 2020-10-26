#pragma once

#include "../util/string.h"

#include "connection.h"


namespace hondo {

struct CollectionFunctionResult
{
	bool success;
	util::String message;
	// data
};

class Collection
{
	util::String name;
	Connection* connection;


public:
	Collection(Connection* s_connection, util::String s_name);
	~Collection();

	CollectionFunctionResult create();
	CollectionFunctionResult retrieve();
	CollectionFunctionResult update();
	CollectionFunctionResult destroy();

	CollectionFunctionResult retrieve_all();

	CollectionFunctionResult nuke();
};

}