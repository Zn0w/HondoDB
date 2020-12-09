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
		client_server_thread = std::thread([this]() { process_client_server_interactions(); });
	}
	else
		status = DatabaseObjectStatus::ConnectFail;
}

HondoDB::~HondoDB()
{
	quit = true;

	if (client_server_thread.joinable())
		client_server_thread.join();
}


rapidjson::Document HondoDB::create()
{
	
}

rapidjson::Document HondoDB::retrieve()
{}

rapidjson::Document HondoDB::update()
{}

rapidjson::Document HondoDB::destroy()
{}

rapidjson::Document HondoDB::retrieve_all()
{}

rapidjson::Document HondoDB::nuke()
{}

DatabaseObjectStatus HondoDB::get_status()
{
	return status;
}


void HondoDB::process_client_server_interactions()
{
	client.authenticate(connection.user, connection.password, connection.db_name);

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
					status = DatabaseObjectStatus::Connected;
				}
				break;

				case hondo::MessageType::ServerDeny:
				{
					std::cout << "HondoDB Server Rejected Connection" << std::endl;
					quit = true;
					status = DatabaseObjectStatus::Denied;
				}
				break;

				case hondo::MessageType::ServerAuthSuccess:
				{
					std::cout << "HondoDB Auth Success" << std::endl;
					status = DatabaseObjectStatus::Authorized;
				}
				break;

				case hondo::MessageType::ServerAuthFailure:
				{
					std::cout << "HondoDB Auth Failure" << std::endl;
					std::cout << msg.body << std::endl;
					quit = true;
					status = DatabaseObjectStatus::ServerAuthFail;
				}
				break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			quit = true;
			status = DatabaseObjectStatus::ServerDown;
		}
	}
}

}