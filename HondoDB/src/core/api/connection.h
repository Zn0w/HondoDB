#pragma once

#include <string>


namespace hondo {

struct Connection
{
	std::string address;
	std::string port;
	std::string user;
	std::string password;
	std::string db_name;

	Connection(std::string s_address, std::string s_port, std::string s_user, std::string s_password, std::string s_db_name)
		: address(s_address), port(s_port), user(s_user), password(s_password), db_name(s_db_name)
	{}
};

}