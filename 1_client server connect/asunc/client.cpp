#include<asio.hpp>
#include<iostream>
using namespace std;

int main() {
	//I/O functionality
	asio::io_context io_context;

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);
	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.connect(endpoint);
	cout << "Connect\n";

	//Establishes a socket connection
	socket.async_connect(endpoint,
		[&](const error_code ec)
		{
			if (ec)
			{
				cout << ec.message() << endl;
			}
			else {
				cout << "Connect\n";
			}
		});

	//asynchronous socket reasing
	io_context.run();
	return 0;
}
