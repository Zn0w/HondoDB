#pragma once

#include "collection.h"


namespace hondo {

struct Connection
{
	util::String address;
	util::String port;
	util::String user;
	util::String password;
	util::String db_name;

	Connection(util::String s_address, util::String s_port, util::String s_user, util::String s_password, util::String s_db_name)
		: address(s_address), port(s_port), user(s_user), password(s_password), db_name(s_db_name)
	{}
};

class HondoDB
{
private:
	Connection connection;


public:
	HondoDB(Connection c);
	HondoDB(util::String address, util::String port, util::String user, util::String password, util::String db_name);
	~HondoDB();
	
	Collection get(util::String name);
};

}