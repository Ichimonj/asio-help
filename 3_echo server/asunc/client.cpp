#include<asio.hpp>
#include<iostream>
using namespace std;

vector<char>buf(100);

//async reading data
void startRead(asio::ip::tcp::socket& socket) {
	socket.async_read_some(asio::buffer(buf),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				for (int i = 0; i < length; i++) {
					cout << buf[i];
				}
			}
			else {
				cout << ec.message() << endl;
			}

		});
}

//async write data
void startWrite(asio::ip::tcp::socket& soket,string msg) {
	asio::async_write(soket, asio::buffer(msg.data(),msg.size()),
		[&](const std::error_code& ec, size_t length) {
			if (!ec) {
				cout << "write data" << endl;
				startRead(soket);
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

	//ip addresses
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("127.0.0.1"), 8080);

	//TCP socket
	asio::ip::tcp::socket socket(io_context);

	//Establishes a socket connection
	socket.connect(endpoint, ec);
	if (ec) {
		cout << ec.message() << endl;
		return 1;
	}
	cout << "Connected to server\nPrint msg: ";

	string msg;
	getline(cin, msg);

	startWrite(socket, msg);

	io_context.run();
	return 0;
}