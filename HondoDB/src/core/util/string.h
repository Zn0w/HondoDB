/*
	NOTE : length DOES NOT include null terminator, both unsigned int get_length(); and unsigned int count_length(const char* data); return
		length WITHOUT counting null terminator
*/

#pragma once

#include <cstdlib>


namespace hondo { namespace util {

//typedef const char* String;

class String
{
	char* memory;
	unsigned int length;


public:
	String();
	String(const char* value);
	String(unsigned int length);
	//String(String string);
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

	void append(const char* value);
	void append(String string);

private:
	void put(const char* value, unsigned int l);
	unsigned int count_length(const char* data);
};

} }