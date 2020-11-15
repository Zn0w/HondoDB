#include <iostream>

#include <core/server/server.h>


int main()
{
	hondo::net::ServerInterface server(252525);
	server.start();

	return 0;
}