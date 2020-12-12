#pragma once

#include <string>
#include <cstdio>
#include <cerrno>

#include <sys/stat.h>


namespace hondo {

struct StorageEngine
{
	std::string db_directory;

	StorageEngine(std::string db_directory);
	~StorageEngine();

	static std::string get_file_contents(std::string filename);
	static bool dir_exists(const char* dir_name);
};

}