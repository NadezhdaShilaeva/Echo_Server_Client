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

class Client_tcp : public boost::enable_shared_from_this<Client_tcp>
{
public:
	Client_tcp(io_context& io_context, const std::string& message)
		: context(io_context), socket(io_context), message(message)
	{}

	~Client_tcp()
	{
		Stop();
	}

	static boost::shared_ptr<Client_tcp> Start(io_context& io_context, ip::tcp::endpoint endpoint, const std::string& message)
	{
		boost::shared_ptr<Client_tcp> newClient(new Client_tcp(io_context, message));
		newClient->Start(endpoint);

		return newClient;
	}

	void Start(ip::tcp::endpoint endpoint)
	{
		try
		{
			auto self(shared_from_this());
			socket.async_connect(endpoint,
				[this, self](boost::system::error_code ec)
				{
					if (!ec)
					{
						std::cout << "Connected to server.\n";
						WriteMessage(message);
					}
					else
					{
						std::cerr << "Error from client: " << ec << '\n';
						Stop();
					}
				});
		}
		catch (std::exception& e)
		{
			std::cerr << "Error from client: " << e.what() << '\n';
			Stop();
		}
	}

	bool IsStarted()
	{
		return socket.is_open();
	}

	void Stop()
	{
		if (IsStarted())
		{
			socket.close();
		}
	}

private:
	void WriteMessage(const std::string& message)
	{
		std::ostream write_buf(&write_buffer);
		write_buf << message << '\n';

		auto self(shared_from_this());
		async_write(socket, write_buffer,
			[this, self](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					ReadMessage();
				}
				else
				{
					std::cerr << "Error from client: " << ec << '\n';
					Stop();
				}
			});
	}

	void ReadMessage()
	{
		auto self(shared_from_this());
		async_read_until(socket, read_buffer, "\n",
			[this, self](boost::system::error_code ec, std::size_t lenght)
			{
				if (!ec) {
					std::string serverMessage;
					std::istream read_buf(&read_buffer);
					getline(read_buf, serverMessage);

					std::cout << "Message from server: " << serverMessage << '\n';
				}
				else
				{
					std::cerr << "Error from client: " << ec << '\n';
				}

				Stop();
			});
	}

private:
	io_context& context;
	ip::tcp::socket socket;
	std::string message;
	boost::asio::streambuf read_buffer;
	boost::asio::streambuf write_buffer;
};
