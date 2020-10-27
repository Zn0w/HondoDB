/*
	NOTE : data structure to represent JSON data
*/

#pragma once

#include "../util/string.h"


namespace hondo {

enum ItemType
{
	Invalid,
	Number,
	String,
	Boolean,
	Array,
	Object,
	Null
};
	
struct Item
{
	// allows to go through object properties(name-value pairs) if is object / array elements(zero or more values of any type, ordered) if is array
	Item* prev;
	Item* next;

	// if is array / object, points to a set of items in array / object
	Item* child;

	// type of item
	ItemType type;

	// the item's name if it is a property of object
	util::String name;

	// if type == Number, integer part of number, if 
	int value_int;

	// if type == Number, the item's number
	double value_number;

	// if type == String, the item's string
	util::String value_string;
};

}