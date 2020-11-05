#pragma once

#include <iostream>

#include "../net_common/client_interface.h"


namespace hondo {

class Client : public net::ClientInterface
{
protected:
	std::string username;
	std::string password;


public:
	Client(std::string s_username, std::string s_password)
		: username(s_username), password(s_password)
	{}
	
	void authenticate()
	{
		net::Message message;
		message.header.id = net::MessageType::Authenticate;
	}
};

}