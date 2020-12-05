#include <iostream>

#include <core/server/server.h>


int main()
{
	system("dir");
	hondo::Server server(252525);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}