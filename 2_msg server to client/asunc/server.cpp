#include<asio.hpp>
#include<iostream>
using namespace std;

//write func
void out(asio::ip::tcp::socket& socket, string& msg) {
	//asynchronous socket write
	asio::async_write(socket, asio::buffer(msg.data(), msg.length()),
		[&](const error_code& ec, size_t length) {
			if (ec) {
				cout << ec.message();
			}
			else {
				cout << "out - " << length << " byte" << endl;
			}

		});
}

int main() {
	//I/O functionality
	asio::io_context io_context;

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//the acceptor object listens for connections at the address
	asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));

	//waits for a connection with the client and writes it to the socket
	acceptor.async_accept(socket,
		[&](const error_code& ec) {
			if (ec)
			{
				cout << ec.message() << endl;
			}
			else {
				cout << "Connect\n";

				string msg = "hello asio";
				out(socket, msg);
			}
		});
	
	//asynchronous socket reasing
	io_context.run();
	return 0;
}