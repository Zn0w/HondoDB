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
		//client.ping_server();

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
						std::cout << "Server Accepted Connection\n";
					}
					break;

					case hondo::MessageType::ServerDeny:
					{
						std::cout << "Server Rejected Connection\n";
					}
					break;

					case hondo::MessageType::ServerPing:
					{
						std::cout << "Server Ping!\n";
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