#pragma once

#include <stdint.h>

#include "../util/string.h"


namespace hondo { namespace net {

enum MessageType
{
	Rename,		// request to rename db or collection
	
	Create,		// request to put new data into db
	Retrieve,	// request to retrieve some data from db
	Update,		// request to update some data in db
	Delete,		// request to delete some data in db

	Result		// once server processes the request and performs some action, it returns JSON data,
				// which contains:
				//		info about wether or not the requested action was done successfully, and 
				//		data associated with the requested action (e.g. if requested retrieve and it was successful, it contains retrieved data)
};

struct MessageHeader
{
	MessageType id;
	uint32_t size = 0;	// size of body
};

struct Message
{
	MessageHeader header;
	util::String body;

	// returns size of entire message in bytes
	size_t size() { return sizeof(MessageHeader) + (body.get_length() + 1); }
};

} }