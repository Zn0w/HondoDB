/*
	NOTE : For now, this is a test for core features
*/

#include <iostream>
#include <cassert>

#include "core/api/api.h"

#include "core/client/client.h"


int main()
{
	hondo::HondoDB mydb("d://myhondodb", "", "daniil", "1234", "mydb");
	mydb.get("customers").retrieve_all();

	hondo::test_net();

	hondo::util::String s("hello world!");
	assert(s.get_length() == 12);
	std::cout << s << std::endl;

	s.put(":)");
	assert(s.get_length() == 2);
	std::cout << s << std::endl;

	hondo::util::String s2;
	assert(s2.get_length() == 0);
	std::cout << s2 << std::endl;

	s2.put("12345");
	assert(s2.get_length() == 5);
	std::cout << s2 << std::endl;

	s.put(s2);
	assert(s.get_length() == 5);
	std::cout << s << std::endl;

	assert(s.contains("34"));
	assert(!s.contains("abc"));
	assert(!s.contains("56"));

	s2.put("Hello World!");
	s.put("World");
	assert(s2.contains(s));

	hondo::util::String s3("Hello, "), s4("World!"), s5;
	s5.concat(s3);
	s5.concat(s4);
	std::cout << s5 << std::endl;
	

	system("pause");
	
	return 0;
}