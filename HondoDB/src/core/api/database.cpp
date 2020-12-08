#include "database.h"


namespace hondo { 

/*
	TODO : once net is working, check if able to establish connection, if not, set a temporary db environment in computer memory
*/
HondoDB::HondoDB(Connection c)
	: connection(c)
{
	if (client.Connect(connection.address, std::stoi(connection.port)))
	{
		client.authenticate(connection.user, connection.password, connection.db_name);

		bool quit = false;
		while (!quit)
		{
			if (client.IsConnected())
			{
				if (!client.Incoming().empty())
				{
					auto msg = client.Incoming().pop_front().msg;

					switch (msg.header.id)
					{
					case hondo::MessageType::ServerAccept:
					{
						std::cout << "HondoDB Server Accepted Connection" << std::endl;
					}
					break;

					case hondo::MessageType::ServerDeny:
					{
						std::cout << "HondoDB Server Rejected Connection" << std::endl;
						quit = true;
					}
					break;

					case hondo::MessageType::ServerAuthSuccess:
					{
						std::cout << "HondoDB Auth Success" << std::endl;
					}
					break;

					case hondo::MessageType::ServerAuthFailure:
					{
						std::cout << "HondoDB Auth Failure" << std::endl;
						std::cout << msg.body << std::endl;
						quit = true;
					}
					break;
					}
				}
			}
			else
			{
				std::cout << "Server Down\n";
				quit = true;
			}
		}
	}
	else
		;// TODO
}

HondoDB::~HondoDB()
{
	
}

Collection HondoDB::get(std::string name)
{
	return Collection(&connection, name);
}

}