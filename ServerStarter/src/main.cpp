#include <iostream>

#include <core/server/server.h>


int main()
{
	hondo::Server server(27575);
	server.start();

	while (true)
	{
		server.update();
	}
	
	return 0;
}