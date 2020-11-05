#include "database.h"


namespace hondo { 

/*
	TODO : once net is working, check if able to establish connection, if not, set a temporary db environment in computer memory
*/
HondoDB::HondoDB(Connection c)
	: connection(c)
{
	
}

HondoDB::HondoDB(std::string address, std::string port, std::string user, std::string password, std::string db_name)
	: connection(address, port, user, password, db_name)
{
	
}

HondoDB::~HondoDB()
{
	
}

Collection HondoDB::get(std::string name)
{
	return Collection(&connection, name);
}

}