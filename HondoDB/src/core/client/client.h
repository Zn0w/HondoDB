#pragma once

#include <iostream>

#include "../net_common/client_interface.h"


namespace hondo {

class Client : public net::ClientInterface
{
protected:
	util::String username;
	util::String password;


public:
	Client(util::String s_username, util::String s_password)
		: username(s_username), password(s_password)
	{}
	
	void authenticate()
	{
		net::Message message;
		message.header.id = net::MessageType::Authenticate;
	}
};

}