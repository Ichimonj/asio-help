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
	socket.connect(endpoint);

	for (;;) {
		//Create buffer 
		while (true) {
			string msg;
			cout << "Connected to server\nPrint msg: ";
			
			getline(cin, msg);
			asio::write(socket, asio::buffer(msg.data(), msg.size()), ec);
			if (ec) {
				cout << ec.message() << endl;
			}
		}
	}
	return 0;
}