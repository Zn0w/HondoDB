#pragma once

#include <iostream>

#define ASIO_STANDALONE

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


namespace hondo {

void test_net()
{
	asio::error_code error_code;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", error_code), 80);

	asio::ip::tcp::socket socket(context);

	socket.connect(endpoint, error_code);

	if (!error_code)
	{
		std::cout << "Successfully connected!" << std::endl;
	}
	else
	{
		std::cout << "Failed to connect: " << error_code.message() << std::endl;
	}
}

}