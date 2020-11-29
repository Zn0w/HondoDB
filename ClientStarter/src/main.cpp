/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>

using asio::ip::tcp;


int main(int argc, char* argv[])
{
	hondo::HondoDB db(hondo::Connection("127.0.0.1", "252525", "daniil", "12345", "cars"));

	return 0;
}