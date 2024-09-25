#include<asio.hpp>
#include<iostream>
using namespace std;

int main() {	
	//I/O functionality
	asio::io_context io_context;

	//the acceptor object listens for connections at the address
	asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//waits for a connection with the client and writes it to the socket
	acceptor.accept(socket);
	cout << "Connect\n";

	return 0;
}