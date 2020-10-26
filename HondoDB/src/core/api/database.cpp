#include "database.h"


namespace hondo { 

/*
	TODO : once net is working, chack if able to establish connection, if not, set a temporary db environment in computer memory
*/
HondoDB::HondoDB(Connection c)
	: connection(c)
{
	
}

HondoDB::HondoDB(util::String address, util::String port, util::String user, util::String password, util::String db_name)
	: connection(address, port, user, password, db_name)
{
	
}

HondoDB::~HondoDB()
{
	
}

Collection HondoDB::get(util::String name)
{
	return Collection(&connection, name);
}

}