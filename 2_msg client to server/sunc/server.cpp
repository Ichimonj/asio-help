#include<asio.hpp>
#include<iostream>
#include<vector>
using namespace std;

int main() {
	error_code ec;

	//I/O functionality
	asio::io_context io_context;

	//the acceptor object listens for connections at the address
	asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//waits for a connection with the client and writes it to the socket
	acceptor.accept(socket,ec);
	if (ec) {
		cout << ec.message();
		return 1;
	}
	cout << "Connect\n";
	
	//Create buffer 
	vector<char>buf(100);

	//reading into buffer and determining size
	size_t length = socket.read_some(asio::buffer(buf), ec);
	if (ec) {
		cout << ec.message();
		return 1;
	}

	//output of the received message
	for (int i = 0; i < length; i++) {
		cout << buf[i];
	}
	return 0;

}