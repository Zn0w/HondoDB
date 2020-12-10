/*
	NOTE : Client
*/

#include <iostream>

#include <core/api/api.h>

#include <core/client/client.h>

using asio::ip::tcp;


int main(int argc, char* argv[])
{
	hondo::HondoDB db(hondo::Connection("127.0.0.1", "252525", "daniil", "12345", "car_shop"));

	rapidjson::Document new_car_object_json;
	new_car_object_json.SetObject();
	rapidjson::Document::AllocatorType& allocator = new_car_object_json.GetAllocator();

	new_car_object_json.AddMember("model_name", "hondocarv2", allocator);
	new_car_object_json.AddMember("price", "5000", allocator);

	db.create("cars", new_car_object_json, [](rapidjson::Document& response_json) {
		rapidjson::StringBuffer strbuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
		response_json.Accept(writer);

		std::cout << "Output response json from the handler function: " << strbuf.GetString() << std::endl;
	});

	system("pause");

	return 0;
}