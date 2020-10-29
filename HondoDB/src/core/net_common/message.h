#pragma once

#include "common.h"


namespace hondo { namespace net {

enum MessageType
{
	ServerAccept,	// to let client know it's been accepted
	
	Rename,			// request to rename db or collection
	
	Create,			// request to put new data into db
	Retrieve,		// request to retrieve some data from db
	Update,			// request to update some data in db
	Delete,			// request to delete some data in db

	Result			// once server processes the request and performs some action, it returns JSON data,
					// which contains:
					//		info about wether or not the requested action was done successfully, and 
					//		data associated with the requested action (e.g. if requested retrieve and it was successful, it contains retrieved data)
};

struct MessageHeader
{
	MessageType id = Result;
	uint32_t size = 0;	// size of body
};

struct Message
{
	MessageHeader header;
	std::vector<uint8_t> body;


	// returns size of entire message in bytes
	size_t size() { return sizeof(MessageHeader) + body.size(); }
};

class Connection;

// When server gets message from client, it stores it in this structure, to be able to send a response back to that client
struct OwnedMessage
{
	std::shared_ptr<Connection> remote = nullptr;
	Message message;
};

} }