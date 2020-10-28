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

String::String(const String& string)
{
	memory = (char*)malloc(sizeof(char) * (string.length + 1));
	if (memory)
		length = string.length;
	else
		length = 0;
	memcpy(memory, string.memory, length + 1);
}

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
{
	concat(value, count_length(value));
}

void String::concat(String string)
{
	concat(string.c_string(), string.get_length());
}

bool String::contains(const char* value)
{
	return contains(value, count_length(value));
}

bool String::contains(String string)
{
	return contains(string.c_string(), string.get_length());
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

std::ostream& operator<<(std::ostream& os, const String& string)
{
	if (string.memory)
		os << string.memory;
	else
		os << "";

	return os;
}

void String::put_at(char c, unsigned int position)
{
	if (memory && position <= length)
		memory[position] = c;
}

void String::put(const char* value, unsigned int l)
{
	if (memory)
	{
		memory = (char*) realloc(memory, l + 1);
	}
	else
	{
		memory = (char*)malloc(sizeof(char) * (l + 1));
	}

	if (memory)
		length = l;
	else
		length = 0;
	memcpy(memory, value, length + 1);
}

void String::concat(const char* value, unsigned int l)
{
	if (memory)
	{
		memory = (char*)realloc(memory, length + l + 1);
	}
	else
	{
		memory = (char*)malloc(sizeof(char) * (length + l + 1));
	}

	if (memory)
	{
		memcpy(memory + length, value, l + 1);
		length += l;
	}
	else
		length = 0;
}

bool String::contains(const char* value, unsigned int l)
{
	if (!memory)
		return false;
	
	int start_pos;
	for (start_pos = 0; memory[start_pos] != value[0] && start_pos <= length; start_pos++);

	int i;
	for (i = 0; i < l && memory[start_pos + i] == value[i]; i++);

	return i == l;
}

unsigned int String::count_length(const char* data)
{
	unsigned int l = 0;
	for (; data[l] != '\0'; l++);
	return l;
}

} }