#pragma once

#include "../vendor/olc_net/olc_net.h"
#include "../vendor/rapidjson/rapidjson.h"


namespace hondo {

	enum MessageType
	{
		ServerAccept,
		ServerDeny,
		ServerAuthSuccess,
		ServerAuthFailure,
		ServerMessage,
		ServerPing,
		MessageAll,
		Authenticate,
		DBQuery,
		DBQueryResult
	};
	
	class Client : public olc::net::client_interface<MessageType>
	{
	public:
		void authenticate(std::string user, std::string password, std::string db_name)
		{
			olc::net::message<MessageType> msg;
			msg.header.id = MessageType::Authenticate;

			std::string auth_json_str = "{\"user\":\"" + user + "\",\"" + password + "\":\"12345\",\"" + db_name + "\":\"cars\"}";

			msg << auth_json_str;
			Send(msg);
		}
		
		void ping_server()
		{
			/*olc::net::message<MessageType> msg;
			msg.header.id = MessageType::ServerPing;

			// Caution with this...
			std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

			msg << timeNow;
			Send(msg);*/
		}

		void message_all()
		{
			olc::net::message<MessageType> msg;
			msg.header.id = MessageType::MessageAll;
			Send(msg);
		}
	};

}