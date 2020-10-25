#pragma once

#include "../util/string.h"


namespace hondo {

struct FunctionResult
{
	bool success;
	util::String message;
};

struct Collection
{
	util::String name;

	FunctionResult create();
	FunctionResult retrieve();
	FunctionResult update();
	FunctionResult destroy();

	FunctionResult nuke();
};

}