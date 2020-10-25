#pragma once

#include <fstream>
#include <filesystem>

#include "../util/string.h"


namespace hondo { namespace storage {

struct StorageFunctionResult
{
	bool success;
	util::String message;
	// data
};

StorageFunctionResult create_database();
StorageFunctionResult create_collection();

StorageFunctionResult get_collection(util::String name);
StorageFunctionResult get_collections(util::String name);

} }