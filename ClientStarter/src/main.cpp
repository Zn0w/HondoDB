/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>


int main()
{
	hondo::Client client("daniil", "12345");
	client.connect("127.0.0.1", 27575);

	bool accepted = false;

	if (client.is_connected())
	{
		if (accepted)
		{
			client.authenticate();
		}

		if (!client.incoming().empty())
		{
			auto message = client.incoming().pop_front().message;
			switch (message.header.id)
			{
			case hondo::net::MessageType::ServerAccept:
			{
				std::cout << "Server Accepted Connection" << std::endl;
				accepted = true;
			} break;
			}
		}
	}

	return 0;
}