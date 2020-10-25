#pragma once

#include "../util/string.h"


struct FunctionResult
{
	bool success;
	String message;
};

struct Collection
{
	String name;

	FunctionResult create();
	FunctionResult retrieve();
	FunctionResult update();
	FunctionResult destroy();

	FunctionResult nuke();
};