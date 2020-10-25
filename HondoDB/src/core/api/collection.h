#pragma once

#include "../util/string.h"

#include "../storage_engine/storage.h"


namespace hondo {

struct CollectionFunctionResult
{
	bool success;
	util::String message;
	// data
};

struct Collection
{
	util::String name;

	CollectionFunctionResult create();
	CollectionFunctionResult retrieve();
	CollectionFunctionResult update();
	CollectionFunctionResult destroy();

	CollectionFunctionResult retrieve_all();

	CollectionFunctionResult nuke();
};

}