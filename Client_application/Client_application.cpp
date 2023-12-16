// Client_application.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Client_tcp.h"

int main(int argc, char* argv[])
{
	boost::asio::io_context io_context;
	ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), 8001);
	boost::shared_ptr<Client_tcp> client = Client_tcp::Start(io_context, endpoint);

	std::string message;

	while (true)
	{
		getline(std::cin, message);

		if (message == "exit")
		{
			break;
		}
		else
		{
			client->WriteMessage(message);
		}
	}

	client->Stop();

	return 0;
}
