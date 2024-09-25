#include<asio.hpp>
#include<iostream>
using namespace std;

void startWrite(asio::ip::tcp::socket& soket, size_t length);

vector<char>buf(100);

//async reading data
void startRead(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(buf),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				for (int i = 0; i < length; i++) {
					cout << buf[i];
				}
				cout << '\n';
				startWrite(socket, length);
			}
			else {
				cout << ec.message() << endl;
			}

		});
}

//async write data
void startWrite(asio::ip::tcp::socket& soket,size_t length) {
	asio::async_write(soket, asio::buffer(buf.data(), length),
		[&](const std::error_code&ec,size_t length) {
			if (!ec) {
				cout << "write data" << endl;
			}
			else {
				cout << ec.message() << endl;
			}
		});
}

int main() {
	error_code ec;

	//I/O functionality
	asio::io_context io_context;

	//the acceptor object listens for connections at the address
	asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::make_address_v4("127.0.0.1"), 8080));

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//waits for a connection with the client and writes it to the socket
	acceptor.accept(socket, ec);

	if (ec) {
		cout << ec.message() << endl;
		return 1;
	}
	cout << "Client connected\n";

	startRead(socket);

	io_context.run();

	return 0;
}