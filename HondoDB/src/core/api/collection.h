/*
	NOTE : Item is JSON data returned from the operations (for update, delete and create operations,
		JSON data representing the result of the operation will be returned)
*/

#pragma once

#include "../util/string.h"

#include "../item/item.h"

#include "connection.h"


namespace hondo {

class Collection
{
	util::String name;
	Connection* connection;


public:
	Collection(Connection* s_connection, util::String s_name);
	~Collection();

	Item create();
	Item retrieve();
	Item update();
	Item destroy();

	Item retrieve_all();

	Item nuke();
};

}