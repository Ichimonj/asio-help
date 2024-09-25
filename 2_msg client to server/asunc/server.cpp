#include<asio.hpp>
#include<iostream>
#include<vector>
using namespace std;

//read func
vector<char>buf(100);
void read(asio::ip::tcp::socket& socket) {
	//asynchronous socket read
	socket.async_read_some(asio::buffer(buf.data(), buf.size()),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				for (int i = 0; i < length; i++) {
					cout << buf[i];
				}
				read(socket);
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
				read(socket);
			}
		});
	//asynchronous socket reasing
	io_context.run();
	return 0;
}