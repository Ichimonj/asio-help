#include<asio.hpp>
#include<iostream>
using namespace std;

int main() {
	error_code ec;

	//I/O functionality
	asio::io_context io_context;

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.connect(endpoint,ec);
	if (ec) {
		cout << ec.message();
		return 1;
	}

	string msg = "hello asio";

	//synchronous socket writing
	asio::write(socket, asio::buffer(msg.data(), msg.length()), ec);
	if (ec) {
		cout << ec.message();
		return 1;
	}
	return 0;
}