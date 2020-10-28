/*
	NOTE : length DOES NOT include null terminator, both unsigned int get_length(); and unsigned int count_length(const char* data); return
		length WITHOUT counting null terminator
*/

#pragma once

#include <cstdlib>
#include <string>
#include <iostream>


namespace hondo { namespace util {

class String
{
	char* memory = 0;
	unsigned int length = 0;


public:
	String();
	String(const char* value);
	String(unsigned int length);
	String(const String& string);
	~String();

	void put(const char* value);
	void put(String string);
	
	unsigned int get_length();
	
	void concat(const char* value);
	void concat(String string);

	bool contains(const char* value);
	bool contains(String string);

	const char* c_string();

	char at(unsigned int position);

	void put_at(char c, unsigned int position);

	friend std::ostream& operator<<(std::ostream& os, const String& string);

private:
	void put(const char* value, unsigned int l);
	void concat(const char* value, unsigned int l);
	bool contains(const char* value, unsigned int l);
	unsigned int count_length(const char* data);
};

} }