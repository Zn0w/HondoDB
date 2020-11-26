#pragma once

#include "../vendor/olc_net/olc_net.h"
#include "../vendor/cJSON/cJSON.h"


namespace hondo {

	enum MessageType
	{
		ServerAccept,
		ServerDeny,
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
			
			std::cout << "cJSON version: " << cJSON_Version() << std::endl;

			std::string auth_json_str = 
				"{'user':'" + user + "', " +
				"'password':'" + password + "', " +
				"'db_name':'" + db_name + "'" +
				"'}";
			//cJSON* auth_json = cJSON_Parse(auth_json_str.c_str());

			msg << auth_json_str.c_str();
			Send(msg);

			//cJSON_Delete(auth_json);
		}
		
		void ping_server()
		{
			olc::net::message<MessageType> msg;
			msg.header.id = MessageType::ServerPing;

			// Caution with this...
			std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

			msg << timeNow;
			Send(msg);
		}

		void message_all()
		{
			olc::net::message<MessageType> msg;
			msg.header.id = MessageType::MessageAll;
			Send(msg);
		}
	};

}