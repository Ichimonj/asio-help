#include<asio.hpp>
#include<iostream>
#include<vector>

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

	//Create buffer 
	vector<char>buf(100);

	//reading into buffer and determining size
	size_t lenth = socket.read_some(asio::buffer(buf), ec);
	if (ec) {
		cout << ec.message();
		return 1;
	}

	//output of the received message
	for (int i = 0; i < lenth; i++) {
		cout << buf[i];
	}
	
	return 0;
}