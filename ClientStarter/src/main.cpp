/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>

using asio::ip::tcp;


int main(int argc, char* argv[])
{
	hondo::HondoDB db(hondo::Connection("127.0.0.1", "252525", "daniil", "12345", "car_shop"), [](hondo::DatabaseObjectStatus status) {
		if (status == hondo::DatabaseObjectStatus::Authorized)
		{
			std::cout << "Was successfully connected & authorized" << std::endl;
			return 1;
		}
		else if (status == hondo::DatabaseObjectStatus::ServerAuthFail)
		{
			std::cout << "Didn't authorize" << std::endl;
			return 1;
		}
		else if (status == hondo::DatabaseObjectStatus::ConnectFail)
		{
			std::cout << "Failed to connect" << std::endl;
			return 1;
		}
		else if (status == hondo::DatabaseObjectStatus::ServerDown)
		{
			std::cout << "Server was down" << std::endl;
			return 1;
		}
		else if (status == hondo::DatabaseObjectStatus::Connected)
		{
			std::cout << "Something wrong happened while trying to send auth request or tryeing to recieve auth response" << std::endl;
			return 1;
		}
		else
		{
			std::cout << "Something unexpected happened" << std::endl;
			return 1;
		}
	});

	rapidjson::Document new_car_object_json;
	new_car_object_json.SetObject();
	rapidjson::Document::AllocatorType& test_create_allocator = new_car_object_json.GetAllocator();

	new_car_object_json.AddMember("model_name", "hondocarv2", test_create_allocator);
	new_car_object_json.AddMember("price", "5000", test_create_allocator);

	db.create("cars", new_car_object_json, [](rapidjson::Document& response_json) {
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		response_json.Accept(writer);

		std::cout << "Create op result: " << strbuf.GetString() << std::endl;
	});

	rapidjson::Document condition_object_json;
	condition_object_json.SetObject();
	rapidjson::Document::AllocatorType& test_retrieve_allocator = condition_object_json.GetAllocator();

	condition_object_json.AddMember("cond", "id > 30", test_retrieve_allocator);

	db.retrieve("cars", condition_object_json, [](rapidjson::Document& response_json) {
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		response_json.Accept(writer);

		std::cout << "Retieve op result: " << strbuf.GetString() << std::endl;
	});

	system("pause");

	return 0;
}