#pragma once

#include <string>
#include <cstdio>
#include <cerrno>

#include "../util/string.h"


namespace hondo {

struct StorageEngine
{
	util::String root_directory;
	util::String db_directory;

	StorageEngine(util::String root_directory, util::String db_directory);
	~StorageEngine();

	util::String get_file_contents(util::String filename);
};

}