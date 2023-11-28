// Server_application.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "Server_tcp.h"

int main(int argc, char* argv[])
{
	try {
		boost::asio::io_context io_context;

		Server_tcp server(io_context, 8001);
		server.Start();

		io_context.run();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}
