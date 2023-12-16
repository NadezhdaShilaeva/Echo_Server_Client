#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(ip::tcp::socket socket)
		: socket(std::move(socket))
	{}

	~Session()
	{
		Stop();
	}

	void Start()
	{
		std::cout << "Server connected to client.\n";
		ReadMessage();
	}

	void Stop()
	{
		if (IsStarted())
		{
			socket.close();
		}
	}

	bool IsStarted()
	{
		return socket.is_open();
	}

private:
	void ReadMessage()
	{
		auto self(shared_from_this());
		async_read_until(socket, buffer, "\n",
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::string clientMessage;
					std::istream read_buf(&buffer);
					getline(read_buf, clientMessage);

					std::ostream write_buf(&buffer);
					write_buf << clientMessage << '\n';

					std::cout << "Message from client: " << clientMessage << '\n';

					WriteMessage(length);
				}
				else
				{
					std::cerr << "Error from server: " << ec << '\n';
				}
			});
	}

	void WriteMessage(std::size_t length)
	{
		auto self(shared_from_this());
		async_write(socket, buffer,
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					buffer.consume(length);
					ReadMessage();
				}
				else
				{
					std::cerr << "Error from server: " << ec << '\n';
				}
			});
	}

private:
	ip::tcp::socket socket;
	streambuf buffer;
};

class Server_tcp : public boost::enable_shared_from_this<Server_tcp> 
{
public:
	Server_tcp(io_context& io_context, int port)
		: context(io_context), acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), port))
	{}

	~Server_tcp()
	{
		Stop();
	}

	void Start()
	{
		std::cout << "Server is started.\n";
		WaitForClient();
	}

	void Stop() 
	{
		context.stop();
		std::cout << "Server is stopped.\n";
	}

private:
	void WaitForClient()
	{
		acceptor.async_accept(
			[this](boost::system::error_code ec, ip::tcp::socket socket)
			{
				if (!ec)
				{
					auto session = std::make_shared<Session>(std::move(socket));
					session->Start();
				}
				else
				{
					std::cerr << "Error from server: " << ec << '\n';
				}

				WaitForClient();
			});
	}

private:
	io_context& context;
	ip::tcp::acceptor acceptor;
};