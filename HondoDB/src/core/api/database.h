#pragma once

#include "collection.h"


struct Connection
{
	String address;
	String port;
	String user;
	String password;
	String db_name;
};

class HondoDB
{
private:
	Connection connection;


public:
	Collection get(String name);
};