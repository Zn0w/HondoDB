/*
	NOTE : For now, this is a test for core features
*/

#include <iostream>

#include "core/api/api.h"


int main()
{
	hondo::HondoDB mydb("d://myhondodb", "", "daniil", "1234", "mydb");
	mydb.get("customers").retrieve_all();
	

	system("pause");
	
	return 0;
}