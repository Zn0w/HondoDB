/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>

using namespace hondo::net;

using asio::ip::tcp;


int main(int argc, char* argv[])
{
	hondo::net::ClientInterface client;
	client.connect("127.0.0.1", 252525);

	while (true)
	{
		client.update();
	}

	system("pause");

	return 0;
}