#pragma once

#include <string>
#include <cstdio>
#include <cerrno>


namespace hondo {

struct StorageEngine
{
	std::string root_directory;
	std::string db_directory;

	StorageEngine(std::string root_directory, std::string db_directory);
	~StorageEngine();

	static std::string get_file_contents(std::string filename);
};

}