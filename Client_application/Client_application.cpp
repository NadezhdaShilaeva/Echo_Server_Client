﻿// Client_application.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Client_tcp.h"

int main(int argc, char* argv[])
{
	ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 8001);
	std::string message;

	while (true)
	{
		std::cout << "Enter message (or 'exit' to stop): ";
		getline(std::cin, message);

		if (message == "exit")
		{
			break;
		}
		else
		{
			boost::asio::io_context io_context;
			Client_tcp::Start(io_context, endpoint, message);
			io_context.run();
		}
	}

	return 0;
}
