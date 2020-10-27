#include "string.h"


namespace hondo { namespace util {

String::String()
{
	memory = 0;
	length = 0;
}

String::String(const char* value)
{
	put(value);
}

String::String(unsigned int new_length)
{
	memory = (char*) malloc(sizeof(char) * (new_length + 1));
	if (memory)
		length = new_length;
	else
		length = 0;
	memset(memory, '\0', length + 1);
}

//String::String(String string)
//{}

String::~String()
{
	free(memory);
}

void String::put(const char* value)
{
	put(value, count_length(value));
}

void String::put(String string)
{
	put(string.c_string(), string.get_length());
}

unsigned int String::get_length()
{
	return length;
}

void String::concat(const char* value)
{}

void String::concat(String string)
{}

bool String::contains(const char* value)
{
	return false;
}

bool String::contains(String string)
{
	return false;
}

const char* String::c_string()
{
	return memory;
}

char String::at(unsigned int position)
{
	if (memory && position <= length)
		return memory[position];
	else
		return 0;
}

void String::put_at(char c, unsigned int position)
{
	if (memory && position <= length)
		memory[position] = c;
}

void String::append(const char* value)
{}

void String::append(String string)
{}

void String::put(const char* value, unsigned int l)
{
	memory = (char*)malloc(sizeof(char) * (l + 1));
	if (memory)
		length = l;
	else
		length = 0;
	memcpy(memory, value, length + 1);
}

unsigned int String::count_length(const char* data)
{
	unsigned int l = 0;
	for (; data[l] != '\0'; l++);
	return l;
}

} }