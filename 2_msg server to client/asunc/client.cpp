#include<asio.hpp>
#include<iostream>
using namespace std;

//read func
vector<char>buf(100);
void read(asio::ip::tcp::socket& socket) {
	//asynchronous socket read
	socket.async_read_some(asio::buffer(buf.data(), buf.size()),
		[&](const std::error_code& ec, size_t length) {
			for (int i = 0; i < length; i++) {
				cout << buf[i];
			}
			read(socket);

		});
}

int main() {
	//I/O functionality
	asio::io_context io_context;

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.async_connect(endpoint,
		[&](error_code ec)
		{
			if (ec)
			{
				cout << ec.message() << endl;
			}
			else {
				cout << "Connect\n";
				read(socket);
			}
		});

	//asynchronous socket reasing
	io_context.run();

	return 0;
}